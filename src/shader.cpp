#include "shader.h"

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
  
  std::string vertexCode;
  std::string fragmentCode;

  //reads file from the path 
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.open(vertexPath);
  fShaderFile.open(fragmentPath);

  std::stringstream vStream, fStream;

  vStream << vShaderFile.rdbuf();
  fStream << fShaderFile.rdbuf();
  vShaderFile.close();
  fShaderFile.close();

  vertexCode = vStream.str();
  fragmentCode = fStream.str();
  
  if(!init(vertexCode, fragmentCode)) {
    std::cout<<"\nError Compiling Shaders";
  } 
  
}

Shader::~Shader() {
  glDeleteProgram(programID);
}

bool Shader::init(std::string vertexCode, std::string fragmentCode) {

  vertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
  const char* vSource = vertexCode.c_str();
  glShaderSource(vertexShaderID, 1, &vSource, nullptr);
  glCompileShader(vertexShaderID);

  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fSource = fragmentCode.c_str();
  glShaderSource(fragmentShaderID, 1, &fSource, nullptr);
  glCompileShader(fragmentShaderID); 
  
  if(!checkStatus(vertexShaderID, SHADER) || !checkStatus(fragmentShaderID, SHADER)) {
    return false;
  }

  programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);

  if(!checkStatus(programID, PROGRAM)){
      return false;
  } 

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  return true;
}

void Shader::setUniformMat4(const char* name, glm::mat4 &matrix) {
  unsigned int Loc = glGetUniformLocation(programID, name);
  glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec4(const char* name, glm::vec4 color) {
  unsigned int Loc = glGetUniformLocation(programID, name);
  glUniform4fv(Loc, 1, glm::value_ptr(color));
}


void Shader::use() {

  glUseProgram(programID);

}

bool Shader::checkStatus(unsigned int id, Info info) {
    int status;
    int errorMsgLength;

    switch(info) {
        case SHADER: { 
            glGetShaderiv(id, GL_COMPILE_STATUS, &status);
            if (!status) { 
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorMsgLength);
                char* errorMsg = new char[errorMsgLength];
                glGetShaderInfoLog(id, errorMsgLength, nullptr, errorMsg);
                std::cout << "SHADER COMPILATION ERROR:\n" << errorMsg << '\n';
                delete[] errorMsg;
                return false;
            }
        } 
        break;
        
        case PROGRAM: { 
            glGetProgramiv(id, GL_LINK_STATUS, &status); 
            if (!status) { 
                glGetProgramiv(id, GL_INFO_LOG_LENGTH, &errorMsgLength);
                char* errorMsg = new char[errorMsgLength];
                glGetProgramInfoLog(id, errorMsgLength, nullptr, errorMsg);
                std::cout << "SHADER PROGRAM LINKING ERROR:\n" << errorMsg << '\n';
                delete[] errorMsg;
                return false;
            }
        } 
        break;
    } 
    return true;
}


 
