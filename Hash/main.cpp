#include <iostream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <memory>
#include <limits>
#include <list>
#include <regex>
#include <functional>
#include <math.h>
#include <unordered_set>
#include <iomanip>
using namespace std;

/*
哈希表：
理解上可以理解成C++中的unordered_set
构造方法采用 除留余数法
解决冲突的方法采用 开放定址法的线性探测法
*/

const int NULLKEY = INT32_MIN;
class  HashTable
{
   int *_elem; 
   int _size;

public:
   HashTable(int size)
   :_size(size)
   {
      cout << "HashTable()" << endl;
      _elem = (int*)malloc(_size*sizeof(int));
      for (int i = 0; i < _size; ++i)
            _elem[i] = NULLKEY;
   }

   int Hash(int key) //哈希函数的构造方法，采用除留余数法
   {
      return key % _size;
   }

   void insert(int key)
   {
      int addr = Hash(key);
      while (_elem[addr] != NULLKEY)//如果发生冲突，那么采用开放地址法中的线性探测法
         addr = (addr + 1) % _size;
      _elem[addr] = key;
   }

   bool search(int key)
   {
      int addr = Hash(key);
      while(_elem[addr] != key)
      {
         if (_elem[addr] == NULLKEY)
            return false; //关键字不存在
         addr = (addr + 1) % _size;
         if (addr = Hash(key))
            return false; //重新回到原点
      }
      return true;
   }

   void print() const
   {
      cout << "下标：";
      for (int i = 0; i < _size; ++i)
         cout << setw(4) << i;
      cout << endl;
      
      cout << "关键字:";
      for (int i = 0; i < _size; ++i)
         cout << setw(4) << _elem[i];
      cout << endl;
   }

};


int main()
{
   vector<int> nums{12, 67, 56, 16, 25, 37, 22, 29, 15, 47, 48, 34};
   HashTable hashtable(nums.size());
   for (auto& e: nums)
      hashtable.insert(e);
   hashtable.print();      
   return 0;
}
