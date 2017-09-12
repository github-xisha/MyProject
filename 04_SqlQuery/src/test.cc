/*************************************************************************
	> File Name: test.cc
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Mon 11 Sep 2017 11:45:15 PM CST
 ************************************************************************/

#include "SqlQuery.h"
#include<iostream>
using std::cout;
using std::endl;
using std::string;
int main()
{
    //SqlQuery mysql("login_password","login","username","salt");//分别是数据库名、表名、列名、列名
    SqlQuery mysql("login_password","login","username","password");//分别是数据库名、表名、列名、列名
    string result=mysql.getQueryResult("look");//username列名下的look--->salt列名下的值
    cout<<"result="<<result<<endl;
    return 0;
}

