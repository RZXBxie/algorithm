package backtrack

import (
	"fmt"
	"reflect"
	"sort"
	"testing"
)

// normalize 对二维切片做规范化排序，便于无序比较：
// 1. 每个子切片内部排序（组合无序，用于比较时拉平）
// 2. 子切片之间按字典序排序
func normalize(in [][]int) [][]int {
	out := make([][]int, len(in))
	for i, v := range in {
		cp := append([]int(nil), v...)
		sort.Ints(cp)
		out[i] = cp
	}
	sort.Slice(out, func(i, j int) bool {
		return fmt.Sprint(out[i]) < fmt.Sprint(out[j])
	})
	return out
}

func TestCombine(t *testing.T) {
	tests := []struct {
		name    string
		nums    []int
		m       int
		want    [][]int
		wantLen int // C(n, m)
	}{
		{"空数组取 0 个", []int{}, 0, [][]int{{}}, 1},
		{"非法: m<0", []int{1, 2, 3}, -1, nil, 0},
		{"非法: m>n", []int{1, 2}, 3, nil, 0},
		{"m=0 返回空组合", []int{1, 2, 3}, 0, [][]int{{}}, 1},
		{"m=n 返回整体", []int{1, 2, 3}, 3, [][]int{{1, 2, 3}}, 1},
		{"C(4,2)=6", []int{1, 2, 3, 4}, 2,
			[][]int{{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}, 6},
		{"C(5,3)=10", []int{1, 2, 3, 4, 5}, 3, nil, 10},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := Combine(tt.nums, tt.m)

			// 数量校验
			if len(got) != tt.wantLen {
				t.Errorf("Combine(%v, %d) 返回 %d 个组合, 期望 %d 个",
					tt.nums, tt.m, len(got), tt.wantLen)
			}

			// 每个组合长度校验
			for _, c := range got {
				if tt.m >= 0 && len(c) != tt.m {
					t.Errorf("组合 %v 长度为 %d, 期望 %d", c, len(c), tt.m)
				}
			}

			// 查重
			seen := map[string]bool{}
			for _, c := range got {
				key := fmt.Sprint(c)
				if seen[key] {
					t.Errorf("发现重复组合: %v", c)
				}
				seen[key] = true
			}

			// 具体内容比较（want 非 nil 时）
			if tt.want != nil {
				if !reflect.DeepEqual(normalize(got), normalize(tt.want)) {
					t.Errorf("Combine(%v, %d) = %v, want %v", tt.nums, tt.m, got, tt.want)
				}
			}
		})
	}
}

func TestCombineAll(t *testing.T) {
	tests := []struct {
		name    string
		nums    []int
		wantLen int // 2^n
		want    [][]int
	}{
		{"空数组", []int{}, 1, [][]int{{}}},
		{"单元素", []int{1}, 2, [][]int{{}, {1}}},
		{"两个元素", []int{1, 2}, 4, [][]int{{}, {1}, {1, 2}, {2}}},
		{"三个元素", []int{1, 2, 3}, 8,
			[][]int{{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}}},
		{"四个元素", []int{1, 2, 3, 4}, 16, nil},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := CombineAll(tt.nums)

			// 数量校验：2^n
			if len(got) != tt.wantLen {
				t.Errorf("CombineAll(%v) 返回 %d 个子集, 期望 %d 个 (2^%d)",
					tt.nums, len(got), tt.wantLen, len(tt.nums))
			}

			// 查重
			seen := map[string]bool{}
			for _, c := range got {
				key := fmt.Sprint(c)
				if seen[key] {
					t.Errorf("发现重复子集: %v", c)
				}
				seen[key] = true
			}

			// 具体内容比较
			if tt.want != nil {
				if !reflect.DeepEqual(normalize(got), normalize(tt.want)) {
					t.Errorf("CombineAll(%v) = %v, want %v", tt.nums, got, tt.want)
				}
			}
		})
	}
}

func TestCombine_Print(t *testing.T) {
	nums := []int{1, 2, 3, 4}
	m := 2
	result := Combine(nums, m)
	t.Logf("输入: %v, m=%d", nums, m)
	t.Logf("组合 (%d 个):", len(result))
	for i, c := range result {
		t.Logf("  %d: %v", i+1, c)
	}
}

func TestCombineAll_Print(t *testing.T) {
	nums := []int{1, 2, 3}
	result := CombineAll(nums)
	t.Logf("输入: %v", nums)
	t.Logf("全组合 (%d 个):", len(result))
	for i, c := range result {
		t.Logf("  %d: %v", i+1, c)
	}
}
