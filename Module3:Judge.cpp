#include "Module.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

void Storestring(char* outputfile_path, std::vector<std::string>& vstrFile)
{
    fstream file;
    file.open(outputfile_path,ios::in);
    if(!file)
    {
        cout<<"Open output1.txt unsuccessfully!!"<<endl;
    }
    else
    {
        char arr[100] = {0};
        while(file >> arr)
        {
            std::string buf = arr;
            vstrFile.push_back(arr);
        }
    }
    file.close();
}

int Compare(char* outputfile_path1, char* outputfile_path2)
{
    std::vector<std::string> vstrFile1;
    std::vector<std::string> vstrFile2;
    
    Storestring(outputfile_path1, vstrFile1);
    Storestring(outputfile_path2, vstrFile2);

    if(vstrFile1.size() != vstrFile2.size())
    {
        return 0;
    }
    else
    {
        for(int i = 0;i < vstrFile1.size();i++)
        {
            if(vstrFile1[i] != vstrFile2[i])
            {
                return 0;
            }
        }
    }   
    return 1;
}
char* strmcpy(const char* t, int m)
{
    char s[100] = {0};
    int n = strlen(t);
    for(int i = 0;i<=n-m+1;i++)
    {
        if(t[m+1-i] == '\0') s[i] = '\0';
        s[i] = t[m+i-1];
    }
    return s;
}

void strmcpy(char *s, int m ,int n ,char *t)
{
    char *p = s,*q = t;
    p += m;
    while(*p != '\0'&& n)
    {
        *q = *p;
        p++;
        q++;
        n--;
    }
}

void SetResult(const char* file1_path, const char* file2_path, int result)
{
    char path1_relative[100] = {0};
    char path2_relative[100] = {0};
    strmcpy((char*)file1_path, 8 ,strlen(file1_path)-1, path1_relative);
    strmcpy((char*)file2_path, 8 ,strlen(file2_path)-1, path2_relative);
    printf("%s\n", path1_relative);
    printf("%s\n", path2_relative);

    char path_file[100] = {0};
    int m = 0;
    for(int j = strlen(file1_path)-1;;j--)
        if (file1_path[j] == '/'){
            m = j;break;
        }
                
    strncpy(path_file, file1_path, m);

    std::string csvpath1 = std::string(path_file) + std::string("/equal.csv");
    std::string csvpath2 = std::string(path_file) + std::string("/inequal.csv");


    if(result == 1)
    {
        FILE *fp;
        fp = 0;
        if((fp = fopen("./output/equal.csv","a"))==0)
        {
            printf("Fail to open the file!\n");
            //return -1;
        }
        fprintf(fp, "%s %s\n", path1_relative, path2_relative);
        fclose(fp);
    }
    else if(result == 0)
    {
        FILE *fp;
        fp = 0;
        if((fp = fopen("./output/inequal.csv","a"))==0)
        {
            printf("Fail to open the file!\n");
            //return -1;
        }
        fprintf(fp, "%s %s\n", path1_relative, path2_relative);
        fclose(fp);
    }
}
