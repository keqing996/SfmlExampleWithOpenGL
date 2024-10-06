#include "VertexData.h"
#include <glad/gl.h>

VertexData::VertexData(unsigned int dataSizeInBytes, float* data)
{
    /* [VBO] Create vertex buffer */
    ::glGenBuffers(1, &vbo);
    ::glBindBuffer(GL_ARRAY_BUFFER, vbo);
    ::glBufferData(GL_ARRAY_BUFFER, dataSizeInBytes, data, GL_STATIC_DRAW);

    /* [VAO] Generate vertex array for plane and bind it */
    ::glGenVertexArrays(1, &vao);
    ::glBindVertexArray(vao);

    /* Layout */
    ::glEnableVertexAttribArray(0);
    ::glEnableVertexAttribArray(1);
    ::glEnableVertexAttribArray(2);
    ::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    ::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    ::glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    triangleNum = dataSizeInBytes / 8;
}

VertexData::~VertexData()
{
    ::glDeleteBuffers(1, &vbo);
    ::glDeleteVertexArrays(1, &vao);
}
