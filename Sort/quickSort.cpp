#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


template<typename Iterator, typename Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred)
{
    auto slow = first;
    auto fast = first;
    for (; fast != last; fast++)
        if(pred(*fast))
            swap(*slow++, *fast);
    return slow;
}

template <typename Iterator>
void quick_sort(Iterator first, Iterator last)
{
    if (first != last)
    {
        auto mid1 = Partition(first, last, bind2nd(less<int>(), *first));
        auto mid2 = Partition(first, last, bind2nd(less_equal<int>(), *first));
        quick_sort(first, mid1);
        quick_sort(mid2, last);
    }
}


int main()
{
    vector<int> nums{1, 5, 5, 3, 4, 4, 2, 5, 7, 9, 8, 6, 8, 0, 7, 6};
    quick_sort(nums.begin(), nums.end());
    for (auto &e : nums)
        cout << e << endl;
    return 0;
}