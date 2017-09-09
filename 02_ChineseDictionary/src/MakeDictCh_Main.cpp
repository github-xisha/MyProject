#include "MakeDictCh.h"
#include "ReadDir.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, const char *argv[])
{
    const char * const dict_path =  "../dict/jieba.dict.utf8";
    const char * const model_path = "../dict/hmm_model.utf8";
    string readfilename = "../data/ch_in.txt";
    string readExcludeFile = "../data/ch_exclude.txt";
    string writefilename = "../data/ch_out.txt";
    //下载到本地后，下面一条语句中的路径需要修改
    ReadDir dir(string("/home/xisha/MyGitHub/MyProject/02_ChineseDictionary/data/art/"));
    std::vector<std::string> fileNames=dir.getFileNames();
    MakeDictCh makedict(dict_path, model_path);
    //下载到本地后，下面一条语句中的路径需要修改
    string ss="/home/xisha/MyGitHub/MyProject/02_ChineseDictionary/data/art/";
    string path;
    for(int idx=0;idx!=fileNames.size();++idx)
    {
        path=ss+fileNames[idx];
        makedict.readfile(path, readExcludeFile);
    }
    makedict.writefile(writefilename);
    return 0;
}
