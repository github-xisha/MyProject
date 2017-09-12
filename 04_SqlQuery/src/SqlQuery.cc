/*************************************************************************
	> File Name: SqlQuery.cc
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Mon 11 Sep 2017 11:13:24 PM CST
 ************************************************************************/

#include "SqlQuery.h"
#include <stdio.h>
#include <string.h>
#include<iostream>

SqlQuery::SqlQuery(std::string database,std::string table,std::string inputRowName,std::string queryRowName)
:_server("localhost")
,_user("root")
,_password("123")
,_database(database)
,_table(table)
,_inputRowName(inputRowName)
,_queryRowName(queryRowName)
{
    _command=std::string("select ")+_queryRowName+std::string(" from ")+_table+std::string(" where ")+_inputRowName;
}
SqlQuery::~SqlQuery()
{
}

std::string SqlQuery::getQueryResult(std::string inputName)
{
    _command=_command+std::string("='")+inputName+std::string("'");
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
        _res=mysql_use_result(_conn);
        if(_res)
        {
            while((_row=mysql_fetch_row(_res))!=NULL)
            {
                for(t=0;t<mysql_num_fields(_res);t++)
                {
        		    return std::string(_row[t]);
                }
                break;
            }
        }
        mysql_free_result(_res);
    }
    mysql_close(_conn);
}

