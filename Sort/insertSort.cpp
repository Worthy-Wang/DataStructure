#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
直接插入排序

最好时间复杂度 :O(n)
平均时间复杂度 ：O(n*n)
最坏时间复杂度 :O(n*n)
空间复杂度 O(1)
为稳定排序
*/
//插入排序
void insertion_sort(vector<int> &nums)
{
    for (int k = 1; k < nums.size(); k++)
    {
        int key = nums[k];
        int i;
        for (i = k - 1; i >= 0 && nums[i] > key; i--)
            nums[i + 1] = nums[i];
        nums[i + 1] = key;
    }
}

void binary_insertion_sort(vector<int> &nums)
{
    for (int k = 1; k < nums.size(); k++)
    {
        int key = nums[k];
        int l = 0, r = k - 1;
        while (l <= r)
        {
            int mid = (l + r) >> 1;
            if (nums[mid] < key)
                l = mid + 1;
            else
                r = mid - 1;
        }
        int i;
        for (i = k - 1; i >= l; i--)
            nums[i + 1] = nums[i];
        nums[i + 1] = key;
    }
}

int main()
{
    vector<int> arr{4, 5, 2, 3, 1, 5, 3, 6};
    insertSort(arr);
    for (auto &e : arr)
        cout << e << endl;
    return 0;
}