package backtrack

// Combine 返回 nums 的所有组合（n 个元素中选择 m 个，回溯法）
// 时间复杂度: O(m * C(n, m))  空间复杂度: O(m)
//   - 共有 C(n, m) 个组合
//   - 每找到一个组合需要 O(m) 拷贝
func Combine(nums []int, m int) [][]int {
	var result [][]int
	n := len(nums)

	// 边界处理：非法参数直接返回空
	if m < 0 || m > n {
		return result
	}
	// m == 0 时，返回一个空组合
	if m == 0 {
		return [][]int{{}}
	}

	var path []int

	var backtrack func(start int)
	backtrack = func(start int) {
		// 终止条件：路径长度达到 m，找到一个完整组合
		if len(path) == m {
			tmp := make([]int, m)
			copy(tmp, path)
			result = append(result, tmp)
			return
		}

		// 剪枝：剩余元素不足以凑齐 m 个时提前结束
		// 还需要选 (m - len(path)) 个，当前下标最远只能到 n - (m - len(path))
		for i := start; i <= n-(m-len(path)); i++ {
			// 做选择
			path = append(path, nums[i])
			// 递归：下一次从 i+1 开始，保证组合不重复、不回头
			backtrack(i + 1)
			// 撤销选择（回溯）
			path = path[:len(path)-1]
		}
	}

	backtrack(0)
	return result
}

// CombineAll 返回nums的全组合
// 时间复杂度 2^n
func CombineAll(nums []int) [][]int {
	var result [][]int
	n := len(nums)

	var path []int

	var backtrack func(start int)

	backtrack = func(start int) {
		// 每个递归节点都对应一个合法子集，直接收集
		tmp := make([]int, len(path))
		copy(tmp, path)
		result = append(result, tmp)

		for i := start; i < n; i++ {
			path = append(path, nums[i])
			backtrack(i + 1)
			path = path[:len(path)-1]
		}
	}

	backtrack(0)

	return result
}
