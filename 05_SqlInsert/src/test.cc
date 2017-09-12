/*************************************************************************
	> File Name: test.cc
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Tue 12 Sep 2017 11:10:15 AM CST
 ************************************************************************/
#include "SqlInsert.h"
#include<iostream>
using std::cout;
using std::endl;
using std::string;
int main()
{
    SqlInsert mysql("login_password","login");
    std::string rowNameList("username,salt,password");
    std::string rowInforList("'look1','$6$23','1234#asd'");//实际应用中，三个字符串是分开的，这里做了处理，便于插入数据库
    mysql.insertToSql(rowNameList,rowInforList); 
    return 0;
}

