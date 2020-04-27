#include <iostream>
#include <vector>
using namespace std;

//二分法：递归
int binary_search(vector<int> &arr, int l, int r, int key)
{
    if (l < r)
    {
        int mid = (l + r) >> 1;
        if (arr[mid] == key)
            return mid;
        else if (key > arr[mid])
            return binary_search(arr, mid + 1, r, key);
        else
            return binary_search(arr, l, mid - 1, key);
    }
    else
        return -1;
}


//二分法：非递归
int binary_search(vector<int>& arr, int key)
{
    int l = 0, r = arr.size()-1;
    while (l <= r)
    {
        int mid = (l + r) >> 1;
        if (key < arr[mid])
            r = mid - 1;
        else if (key > arr[mid])
            l = mid + 1;
        else
            return mid;
    }
    return -1;
}

int main()
{
    vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << binary_search(vec, 0) << endl;   
    return 0;
}