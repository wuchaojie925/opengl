//
//  Mesh.cpp
//  opengl_learn
//
//  Created by chaojie wu on 11/8/20.
//

#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh()
:mLoaded(false)
{
    
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string& filename)
{
    std::vector<unsigned int> vertexIndices, uvIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    
    if (filename.find(".obj") != std::string::npos) {
        std::ifstream fin(filename, std::ios::in);
        if (!fin) {
            std::cerr << "Cannot open "<< filename << "\n";
            return false;
        }
        
        std::cerr << "Loading OBJ file  "<< filename << " ...\n";
        
        std::string lineBuffer;
        while (std::getline(fin, lineBuffer))
        {
            if (lineBuffer.substr(0, 2) == "v ") {
                std::istringstream v(lineBuffer.substr(2));
                glm::vec3 vertex;
                v >> vertex.x >> vertex.y >> vertex.z;
                tempVertices.push_back(vertex);
            }
            else if (lineBuffer.substr(0, 2) == "vt")
            {
                std::istringstream vt(lineBuffer.substr(3));
                glm::vec2 uv;
                vt >> uv.s >> uv.t;
                tempUVs.push_back(uv);
            }
            else if(lineBuffer.substr(0, 2) == "f ")
            {
                int p1, p2, p3; // to store mesh index
                int t1, t2, t3; // to store texture index
                int n1, n2, n3;
                const char* face = lineBuffer.c_str();
                int match = sscanf(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
                                     &p1, &t1, &n1,
                                     &p2, &t2, &n2,
                                     &p3, &t3, &n3
                                     );
                if (match != 9 ) {
                    std::cerr << "Failed to parse OBJ file. match is "<<match<<"\n";
                }
                
                // Ignore normals for now
                
                vertexIndices.push_back(p1);
                vertexIndices.push_back(p2);
                vertexIndices.push_back(p3);
                
                uvIndices.push_back(t1);
                uvIndices.push_back(t2);
                uvIndices.push_back(t3);
            }
        }
        
        // close the file
        fin.close();
        
        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++)
        {
            // Get the attributes using indices
            glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
            glm::vec2 uv = tempUVs[uvIndices[i] - 1];
            
            Vertex meshVertex;
            meshVertex.position = vertex;
            meshVertex.texCoords = uv;
            
            mVertices.push_back(meshVertex);
        }
        
        // Create and init the buffers
        initBuffers();
        
        return (mLoaded = true);
    }
    
    // Should not get here
    return false;
}

void Mesh::draw()
{
    if (!mLoaded) return;
    
    
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

void Mesh::initBuffers()
{
    // position buffer
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
    
    // color buffer
    //glGenBuffers(1, &vbo2);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vert_color), vert_color, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), NULL);
    glEnableVertexAttribArray(0);
    
    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    // color
    //glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), NULL);
    //glEnableVertexAttribArray(1);
    
    // indices
    //glGenBuffers(1, &ibo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
}

