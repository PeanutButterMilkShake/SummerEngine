#include "FileReader.h"
#include <algorithm>
#include <filesystem>

std::string ReadFile(const std::string& filePath)
{
    filesystem::path p(filePath);

    // If the path is relative, we can verify it exists relative to the executable
    if (!filesystem::exists(p))
    {
        std::cerr << "--- FILE ERROR ---" << std::endl;
        std::cerr << "Requested: " << p << std::endl;
        std::cerr << "Current Directory: " << filesystem::current_path() << std::endl;
        std::cerr << "Does file exist here? " << (filesystem::exists(p) ? "Yes" : "No") << std::endl;
        return "";
    }

    std::ifstream file(p);
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

void ReadMeshFile(const string &filePath, vector<float> &vertices, vector<unsigned int> &indices, vector<float> &vertNormals, vector<float> &vertUVs)
{
    vertices.clear();
    indices.clear();
    vertNormals.clear();
    vertUVs.clear();

    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Failed to open OBJ file: " << filePath << endl;
        return;
    }

    vector<glm::vec3> tempPositions;
    vector<glm::vec3> tempNormals;
    vector<glm::vec2> tempUVs;

    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vn")
        {
            glm::vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            tempNormals.push_back(norm);
        }
        else if (prefix == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            tempUVs.push_back(uv);
        }
        else if (prefix == "f")
        {
            for (int i = 0; i < 3; i++)
            {
                string vert;
                ss >> vert;

                // Set flags to check what sscanf successfully parsed
                int vIndex = -1, tIndex = -1, nIndex = -1;

                // Handle the 3 main variations of OBJ face strings
                if (vert.find("//") != string::npos) 
                {
                    // Format: v//vn
                    sscanf(vert.c_str(), "%d//%d", &vIndex, &nIndex);
                } 
                else if (vert.find('/') != string::npos && vert.find_first_of('/') == vert.find_last_of('/')) 
                {
                    // Format: v/vt (There is only one single slash in the string)
                    sscanf(vert.c_str(), "%d/%d", &vIndex, &tIndex);
                } 
                else 
                {
                    // Format: v/vt/vn
                    sscanf(vert.c_str(), "%d/%d/%d", &vIndex, &tIndex, &nIndex);
                }

                // Convert 1-based OBJ indices to 0-based C++ indices safely
                vIndex--; 
                tIndex--; 
                nIndex--;

                // --- Safe Extractions with Fallbacks ---
                glm::vec3 pos(0.0f);
                if (vIndex >= 0 && static_cast<size_t>(vIndex) < tempPositions.size()) {
                    pos = tempPositions[vIndex];
                }

                glm::vec3 norm(0.0f, 1.0f, 0.0f); // Up vector fallback for lighting
                if (nIndex >= 0 && static_cast<size_t>(nIndex) < tempNormals.size()) {
                    norm = tempNormals[nIndex];
                }

                glm::vec2 uv(0.0f);
                if (tIndex >= 0 && static_cast<size_t>(tIndex) < tempUVs.size()) {
                    uv = tempUVs[tIndex];
                }

                // Insert into flat vectors
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);

                vertNormals.push_back(norm.x);
                vertNormals.push_back(norm.y);
                vertNormals.push_back(norm.z);

                vertUVs.push_back(uv.x);
                vertUVs.push_back(uv.y);

                indices.push_back(indices.size());
            }
        }
    }
    
    file.close();
}