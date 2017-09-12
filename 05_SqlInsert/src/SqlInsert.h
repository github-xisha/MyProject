/*************************************************************************
	> File Name: SqlInsert.h
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Tue 12 Sep 2017 11:08:53 AM CST
 ************************************************************************/

#ifndef _SQLINSERT_H
#define _SQLINSERT_H

#include <mysql/mysql.h>
#include <string>

class SqlInsert
{
    public:
        SqlInsert(std::string datebase,std::string table);
        ~SqlInsert();
        void insertToSql(std::string rowNameList,std::string rowInforList);
    private:
        std::string _server;
        std::string _user;//数据库的用户名root
        std::string _password;//登录数据库密码
        
        std::string _database;//数据库库名
        std::string _table;//数据库下的表名
        std::string _command;
        MYSQL *_conn;
        MYSQL_RES *_res;
        MYSQL_ROW _row;
};

#endif
