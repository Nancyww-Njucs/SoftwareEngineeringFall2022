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

#define Max 10

using namespace std;

#include "Module.h"

//----------------------------------

int ExecuteFiles(const std::string& directory) 
{
    DIR* dir = opendir(directory.c_str()); 
    if ( dir == NULL )
    {
        return -1;
    }

    struct dirent* d_ent = NULL;      
    char fullpath[128] = {0};
    char dot[3] = ".";               
    char dotdot[6] = "..";

    //-----------------------------------分别遍历input目录下的dir文件夹-----------------------------------
    while ( (d_ent = readdir(dir)) != NULL )   
    {
        if ( (strcmp(d_ent->d_name, dot) != 0)
              && (strcmp(d_ent->d_name, dotdot) != 0) )  
        {
            if ( d_ent->d_type == DT_DIR && strcmp(d_ent->d_name, "output") != 0 ) 
            {
                std::vector<std::string> vstrFile;
                
                std::string newDirectory = directory + std::string("/") + std::string(d_ent->d_name);
                std::cout << newDirectory << std::endl;

                //---------------showAllFile()将dir里的所有cpp的绝对路径记录在vector结构体中-----------------------              
                showAllFiles(newDirectory.c_str(), vstrFile);

                char path_file[100] = {0};
                int m = 0;
                for(int j = vstrFile[0].size()-1;;j--)
                    if (vstrFile[0].c_str()[j] == '/'){
                        m = j;break;
                    }
                
                strncpy(path_file, vstrFile[0].c_str(), m);

                for (size_t i = 0; i < vstrFile.size()-1; i++)
                    for (size_t j = i+1; j < vstrFile.size(); j++)
                    {
                        //(i,j)compare
                        std::string file1 = vstrFile[i];
                        std::string file2 = vstrFile[j];

                        //-----------------------system("g++ a.cpp -o a.out")生成可执行文件---------------------------------

                        std::string createout1_cmd = std::string("g++ ") + file1 + std::string(" -o ") 
                            + std::string("outputfile1.out");
                        system(createout1_cmd.c_str());
                        cout<<"ok 1"<<endl;

                            
                        std::string createout2_cmd = std::string("g++ ") + file2 + std::string(" -o ") 
                            + std::string("outputfile2.out");
                        system(createout2_cmd.c_str());
                        cout<<"ok 2"<<endl;

                        //------------------------------------------------------------------------------------------------
                        

                        int eq = 0;
                        for(int k = 0;k < Max;k++)
                        {
                            //-------------------------CreateRandomInput()生成随机测试用例-----------------------------------
                            std::string createinput = std::string("touch input.txt");
                            system(createinput.c_str());

                            CreateRandomInput(path_file);
                            //--------------------------------------------------------------------------------------------
                            

                            //------------systeam("a.out <input.txt> ouput.txt")将输出结果记录在txt文件当中-------------------
                            std::string execute1_cmd = std::string("./outputfile1.out <input.txt> output1.txt");
                            system(execute1_cmd.c_str());

                            std::string execute2_cmd = std::string("./outputfile2.out <input.txt> output2.txt");
                            system(execute2_cmd.c_str());

                            std::string cleaninput = std::string("rm input.txt");
                            system(cleaninput.c_str());
                            //--------------------------------------------------------------------------------------------
                            

                            //---------------------比较两个.txt文件的相似性,记录在equal.csv/inequal.csv中----------------------
                            
                            int result = Compare("output1.txt", "output2.txt");
                            if(result == 0)
                            {
                                cout<<"inequal"<<endl;
                                //print the result 
                                SetResult(file1.c_str(), file2.c_str(), 0);
                                break;
                            }
                            else
                            {
                                eq++;
                            }                                                       
                        }
                        //-------------------------若每一组的执行结果都相同，则认为oj1和oj2等价，否则，不等价----------------------
                        if(eq == 10)
                        {
                            cout<<"equal"<<endl;
                            //Put the compare result into equal.csv && inequal.csv
                            SetResult(file1.c_str(), file2.c_str(), 1);
                        }    
                        //------------------------------------------------------------------------------------------------
                    }
            }
        }
    }
    closedir(dir);
    return 0;
}

int main()
{
    system("mkdir output");
    system("touch ./output/equal.csv");
    system("touch ./output/inequal.csv");
	ExecuteFiles("./input");
	return 0;
}
