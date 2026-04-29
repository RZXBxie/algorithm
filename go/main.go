package main

import (
	"fmt"

	"github.com/rzxbxie/algorithm/go/sort"
)

func main() {
	arr := []int{38, 27, 43, 3, 9, 82, 10}
	fmt.Println("排序前:", arr)
	fmt.Println("排序后:", sort.MergeSort(arr))
}
