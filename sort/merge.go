package sort

// MergeSort 对整数切片进行归并排序，返回排序后的新切片
func MergeSort(arr []int) []int {
	n := len(arr)
	if n <= 1 {
		return arr
	}

	mid := n / 2
	left := MergeSort(arr[:mid])
	right := MergeSort(arr[mid:])

	return merge(left, right)
}

// merge 合并两个有序切片
func merge(left, right []int) []int {
	result := make([]int, 0, len(left)+len(right))
	i, j := 0, 0

	for i < len(left) && j < len(right) {
		if left[i] <= right[j] {
			result = append(result, left[i])
			i++
		} else {
			result = append(result, right[j])
			j++
		}
	}

	result = append(result, left[i:]...)
	result = append(result, right[j:]...)
	return result
}
