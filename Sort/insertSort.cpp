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
void insertSort(vector<int> &arr)
{
    for (int i = 1; i < arr.size(); i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
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