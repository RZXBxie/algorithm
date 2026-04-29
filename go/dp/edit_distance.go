package dp

// MinEditDistance 计算将字符串 s1 转换为 s2 所需的最小编辑距离（Levenshtein 距离）。
// 允许的操作有三种，每种操作的代价均为 1：
//
//	dp[i][j]: 将s1的前i个字符转为s2的前j个字符需要的最小次数
//	1. dp[i-1][j] + 1: 将s1的前i-1个字符转为s2的前j个字符，并删除s1的第i个字符
//	2. dp[i][j-1] + 1: 将s1的前i个字符转为s2的前j-1个字符，并在s1的末尾新增一个字符
//	3. dp[i-1][j-1] + 1: 将s1的前i-1个字符转为s2的前j-1个字符，且将s1的第i个字符替换为s2的第j个字符
//
// 时间复杂度 O(m*n)，空间复杂度 O(m*n)，其中 m、n 分别为两个字符串的长度。
func MinEditDistance(s1, s2 string) int {
	r1 := []rune(s1)
	r2 := []rune(s2)
	m, n := len(r1), len(r2)

	// dp[i][j] s1的前i个字符转为s2的前j个字符需要最小次数
	dp := make([][]int, m+1)
	for i := range dp {
		dp[i] = make([]int, n+1)
	}

	// 边界：空串转换需要逐字符插入/删除
	for i := 0; i <= m; i++ {
		dp[i][0] = i
	}
	for j := 0; j <= n; j++ {
		dp[0][j] = j
	}

	for i := 1; i <= m; i++ {
		for j := 1; j <= n; j++ {
			if r1[i-1] == r2[j-1] {
				dp[i][j] = dp[i-1][j-1]
			} else {
				dp[i][j] = min3(
					dp[i-1][j]+1,
					dp[i][j-1]+1,
					dp[i-1][j-1]+1,
				)
			}
		}
	}

	return dp[m][n]
}

func min3(a, b, c int) int {
	m := a
	if b < m {
		m = b
	}
	if c < m {
		m = c
	}
	return m
}
