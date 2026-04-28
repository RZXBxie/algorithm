package sort

import (
	"reflect"
	"testing"
)

func TestMergeSort(t *testing.T) {
	tests := []struct {
		name string
		in   []int
		want []int
	}{
		{"empty", []int{}, []int{}},
		{"single", []int{1}, []int{1}},
		{"sorted", []int{1, 2, 3}, []int{1, 2, 3}},
		{"reversed", []int{5, 4, 3, 2, 1}, []int{1, 2, 3, 4, 5}},
		{"duplicates", []int{3, 1, 2, 3, 1}, []int{1, 1, 2, 3, 3}},
		{"negative", []int{-3, 0, -1, 5, 2}, []int{-3, -1, 0, 2, 5}},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := MergeSort(tt.in)
			if !reflect.DeepEqual(got, tt.want) {
				t.Errorf("MergeSort(%v) = %v, want %v", tt.in, got, tt.want)
			}
		})
	}
}
