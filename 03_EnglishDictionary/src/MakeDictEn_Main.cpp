#include "MakeDictEn.h"
#include "ReadDir.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char *argv[])
{
    ReadDir dir("/home/xisha/MyGitHub/MyProject/03_EnglishDictionary/data/originEnglishLib/");
    vector<string> fileNames=dir.getFileNames();
    string en_outPath("../data/en_out.txt");
    string en_inPath;
    string tempPath("/home/xisha/MyGitHub/MyProject/03_EnglishDictionary/data/originEnglishLib/");
    MakeDictEn count;
    for(int idx=0;idx!=fileNames.size();++idx)
    {
        en_inPath=tempPath+fileNames[idx];
        count.readfile(en_inPath);
    }
    count.writefile(en_outPath);
    return 0;
}
