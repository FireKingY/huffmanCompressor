#include "Compresser.h"
#include <iostream>
using namespace std;

int main()
{
    Compresser com;
    string name;

    char in;
    while(1)
    {
        cout<<"1.压缩文件"<<endl;
        cout<<"2.解压缩文件"<<endl;
        cout<<"3.退出"<<endl;

        cin>>in;
        if(in == 1)
        {
            cout<<"请输入文件名，包含后缀名"<<endl;
            cin>>name;
            com.compress(name);
        }
        else if(in == 2)
        {
            cout<<"请输入文件名，包含后缀名"<<endl;
            cin>>name;
            com.depress(name);           
        }
        else
        {
            return 0;
        }
    }
}