#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

// AVL树实现代码，参考文章：https://www.hello-algo.com/chapter_tree/avl_tree/ 实现
class AVLTree {
    struct TreeNode {
        int val;
        int height = 0;
        TreeNode *left;
        TreeNode *right;

        explicit TreeNode(const int x) : val(x), left(nullptr), right(nullptr) {
        }
    };

    TreeNode *root = nullptr;
    // 定义空节点高度为-1
    static int height(TreeNode *node) {
        if (!node) return -1;
        return node->height;
    }

    // 更新节点高度
    static void updateHeight(TreeNode *node) {
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    // 获取节点平衡因子
    // 节点平衡因子=左子树高度-右子树高度
    static int balanceFactor(const TreeNode *node) {
        if (!node) return 0;

        return height(node->left) - height(node->right);
    }

    // 右旋
    static TreeNode *rightRotate(TreeNode *node) {
        TreeNode *child = node->left;
        TreeNode *grandChild = child->right;

        // 以node为原点，将child向右旋转
        child->right = node;
        node->left = grandChild;

        // 更新高度
        updateHeight(node);
        updateHeight(child);

        return child;
    }

    // 左旋
    static TreeNode *leftRotate(TreeNode *node) {
        TreeNode *child = node->right;
        TreeNode *grandChild = child->left;

        child->left = node;
        node->right = grandChild;

        updateHeight(node);
        updateHeight(child);

        return child;
    }

    // 执行旋转操作，使该子树恢复平衡
    // 四种旋转操作的选择情况（记失衡节点为node，平衡因子表示为bf）：
    // 1. node.bf > 1, 左偏树; node.child.bf >= 0, 右旋
    // 2. node.bf > 1, 左偏树; node.child.bf < 0, 先左旋后右旋
    // 3. node.bf < -1, 右偏树; node.child.bf < 0, 左旋
    // 4. node.bf < -1, 右偏树; node.child.bf >= 0, 先右旋后左旋
    static TreeNode *rotate(TreeNode *node) {
        // 左偏树
        if (const int _balanceFactor = balanceFactor(node); _balanceFactor > 1) {
            // 右旋
            if (balanceFactor(node->left) >= 0) {
                return rightRotate(node);
            }

            // 先左旋后右旋
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // 右偏树
        if (const int _balanceFactor = balanceFactor(node); _balanceFactor < -1) {
            // 左旋
            if (balanceFactor(node->right) < 0) {
                return leftRotate(node);
            }

            // 先右旋后左旋
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // 平衡树，直接返回
        return node;
    }

    // 递归插入节点，并进行旋转操作以维持平衡
    static TreeNode *insert(TreeNode *node, const int val) {
        if (!node) return new TreeNode(val);
        if (val < node->val) {
            node->left = insert(node->left, val);
        } else if (val > node->val) {
            node->right = insert(node->right, val);
        } else {
            // 重复节点直接返回
            return node;
        }

        // 更新高度
        updateHeight(node);
        // 执行旋转操作，使该子树保持平衡
        return rotate(node);
    }

    /**
 * @brief 从 AVL 平衡二叉搜索树中删除指定值的节点（递归实现）
 *
 * @details 算法遵循经典 BST 删除逻辑，分为三种场景处理：
 *          1. 叶子节点：直接释放内存并返回 nullptr；
 *          2. 单侧子树：释放当前节点，让唯一的子节点替代其位置；
 *          3. 双侧子树：寻找右子树的后继节点（中序后继），先递归在右子树中删除该后继节点，
 *             然后再将后继节点的值覆盖到当前节点，以维持平衡结构的稳定性。
 *          每次递归退出前，均会触发高度更新与自平衡旋转，确保失衡向上传播时能被正确修复。
 *
 * @param node 当前递归进入的子树根节点指针
 * @param val  需要删除的目标键值
 * @return TreeNode* 返回经过旋转平衡调整后，当前子树的新根节点指针
 */
    static TreeNode *remove(TreeNode *node, const int val) {
        if (!node) return nullptr;
        if (val < node->val) {
            node->left = remove(node->left, val);
        } else if (val > node->val) {
            node->right = remove(node->right, val);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                TreeNode *child = node->left ? node->left : node->right;
                // 没有孩子
                if (!child) {
                    delete node;
                    return nullptr;
                }
                // 只有一个孩子
                delete node;
                node = child;
            }
            // 有两个孩子
            else {
                // 先找到中序遍历的后继结点
                TreeNode *tmp = node->right;
                while (tmp->left) {
                    tmp = tmp->left;
                }
                const int tmpVal = tmp->val;
                node->right = remove(node->right, tmpVal);
                node->val = tmpVal;
            }
        }
        updateHeight(node);
        return rotate(node);
    }

    static void inorder(TreeNode* node, vector<int>& nums) {
        if (!node) return;
        inorder(node->left, nums);
        nums.push_back(node->val);
        inorder(node->right, nums);
    }

    // 检查子树是否平衡，返回高度（平衡）或 -999（不平衡）
    static int checkBalance(TreeNode* node) {
        if (!node) return -1;
        int leftH = checkBalance(node->left);
        int rightH = checkBalance(node->right);
        if (leftH == -999 || rightH == -999) return -999;
        if (abs(leftH - rightH) > 1) return -999;
        return max(leftH, rightH) + 1;
    }

public:
    void insert(const int val) {
        root = insert(root, val);
    }

    void remove(const int val) {
        root = remove(root, val);
    }

    [[nodiscard]] vector<int> getElements() const {
        vector<int> ans;
        inorder(root, ans);
        return ans;
    }

    [[nodiscard]]bool isBalanced() const {
        return checkBalance(root) != -999;
    }

};

// 测试工具函数
bool isSorted(const vector<int>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i] <= v[i-1]) return false;
    }
    return true;
}

bool hasDuplicate(const vector<int>& v) {
    unordered_set<int> s(v.begin(), v.end());
    return s.size() != v.size();
}

void testInsertBasic() {
    cout << "=== Test: 基本插入 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 15, 3, 7, 12, 20};
    for (int v : vals) tree.insert(v);

    auto elements = tree.getElements();
    cout << "中序遍历: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testInsertSequence() {
    cout << "=== Test: 顺序插入（触发左旋）===" << endl;
    AVLTree tree;
    // 顺序插入会触发左旋
    for (int i = 1; i <= 7; i++) tree.insert(i);

    auto elements = tree.getElements();
    cout << "中序遍历: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testInsertReverse() {
    cout << "=== Test: 逆序插入（触发右旋）===" << endl;
    AVLTree tree;
    // 逆序插入会触发右旋
    for (int i = 7; i >= 1; i--) tree.insert(i);

    auto elements = tree.getElements();
    cout << "中序遍历: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testRemoveLeaf() {
    cout << "=== Test: 删除叶子节点 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 15, 3, 7, 12, 20};
    for (int v : vals) tree.insert(v);

    tree.remove(3);  // 删除叶子
    auto elements = tree.getElements();
    cout << "删除3后中序: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testRemoveNodeWithOneChild() {
    cout << "=== Test: 删除只有一个孩子的节点 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 15, 3};
    for (int v : vals) tree.insert(v);

    auto elements = tree.getElements();
    cout << "删除5后中序: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testRemoveNodeWithTwoChildren() {
    cout << "=== Test: 删除有两个孩子的节点 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 15, 3, 7, 12, 20};
    for (int v : vals) tree.insert(v);

    tree.remove(10);  // 删除根节点（有两个孩子）
    auto elements = tree.getElements();
    cout << "删除10后中序: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有序: " << (isSorted(elements) ? "✓" : "✗") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testRemoveAll() {
    cout << "=== Test: 逐个删除所有节点 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 15, 3, 7, 12, 20};
    for (int v : vals) tree.insert(v);

    for (int v : vals) {
        tree.remove(v);
        auto elements = tree.getElements();
        cout << "删除" << v << "后: ";
        for (int e : elements) cout << e << " ";
        cout << " | 平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
        if (!tree.isBalanced()) {
            cout << "  ⚠️ 不平衡！" << endl;
        }
        if (!elements.empty() && !isSorted(elements)) {
            cout << "  ⚠️ 无序！" << endl;
        }
    }
    cout << endl;
}

void testDuplicateInsert() {
    cout << "=== Test: 重复插入 ===" << endl;
    AVLTree tree;
    int vals[] = {10, 5, 10, 5, 20};
    for (int v : vals) tree.insert(v);

    auto elements = tree.getElements();
    cout << "中序遍历: ";
    for (int v : elements) cout << v << " ";
    cout << endl;

    cout << "是否有重复: " << (hasDuplicate(elements) ? "✗" : "✓") << endl;
    cout << "是否平衡: " << (tree.isBalanced() ? "✓" : "✗") << endl;
    cout << endl;
}

void testRandom() {
    cout << "=== Test: 随机数据压力测试 ===" << endl;
    // 用固定种子保证可复现
    mt19937 rng(42);
    uniform_int_distribution<int> dist(1, 1000);

    AVLTree tree;
    vector<int> inserted;

    // 随机插入100个数
    for (int i = 0; i < 100; i++) {
        int v = dist(rng);
        tree.insert(v);
        inserted.push_back(v);

        // 每10次检查一次
        if (i % 10 == 9) {
            auto elements = tree.getElements();
            if (!isSorted(elements)) {
                cout << "  ⚠️ 插入" << v << "后无序！" << endl;
            }
            if (!tree.isBalanced()) {
                cout << "  ⚠️ 插入" << v << "后不平衡！" << endl;
            }
        }
    }

    cout << "插入100个随机数后: ";
    cout << "有序: " << (isSorted(tree.getElements()) ? "✓" : "✗") << ", ";
    cout << "平衡: " << (tree.isBalanced() ? "✓" : "✗") << ", ";
    cout << "节点数: " << tree.getElements().size() << endl;

    // 随机删除50个数
    shuffle(inserted.begin(), inserted.end(), rng);
    for (int i = 0; i < 50; i++) {
        int v = inserted[i];
        tree.remove(v);

        auto elements = tree.getElements();
        if (!isSorted(elements)) {
            cout << "  ⚠️ 删除" << v << "后无序！" << endl;
        }
        if (!tree.isBalanced()) {
            cout << "  ⚠️ 删除" << v << "后不平衡！" << endl;
        }
    }

    cout << "删除50个随机数后: ";
    cout << "有序: " << (isSorted(tree.getElements()) ? "✓" : "✗") << ", ";
    cout << "平衡: " << (tree.isBalanced() ? "✓" : "✗") << ", ";
    cout << "节点数: " << tree.getElements().size() << endl;
    cout << endl;
}

int main() {
    testInsertBasic();
    testInsertSequence();
    testInsertReverse();
    testRemoveLeaf();
    testRemoveNodeWithOneChild();
    testRemoveNodeWithTwoChildren();
    testRemoveAll();
    testDuplicateInsert();
    testRandom();

    cout << "所有测试完成！" << endl;
    return 0;
}
