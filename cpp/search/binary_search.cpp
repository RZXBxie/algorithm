#include <iostream>
#include <vector>

using namespace std;

// first_appearance 查找第一个值为target的数据下标
int first_appearance(const vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low < high) {
        int mid = low + ((high - low) >> 1);
        if (nums[mid] < target) low = mid + 1;
        else high = mid;
    }

    return nums[low] == target ? low : -1;
}

// last_appearance 查找最后一个值为target的数据下标
int last_appearance(const vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low < high) {
        // 注意找右边界是向上取整，否则会死循环
        int mid = low + ((high - low + 1) >> 1);
        if (nums[mid] > target) high = mid - 1;
        else low = mid;
    }
    return nums[low] == target ? low : -1;
}

// first_bigger 查找第一个比target大的元素的下标
int first_bigger(const vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low < high) {
        int mid = low + ((high - low) >> 1);
        if (nums[mid] <= target) low = mid + 1;
        else high = mid;
    }
    return nums[low] > target ? -1 : low;
}

// last_smaller 查找最后一个比target小的元素
int last_smaller(const vector<int>& nums, int target) {
    int low = 0, high = nums.size() - 1;
    while (low < high) {
        int mid = low + ((high - low + 1) >> 1);
        if (nums[mid] >= target) high = mid - 1;
        else low = mid;
    }
    return nums[low] < target ? low : -1;
}

int binary_search(const vector<int>& nums, int target) {
    return first_appearance(nums, target);

    return -1;
}

int main() {
    vector<int> nums = {1, 2, 3, 4, 4, 6};
    cout << binary_search(nums, 4) << endl;
    cout << binary_search(nums, 1) << endl;
    cout << binary_search(nums, 5) << endl;

    return 0;
}