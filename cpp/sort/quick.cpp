#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int partition(vector<int>& arr, int left, int right) {
    int pivot = arr[right];

    // i维护比pivot小的区域的右边界，初始状态下就是left-1
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            // 找到位置了，i向右移一位
            i++;
            swap(arr[i], arr[j]);
        }
    }

    // 将pivot放到最终位置
    swap(arr[i+1], arr[right]);

    return i+1;
}

void quick_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int index = partition(arr, left, right);
    quick_sort(arr, left, index - 1);
    quick_sort(arr, index + 1, right);
}

int main() {
    vector<int> arr = {3, 5, 4, 1, 2};
    quick_sort(arr, 0, arr.size() - 1);
    for (int num : arr) {
        cout << num << ' ';
    }

    return 0;
}