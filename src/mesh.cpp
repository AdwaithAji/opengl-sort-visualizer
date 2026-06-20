#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, unsigned short indices[], unsigned int indicesCount) {
  int vertexCount = vertices.size();  
  elementCount = indicesCount;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, &vertices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indicesCount, indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

Mesh::~Mesh() {

}

void Mesh::draw() {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, 0);
} 
