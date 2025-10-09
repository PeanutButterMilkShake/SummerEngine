#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

string ReadFile(const string &filePath);
void ReadMeshFile(const string &filePath, vector<float> &vertices, vector<unsigned int> &indices);

#endif