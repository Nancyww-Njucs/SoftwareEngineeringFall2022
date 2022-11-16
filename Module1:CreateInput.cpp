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

int CreateRandomInput(char* directory) 
{                
    std::string format_path = std::string(directory) + std::string("/stdin_format.txt");
    //------------------------以string形式读取stdin_format.txt-------------------
    FILE *fp;

    if((fp = fopen(format_path.c_str(),"r")) == 0)
    {
        printf("文件打开失败!\n");
        fclose(fp);
        return 0;
    }
    //printf("OPEN SUCCESSFULLY!");
    char szTest[1000] = {0};
    int len = 0;
    int count = 0;

    memset(szTest, 0, sizeof(szTest));
    while(1)
    {
        if(fgets(szTest,sizeof(szTest),fp)==0)
        {
            break;
        }                    
    }
    fclose(fp);

    int begin = 0,end = 0;
    for(int i = 0; szTest[i] != '\0'; i++)
    {
        if(szTest[i] != ' ')
        {
            end++;
            if(szTest[i+1] == '\0')
                HandleChar(szTest,begin,i);                              
        }
        else
        {
            end = i-1;
            HandleChar(szTest,begin,end);
            begin = i+1;
            end = begin;
        }                           
    }    
    return 0;
}

//------------------------Create random input ----> txt----------------------------
void HandleChar(char* arr, int begin, int end)
{
    FILE *fp;
    fp = 0;
    if((fp = fopen("./input.txt","a"))==0)
    {
        printf("文件打开失败!\n");
    }
    if(arr[begin] == 'i')
    {
        int low = 0;
        int high = 0;
        int i = begin + 4;
        for(; arr[i]!=','; i++)
            low = (arr[i] - '0') + low*10;

        for(int j = i+1;arr[j]!=')';j++)
            high = (arr[j] - '0') + high*10;
        int random = low+rand()%(high-low+1); 
        fprintf(fp,"%d",random);
    }
    else if(arr[begin] == 'c')
    {
        int random = 97+rand()%(122-97+1); 
        fprintf(fp,"%c",random+' '-32);
    }
    else if(arr[begin] == 's')
    {
        int low = 0;
        int high = 0;
        int i = begin + 7;
        for(; arr[i]!=','; i++)
            low = (arr[i] - '0') + low*10;

        for(int j = i+1;arr[j]!=')';j++)
            high = (arr[j] - '0') + high*10;

        int random_length = low+rand()%(high-low+1);
        for(int k = 1;k <= random_length;k++)
        {
            fprintf(fp,"%c",48+rand()%(122-48+1)+' '-32);
        }
    }
    fprintf(fp,"%c",32);
    fclose(fp);
}

void showAllFiles(const char *dir_name, std::vector<std::string>& filesAbsolutePath)
{
	struct dirent *filename = NULL;
	DIR* dir = opendir(dir_name);
	if (NULL == dir)
	{
		cout << "Can not open dir " << dir_name << endl;
		return;
	}
	cout << "Successfully opened the dir !" << endl;

	while ((filename = readdir(dir)) != NULL)
	{
		if (strcmp(filename->d_name, ".") == 0 ||
			strcmp(filename->d_name, "..") == 0)
			continue;

        char name[1000] = {0};
        strncpy(name, filename->d_name, strlen(filename->d_name)-4);
        if(strcmp(name, "stdin_format") != 0 && strcmp(name, "input") != 0)
		{
            std::string absolutePath = dir_name + std::string("/") + filename->d_name;
            filesAbsolutePath.push_back(absolutePath);
        }
	}
}

