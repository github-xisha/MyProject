/*************************************************************************
	> File Name: SqlInsert.cc
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Tue 12 Sep 2017 11:09:24 AM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include<iostream>
#include <map>
using namespace std;
int main()
{
    map<string,string> _rowNameMap;
    string rowNameList("ab cd ef");
    string rowInforList("1234 56 1000");
    int pre=0,pre2=0,j=0;
    for(int i=0;i<=rowNameList.size();++i)
    {
        if((rowNameList[i]==' ')||(i==rowNameList.size()))
        {
            for(j=pre2;j<=rowInforList.size();++j)
            {
                if((rowInforList[j]==' ')||(j==rowInforList.size()))
                {
                    _rowNameMap[rowNameList.substr(pre,i-pre)]=rowInforList.substr(pre2,j-pre2);
                    pre=i+1;
                    pre2=j+1;
                    break;
                }
            }    
        }
    }
    cout<<"ab="<<_rowNameMap["ab"]<<endl;
    cout<<"cd="<<_rowNameMap["cd"]<<endl;
    cout<<"ef="<<_rowNameMap["ef"]<<endl;
    return 0;
}
