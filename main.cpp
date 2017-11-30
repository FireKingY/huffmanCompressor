#include "Compresser.h"
#include <iostream>
using namespace std;

int main()
{
    Compresser com;
    com.compress("in.txt");

    for(int i=0;i<256;++i)
    {
        if(com.cnt[i]!=0)
        {
            cout<<com.codes[i]<<endl;
        }
    }
}