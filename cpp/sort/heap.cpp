#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void heapify(vector<int>& arr, int n, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != index) {
        swap(arr[index], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // 阶段 1: 构建大顶堆
    // 从最后一个非叶子节点 (n/2 - 1) 开始，逆序向上进行 heapify
    // 叶子节点天然满足堆的性质，无需处理
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    // 阶段 2: 堆排序
    // 每次将堆顶元素放到末尾，然后执行heapify操作
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);

        // 堆的有效大小缩小了一个，现在是i
        heapify(arr, i, 0);
    }
}

// 使用堆解决top-k问题
priority_queue<int, vector<int>, greater<>> topKHeap(vector<int>& arr, int k) {
    int n = arr.size();
    priority_queue<int, vector<int>, greater<>> pq;
    for (int i = 0; i < k; ++i) {
        pq.push(arr[i]);
    }

    for (int i = k; i < n; ++i) {
        if (arr[i] > pq.top()) {
            pq.pop();
            pq.push(arr[i]);
        }
    }

    return pq;
}


int main() {
    vector<int> arr = {12, 11, 13, 5, 6, 7};

    cout << "排序前: ";
    for (int num : arr) cout << num << " ";
    cout << endl;

    heap_sort(arr);

    cout << "排序后: ";
    for (int num : arr) cout << num << " ";
    cout << endl;

    return 0;
}