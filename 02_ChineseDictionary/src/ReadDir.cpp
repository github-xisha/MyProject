/*************************************************************************
	> File Name: ReadDir.cc
	> Author: XiSha
	> Email: 1540348087@qq.com
	> Created Time: Fri 08 Sep 2017 08:32:03 PM CST
 ************************************************************************/

#include "ReadDir.h"
#include <string.h>
#include <unistd.h>
#include<iostream>
#include<string>

ReadDir::ReadDir(std::string filePath)
:_filePath(filePath)
{
}
ReadDir::~ReadDir()
{
    closedir(_fdir);
}

std::vector<std::string> ReadDir::getFileNames()
{
    _fdir=opendir(_filePath.c_str());
    while((_dirInfor=readdir(_fdir))!=NULL)
    {
        if(strcmp(_dirInfor->d_name,".")&&strcmp(_dirInfor->d_name,"..")&&strcmp(_dirInfor->d_name,".git"))
        {
            _fileNames.push_back(std::string(_dirInfor->d_name));
        }
    }
    return _fileNames;
}


