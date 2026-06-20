#include<glad/glad.h>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<fstream>
#include<sstream>
#include<iostream>

enum Info{SHADER,PROGRAM};
class Shader{
  
  private:
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;
    unsigned int programID;

    bool init(std::string vertexCode, std::string fragmentCode);
    bool checkStatus(unsigned int ID, Info info);

  public:
    Shader(std::string vertexPath, std::string fragmentPath);
    
   ~Shader();
    
    void setUniformMat4(const char* name, glm::mat4 &matrix);
    void setUniformVec4(const char* name, glm::vec4 color);
    void use();

};
