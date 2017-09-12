/*************************************************************************
	> File Name: SqlQuery.h
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Mon 11 Sep 2017 10:54:53 PM CST
 ************************************************************************/

#ifndef _SQLQUERY_H
#define _SQLQUERY_H

#include <mysql/mysql.h>
#include <string>

class SqlQuery
{
    public:
        SqlQuery(std::string datebase,std::string table,std::string inputRowName,std::string queryRowName);
        ~SqlQuery();
        std::string getQueryResult(std::string inputName);
    private:
        std::string _server;
        std::string _user;//数据库的用户名root
        std::string _password;//登录数据库密码
        
        std::string _database;//数据库库名
        std::string _table;//数据库下的表名
        std::string _inputRowName;//列名
        std::string _queryRowName;//列名
        std::string _command;//
        MYSQL *_conn;
        MYSQL_RES *_res;
        MYSQL_ROW _row;
};

#endif
