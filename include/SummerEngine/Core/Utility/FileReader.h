#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

string ReadFile(const string &filePath);
void ReadMeshFile(const string &filePath, vector<float> &vertices, vector<unsigned int> &indices, vector<float> &vertNormals, vector<float> &vertUVs);

#endif