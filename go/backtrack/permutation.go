package backtrack

// Permutation 返回 nums 的所有全排列（回溯法）
// 时间复杂度: O(n * n!)  空间复杂度: O(n)
func Permutation(nums []int) [][]int {
	var result [][]int
	n := len(nums)
	used := make([]bool, n)
	var path []int

	var backtrack func()
	backtrack = func() {
		// 终止条件：路径长度等于数组长度，找到一个完整排列
		if len(path) == n {
			tmp := make([]int, n)
			copy(tmp, path)
			result = append(result, tmp)
			return
		}

		for i := 0; i < n; i++ {
			if used[i] {
				continue
			}
			// 做选择
			path = append(path, nums[i])
			used[i] = true
			// 递归
			backtrack()
			// 撤销选择（回溯）
			path = path[:len(path)-1]
			used[i] = false
		}
	}

	backtrack()
	return result
}
