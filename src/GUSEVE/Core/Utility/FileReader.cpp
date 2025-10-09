#include "FileReader.h"

string ReadFile(const string& filePath)
{
    ifstream file(filePath);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
  
    return contents.str();
}

void ReadMeshFile(const string &filePath, vector<float> &vertices, vector<unsigned int> &indices)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Failed to open OBJ file: " << filePath << endl;
        return;
    }

    vertices.clear();
    indices.clear();

    vector<float> tempVertices;
    string line;

    while (getline(file, line))
    {
        istringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            tempVertices.push_back(x);
            tempVertices.push_back(y);
            tempVertices.push_back(z);
        }
        else if (prefix == "f")
        {
            unsigned int a, b, c;
            string v1, v2, v3;

            ss >> v1 >> v2 >> v3;

            a = stoi(v1.substr(0, v1.find('/'))) - 1;
            b = stoi(v2.substr(0, v2.find('/'))) - 1;
            c = stoi(v3.substr(0, v3.find('/'))) - 1;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
        }
    }

    vertices = tempVertices;
    file.close();
}