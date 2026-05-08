#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int partition(vector<int> &arr, int left, int right) {
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
    swap(arr[i + 1], arr[right]);

    return i + 1;
}

void quick_sort(vector<int> &arr, int left, int right) {
    if (left >= right) return;

    int index = partition(arr, left, right);
    quick_sort(arr, left, index - 1);
    quick_sort(arr, index + 1, right);
}

int quick_select(vector<int> &arr, int left, int right, int target) {
    if (left == right) return arr[left];
    int index = partition(arr, left, right);
    if (index == target) return arr[index];
    if (index < target) return quick_select(arr, index + 1, right, target);
    if (index > target) return quick_select(arr, left, index - 1, target);

    return -1;
}

int find_kth_largest(vector<int> &arr, int k) {
    int n = arr.size();
    return quick_select(arr, 0, n - 1, n - k);
}

int main() {
    vector<int> arr = {3, 5, 4, 1, 2};
    quick_sort(arr, 0, arr.size() - 1);
    for (int num: arr) {
        cout << num << ' ';
    }

    cout << endl;

    vector<int> arr2 = {3, 5, 4, 1, 2};
    cout << find_kth_largest(arr, 4) << endl;

    return 0;
}
