#pragma once

struct VertexData
{
    unsigned int vbo;
    unsigned int vao;
    unsigned int triangleNum;

    VertexData(unsigned int dataSizeInBytes, float* data);

    ~VertexData();
};