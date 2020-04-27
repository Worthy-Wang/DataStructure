#include <iostream>
#include <string>
using namespace std;

/*
字符串的模式匹配算法：
1.常规算法
2.KMP算法
*/

//1.常规算法 求字符串S在字符串T中的位置
int index(const char* s, const char* t)
{
    int i = 0, j = 0;
    while (s[i] && t[j])
    {
        if (s[i] == t[j])
            i++, j++;
        else
            j = j - i + 1, i = 0;
    }

    if ('\0' == s[i])
        return j - i;
    else
        return -1;
}


//2.KMP算法 求字符串S在字符串T中的位置
int KMP(const char* s, const char* t)
{
    
}

int main()
{
    cout << index("abc", "babababcd") << endl;        
    return 0;
}

