#ifndef MESH_UTILITY_H
#define MESH_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "shader_utility.h"

struct Mesh
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int initialized = -1;
    bool use_indices = false;
};

// ALWAYS SET THE SHAPE BEFORE YOU INITIALIZE

static inline void Mesh_SetTriangle(Mesh& mesh)
{
    mesh.vertices = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    mesh.use_indices = false;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = 0;
}

static inline void Mesh_SetRectangle(Mesh& mesh)
{
    mesh.vertices =
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    mesh.indices =
    {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    mesh.use_indices = true;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = 0;
}

// TODO
static inline void Mesh_SetCircle(Mesh& mesh, float radius, int sectors)
{
    mesh.vertices.clear();
    mesh.indices.clear();

    // center vertex
    mesh.vertices.push_back(0.0f);
    mesh.vertices.push_back(0.0f);
    mesh.vertices.push_back(0.0f);

    // perimeter vertices
    for (int i = 0; i <= sectors; ++i)
    {
        float theta = (float)i / (float)sectors * 2.0f * consts::PI;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        float z = 0.0f;

        mesh.vertices.push_back(x);
        mesh.vertices.push_back(y);
        mesh.vertices.push_back(z);
    }

    // generate the indices for the triangle fan
    for (int i = 0; i <= sectors; ++i)
    {
        mesh.indices.push_back(0);      // center
        mesh.indices.push_back(i);      // current
        mesh.indices.push_back(i + 1);  // next
    }

    mesh.use_indices = true;
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
    mesh.initialized = 0;
}

static inline void Mesh_SetCube(Mesh& mesh)
{
    mesh.vertices =
    {
        // positions         
        -0.5f, -0.5f,  0.5f,  // 0
        0.5f, -0.5f,  0.5f,  // 1
        0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3
        -0.5f, -0.5f, -0.5f,  // 4
        0.5f, -0.5f, -0.5f,  // 5
        0.5f,  0.5f, -0.5f,  // 6
        -0.5f,  0.5f, -0.5f   // 7
    };

    mesh.indices =
    {
        // front
        0,1,2, 2,3,0,
        // right
        1,5,6, 6,2,1,
        // back
        5,4,7, 7,6,5,
        // left
        4,0,3, 3,7,4,
        // top
        3,2,6, 6,7,3,
        // bottom
        4,5,1, 1,0,4
    };

    mesh.use_indices = true;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = 0;
}

static inline void Mesh_SetSphere(Mesh& mesh, float radius, int stacks, int sectors)
{
    mesh.vertices.clear();
    mesh.vertices.clear();

    int stack_count = stacks + 1;
    int sector_count = sectors + 1;

    mesh.vertices.reserve(stack_count * sector_count * 3);

    // phi from 0 to pi (latitude)
    // theta from 0 to 2pi (longitude)

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = (float)i / (float)stacks * consts::PI;
        float cos_phi = cosf(phi);
        float sin_phi = sinf(phi);

        for (int j = 0; j < sectors; ++j)
        {
            float theta = (float)j / (float)sectors * 2.0f * consts::PI;
            float cos_theta = cosf(theta);
            float sin_theta = sinf(theta);

            float x = radius * cos_theta * sin_phi;
            float y = radius * cos_phi;
            float z = radius * sin_theta * sin_phi;

            mesh.vertices.push_back(x);
            mesh.vertices.push_back(y);
            mesh.vertices.push_back(z);
        }
    }

    // generate indices (two triangles per quad)
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            // indices of quad corners
            int a = i * (sectors + 1) + j;
            int b = a + sectors + 1;
            int c = b + 1;
            int d = a + 1;

            // triangle 1: a, b, d
            mesh.indices.push_back((unsigned int)a);
            mesh.indices.push_back((unsigned int)b);
            mesh.indices.push_back((unsigned int)d);

            // triangle 2: d, b, c
            mesh.indices.push_back((unsigned int)d);
            mesh.indices.push_back((unsigned int)b);
            mesh.indices.push_back((unsigned int)c);
        }
    }

    mesh.use_indices = true;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = 0;
}

static inline void Mesh_Generate(Mesh& mesh)
{
    if (mesh.initialized == -1)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);

    if (mesh.use_indices) glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    if (mesh.use_indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

static inline void Mesh_Draw(const Mesh& mesh)
{
    if (mesh.initialized == -1)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glBindVertexArray(mesh.VAO);

    if (mesh.use_indices)
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size() / 3 * sizeof(float));

    glBindVertexArray(0);
}

static inline void Mesh_DrawWireFrame(const Mesh& mesh)
{
    if (mesh.initialized == -1)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Mesh_Draw(mesh);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static inline void Mesh_Delete(Mesh& mesh)
{
    if (mesh.EBO) glDeleteBuffers(1, &mesh.EBO);
    if (mesh.VAO) glDeleteVertexArrays(1, &mesh.VAO);
    if (mesh.VBO) glDeleteBuffers(1, &mesh.VBO);
    
    mesh.vertices.clear();
    mesh.indices.clear();
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
}

#endif