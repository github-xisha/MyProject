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

void SqlInsert::insertToSql(std::string rowNameList,std::string rowInforList)
{
    _command=std::string("insert into ")+_table+std::string("(")+rowNameList+std::string(") values(")+rowInforList+std::string(")");
    //std::cout<<_command<<std::endl;
    int t;
    _conn=mysql_init(NULL);
    if(!mysql_real_connect(_conn,_server.c_str(),_user.c_str(),_password.c_str(),_database.c_str(),0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(_conn));
    }else{
        //printf("connect success\n");   
    }
    t=mysql_query(_conn,_command.c_str());
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(_conn));
    }else{
       printf("insert success\n");
    }
    mysql_close(_conn);
}

