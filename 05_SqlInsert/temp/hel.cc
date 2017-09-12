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

SqlInsert::createSurstr(std::string rowNameList,std::string rowInforList)
{
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
}

SqlInsert::insertToSql(std::string rowNameList,std::string rowInforList)
{
    createSurstr(rowNameList,rowInforList);
    

}


