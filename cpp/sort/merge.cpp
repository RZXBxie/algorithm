#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void merge_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    const int mid = left + ((right - left) >> 1);
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    vector<int> tmp;
    tmp.reserve(right - left + 1);
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) tmp.push_back(arr[i++]);
        else                  tmp.push_back(arr[j++]);
    }
    while (i <= mid)   tmp.push_back(arr[i++]);
    while (j <= right) tmp.push_back(arr[j++]);

    std::copy(tmp.begin(), tmp.end(), arr.begin() + left);
}

int main() {
    vector<int> arr = {3, 5, 1, 4, 2};
    if (!arr.empty()) {
        merge_sort(arr, 0, static_cast<int>(arr.size()) - 1);
    }
    for (const int i : arr) cout << i << ' ';
    cout << '\n';
    return 0;
}