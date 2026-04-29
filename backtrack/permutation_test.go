package backtrack

import (
	"fmt"
	"testing"
)

func TestPermutation(t *testing.T) {
	tests := []struct {
		name     string
		nums     []int
		wantLen  int // 期望排列数量 = n!
	}{
		{"空数组", []int{}, 1},
		{"单元素", []int{1}, 1},
		{"两个元素", []int{1, 2}, 2},
		{"三个元素", []int{1, 2, 3}, 6},
		{"四个元素", []int{1, 2, 3, 4}, 24},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := Permutation(tt.nums)
			if len(result) != tt.wantLen {
				t.Errorf("Permutation(%v) 返回 %d 个排列, 期望 %d 个", tt.nums, len(result), tt.wantLen)
			}

			// 检查每个排列的长度是否正确
			for _, perm := range result {
				if len(perm) != len(tt.nums) {
					t.Errorf("排列 %v 长度为 %d, 期望 %d", perm, len(perm), len(tt.nums))
				}
			}

			// 检查是否有重复排列
			seen := make(map[string]bool)
			for _, perm := range result {
				key := fmt.Sprint(perm)
				if seen[key] {
					t.Errorf("发现重复排列: %v", perm)
				}
				seen[key] = true
			}
		})
	}
}

func TestPermutation_Print(t *testing.T) {
	nums := []int{1, 2, 3}
	result := Permutation(nums)
	t.Logf("输入: %v", nums)
	t.Logf("全排列 (%d 个):", len(result))
	for i, perm := range result {
		t.Logf("  %d: %v", i+1, perm)
	}
}
