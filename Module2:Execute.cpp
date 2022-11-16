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

    while ( (d_ent = readdir(dir)) != NULL )   
    {
        if ( (strcmp(d_ent->d_name, dot) != 0)
              && (strcmp(d_ent->d_name, dotdot) != 0) )  
        {
            if ( d_ent->d_type == DT_DIR ) 
            {
                std::vector<std::string> vstrFile;
                
                std::string newDirectory = directory + std::string("/") + std::string(d_ent->d_name);
                std::cout << newDirectory << std::endl;
                
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
                        
                        //printf("%s\n",file1.c_str());
                        //printf("%s\n",file2.c_str());

                        std::string createout1_cmd = std::string("g++ ") + file1 + std::string(" -o ") 
                            + std::string("outputfile1.out");
                        system(createout1_cmd.c_str());
                        cout<<"ok 1"<<endl;

                            
                        std::string createout2_cmd = std::string("g++ ") + file2 + std::string(" -o ") 
                            + std::string("outputfile2.out");
                        system(createout2_cmd.c_str());
                        cout<<"ok 2"<<endl;
                        
                        int eq = 0;
                        for(int k = 0;k < Max;k++)
                        {
                            std::string createinput = std::string("touch input.txt");
                            system(createinput.c_str());

                            CreateRandomInput(path_file);
                            //cout<<endl;
                            
                            //system(“x.exe <x.txt> x.out");
                            std::string execute1_cmd = std::string("./outputfile1.out <input.txt> output1.txt");
                            system(execute1_cmd.c_str());

                            std::string execute2_cmd = std::string("./outputfile2.out <input.txt> output2.txt");
                            system(execute2_cmd.c_str());

                            std::string cleaninput = std::string("rm input.txt");
                            system(cleaninput.c_str());

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
                        if(eq == 10)
                        {
                            cout<<"equal"<<endl;
                            //Put the compare result into equal.csv && inequal.csv
                            SetResult(file1.c_str(), file2.c_str(), 1);
                        }    
                    }
            }
        }
    }
    closedir(dir);
    return 0;
}

int main()
{
    //std::vector<std::string> vstrFile;
	ExecuteFiles("./input");
    /*
    for (size_t i = 0; i < vstrFile.size(); i++)
    {
        std::cout<<vstrFile[i].c_str()<<std::endl;
    }
    
    */
	return 0;
}
