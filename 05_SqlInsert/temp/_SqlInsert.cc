/*************************************************************************
	> File Name: SqlInsert.cc
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Tue 12 Sep 2017 11:09:24 AM CST
 ************************************************************************/

#include "SqlInsert.h"
#include <stdio.h>
#include <string.h>
#include<iostream>

SqlInsert::SqlInsert(std::string database,std::string table)
:_server("localhost")
,_user("root")
,_password("123")
,_database(database)
,_table(table)
{
}
SqlInsert::~SqlInsert()
{
}

//std::string createSubstr(std::string rowNameList,std::string rowInforList)
//{
//    int pre=0,pre2=0,j=0;
//    for(int i=0;i<=rowNameList.size();++i)
//    {
//        if((rowNameList[i]==' ')||(i==rowNameList.size())
//        {
//            for(j=pre2;j<rowInforList.size();++j;)
//            {
//                if((rowInforList[j]==' ')||(j==rowInforList.size()))
//                {
//                    _rowNameMap[rowNameList.substr(pre,i-pre)]=rowInforList.substr(pre2,j-pre2)
//                    pre=i+1;
//                    pre2=j+1;
//                    break;
//                }
//            }
//        }
//    }
//}

void SqlInsert::insertToSql(std::string rowNameList,std::string rowInforList)
{
    //createSubstr(rowNameList,rowInforList);
   // std::vector<std::string> temp;
   // for(auto it=_rowNameMap.begin();it<_rowNameMap.end();++it)
   // {
   //     temp.push_back(*it);
   // }
   // std::string temp2("");
   // for(int idx=0;idx<temp.size();++idx)
   // {
   //     temp2=temp2+temp[idx];
   //     if(idx<temp.size()-1) 
   //         std::string(",");
   // }
    _command=std::string("insert into ")+_table+std::string("(")+rowNameList+std::string(") values(")+rowInforList+std::string(")");
    std::cout<<_command<<std::endl;
#if 1
    int t;
    _conn=mysql_init(NULL);
    if(!mysql_real_connect(_conn,_server.c_str(),_user.c_str(),_password.c_str(),_database.c_str(),0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(_conn));
    }else{
    
    }
    t=mysql_query(_conn,_command.c_str());
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(_conn));
    }else{
       printf("insert success\n");
    }
    mysql_close(_conn);
#endif    
}

