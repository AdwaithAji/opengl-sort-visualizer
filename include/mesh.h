#include<vector>
#include<glad/glad.h>

struct Vertex{
  float x,y,z;
};

class Mesh{

  private:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO; 
  
  unsigned int elementCount;

  public:
    
    Mesh(std::vector<Vertex> vertices, unsigned short indices[], unsigned int indicesCount);

   ~Mesh();
    
    void draw();
};
