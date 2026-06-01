#include <string>
#include <vector>
#include <iostream>
using namespace std;

// 构造坏字符表
// 坏字符表的目的：当主串中出现坏字符X时，能在O(1)时间内找到模式串中最右边X的位置
// 假设只有ASCII表里255个字符
void generateBadCharTable(const string &pattern, vector<int> &bc) {
    int m = pattern.length();

    for (int i = 0; i < 256; ++i) {
        bc[i] = -1;
    }

    for (int i = 0; i < m; ++i) {
        const int ascii = pattern[i];
        bc[ascii] = i;
    }
}

// 构建suffix辅助数组
// suffix[i]的含义：以i结尾的模式串子串，能和模式串末尾匹配多长？
void generateSuffixArray(const string &pattern, vector<int> &suffix) {
    int m = pattern.length();
    suffix[m - 1] = m;

    for (int i = m - 2; i >= 0; --i) {
        int j = i;
        while (j >= 0 && pattern[j] == pattern[m - 1 - (i - j)]) {
            j--;
        }
        suffix[i] = i - j;
    }
}

// 构建goodSuffix表
// gs[i]的含义：当坏字符出现在下标i时，应该向右边滑动几步？
void generateGoodSuffixTable(const string &pattern, vector<int> &suffix, vector<int> &gs) {
    int m = pattern.length();

    // ------------------------------------------------
    // 情况 3：最差情况（无匹配）。直接滑动整个模式串长度 m
    // ------------------------------------------------
    for (int i = 0; i < m; ++i) {
        gs[i] = m;
    }

    // ------------------------------------------------
    // 情况 2：好后缀的部分后缀，能和模式串的头部（前缀）匹配
    // ------------------------------------------------
    int j = 0;
    for (int i = m - 1; i >= 0; --i) {
        // suffix[i] == i + 1 说明从 0 到 i 的子串，恰好也是整个串的后缀！
        if (suffix[i] == i + 1) {
            // 那么对于前面所有还没被填入真实位移值的坏字符位置
            for (; j < m - 1 - i; ++j) {
                if (gs[j] == m) {
                    gs[j] = m - 1 - i; // 滑动距离
                }
            }
        }
    }

    // ------------------------------------------------
    // 情况 1：最好情况！整个好后缀，在模式串前面的某处完整出现过
    // ------------------------------------------------
    for (int i = 0; i <= m - 2; ++i) {
        // m - 1 - suffix[i] 就是坏字符的位置
        // m - 1 - i 就是模式串需要向右滑动的距离
        // 因为是从左往右遍历，后面出现的覆盖前面的，保证了滑动距离最小
        gs[m - 1 - suffix[i]] = m - 1 - i;
    }
}

// Boyer-Moore 字符串匹配算法
// 核心思想：从模式串的尾部开始比较，利用「坏字符规则」和「好后缀规则」实现大幅跳跃。
// 时间复杂度：最好 O(n/m)，最坏 O(n*m)（加上 Galil 优化可达 O(n)）
// 空间复杂度：O(m + σ)，σ 为字符集大小（这里为 256）
void boyerMoore(const string& text, const string& pattern) {
    const int n = text.length();
    const int m = pattern.length();
    if (m == 0) return;

    // 1.构建bc和gs
    vector<int> bc(256);
    vector<int> suffix(m);
    vector<int> gs(m);
    generateBadCharTable(pattern, bc);
    generateSuffixArray(pattern, suffix);
    generateGoodSuffixTable(pattern, suffix, gs);

    int i = 0;

    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[i + j]) {
            j--;
        }
        if (j < 0) {
            // 情况 A：j 穿透了 0，说明整个模式串都匹配成功了！
            cout << "找到匹配，起始索引为: " << i << endl;

            // 为了寻找文本中的下一个匹配，我们需要继续滑动模式串。
            // 此时可以把整个模式串看作一个巨大的“好后缀”，按照 gs[0] 的指示滑动。
            i += gs[0];
        } else {
            // 情况 B：在 j 的位置发生了不匹配！
            // text[i + j] 就是那个“坏字符”

            // 锦囊 1：问坏字符规则要滑多远？
            // 坏字符在主串的位置 j，减去它在模式串里最靠右的位置 bc[text[i+j]]。
            // 如果模式串压根没这个字符（bc 值为 -1），那么 j - (-1) = j + 1，完美越过这个坏字符。
            int shift_bc = j - bc[text[i + j]];

            // 锦囊 2：问好后缀规则要滑多远？
            // 坏字符发生在 j，说明 j+1 到末尾是好后缀，直接查表！
            int shift_gs = gs[j];

            // 终极奥义：取两者的最大值进行跳跃！
            i += max(shift_bc, shift_gs);
        }
    }

}

// 构建next数组
// next[j]含义：在不包含pattern[j]的模式串中，最长公共前后缀的长度
// 例如abac，对应的next数组为[-1, 0, 0, 1]
// 注意：next[0] = -1 是哨兵，方便 KMP 主循环中识别「需要把模式串整体后移一位」的情形。
vector<int> generateNext(const string& pattern) {
    int m = pattern.length();
    vector<int> next(m);
    next[0] = -1;
    if (m <= 1) return next;
    next[1] = 0;
    // k：当前已经求出的「最长公共前后缀」长度，对应 next[i-1]
    int k = 0;
    for (int i = 2; i < m; ++i) {
        // 想求 next[i] = pattern[0..i-1] 的最长公共前后缀长度。
        // 已知 k = next[i-1]，现在尝试在末尾追加字符 pattern[i-1]。
        // 若 pattern[k] != pattern[i-1]，就回退 k = next[k]，直到能匹配或退到 0。
        while (k > 0 && pattern[k] != pattern[i - 1]) {
            k = next[k];
        }
        if (pattern[k] == pattern[i - 1]) {
            k++;
        }
        next[i] = k;
    }
    return next;
}

// kmp字符串匹配算法，参考文章：https://www.cnblogs.com/zzuuoo666/p/9028287.html 实现
// j=next[j]含义：模式串的0 ~ j-1匹配成功，将模式串向右移动 j-next[j]个位置，继续和text[i]匹配
// 返回值：首次匹配在主串中的起始下标；找不到返回 -1
int kmp(const string& text, const string& pattern) {
    const int n = text.length();
    const int m = pattern.length();
    if (m == 0) return 0;

    // 1. 构建 next 数组
    vector<int> next = generateNext(pattern);

    // 2. 开始匹配
    // i：主串当前比较位置；j：模式串当前比较位置
    int i = 0, j = 0;
    while (i < n) {
        // j == -1：是 next[0] 的哨兵值，表示模式串已经退无可退；
        //         此时让 i 和 j 同时前进，相当于把模式串整体右移一位，
        //         从主串的下一个字符重新开始匹配。
        // text[i] == pattern[j]：当前字符匹配成功，i 和 j 一起向前。
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;

            // 模式串走完，说明找到了一次完整匹配
            if (j == m) {
                return i - j;
            }
        } else {
            // 失配：模式串的 0 ~ j-1 部分已经匹配成功，
            // 利用 next[j]（即 pattern[0..j-1] 的最长公共前后缀长度）
            // 让 j 跳到 next[j] 继续比较，相当于模式串向右滑动 j - next[j] 步。
            // 当 j 退到 0 时，next[0] = -1 会触发上面的哨兵分支。
            j = next[j];
        }
    }
    return -1;
}

int main() {
    cout << "=== 测试1: 基本匹配 ===" << endl;
    // 期望输出: 索引 15
    boyerMoore("HERE IS A SIMPLE EXAMPLE", "EXAMPLE");

    cout << "\n=== 测试2: 多次匹配 ===" << endl;
    // 期望输出: 索引 0, 9, 12
    boyerMoore("AAAAAAAAAAAAAA", "AAAAA");

    cout << "\n=== 测试3: 不匹配 ===" << endl;
    // 期望输出: 无
    boyerMoore("ABCDEFGH", "XYZ");

    cout << "\n=== 测试4: 好后缀规则发挥作用 ===" << endl;
    // pattern = "ABCABC"，后缀 "ABC" 在前面出现过，好后缀规则能大步跳跃
    // 期望输出: 索引 15
    boyerMoore("ABCABDABCABDABCABCABC", "ABCABC");

    cout << "\n=== 测试5: 坏字符规则发挥作用 ===" << endl;
    // 文本中有大量不在模式串中的字符，坏字符规则直接跳过
    // 期望输出: 索引 10
    boyerMoore("ZZZZZZZZZZABCD", "ABCD");

    cout << "\n=== KMP 测试1: 基本匹配 ===" << endl;
    // 期望输出: 15
    cout << "首次匹配下标: " << kmp("HERE IS A SIMPLE EXAMPLE", "EXAMPLE") << endl;

    cout << "\n=== KMP 测试2: 重复字符 ===" << endl;
    // 期望输出: 0
    cout << "首次匹配下标: " << kmp("AAAAAAAAAAAAAA", "AAAAA") << endl;

    cout << "\n=== KMP 测试3: 不匹配 ===" << endl;
    // 期望输出: -1
    cout << "首次匹配下标: " << kmp("ABCDEFGH", "XYZ") << endl;

    cout << "\n=== KMP 测试4: next 数组发挥作用 ===" << endl;
    // 期望输出: 8  (主串 "ABABABABABABABCABAB" 中 "ABABABC" 起始下标)
    cout << "首次匹配下标: " << kmp("ABABABABABABABCABAB", "ABABABC") << endl;

    return 0;
}
