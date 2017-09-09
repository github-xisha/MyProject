/*************************************************************************
	> File Name: ReadDir.h
	> Author: XiSha
	> Email: 1540348087@qq.com
	> Created Time: Fri 08 Sep 2017 08:24:45 PM CST
 ************************************************************************/

#ifndef _READDIR_H
#define _READDIR_H

#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <vector>
class ReadDir
{
    public:
        ReadDir(std::string filePath);
        ~ReadDir();
        std::vector<std::string> getFileNames();
    private:
        std::string _filePath;
        std::vector<std::string> _fileNames;
        DIR* _fdir;
        struct dirent* _dirInfor;
};

#endif
