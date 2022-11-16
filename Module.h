#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int Compare(char* outputfile_path1, char* outputfile_path2);

void SetResult(const char* file1_path, const char* file2_path, int result);

int CreateRandomInput(char* directory);

void HandleChar(char* arr, int begin, int end);

void showAllFiles(const char *dir_name, std::vector<std::string>& filesAbsolutePath);

#endif