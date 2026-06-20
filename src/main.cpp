#include "window.h"
#include "mesh.h"
#include <vector>
#include <string>
#include "shader.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main() {

  Window window(800, 600);
  window.init();

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.getWindow(),true);
  ImGui_ImplOpenGL3_Init("#version 130");


  std::vector<Vertex> bars = {
    {  0.0f,   0.0f, 0.0f}, //top left
    {  0.035f,   0.0f, 0.0f}, //top right
    {  0.0f,  -0.90f, 0.0f}, //bottom left
    {  0.035f,  -0.90f, 0.0f}, //bottom right
  };
  unsigned short indices[] = {0,1,2, 2,3,1};  
  Shader shader("shaders/bar.vert", "shaders/bar.frag");
  Mesh bar(bars, indices, 6);
  glm::mat4 model = glm::mat4(1.0f); 

  std::vector<int> nums;
  int size; 

  glm::vec4 barColor(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec4 barHighlight(1.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 barSorted(0.0f, 1.0f, 0.0f, 1.0f);


  int currentJ = 0;
  int swappedNum = 0;
  bool sorted = false;

  
  int number = 0;
  char numStr[10] = "\0";
  float w_transparency = 0.0f;
  bool transparency = false;
  bool started = false;
  bool submited = false;
  bool validated = false;
  bool validateError = false;
  bool status = true;
  

  while(!window.shouldClose()) {

    window.setViewPort();
    window.clearScreen(0.0f, 0.0f, 0.0f, 1.0f);
    
    // ImGui 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Menu bar
    if(ImGui::BeginMainMenuBar()) {

      if(ImGui::BeginMenu("File")) {

        if(ImGui::MenuItem("Open")) {

        }

        ImGui::EndMenu(); 
      }

      if(ImGui::BeginMenu("Settings")) {
        
        if(ImGui::MenuItem("Transparency")) {
          transparency = true; 
        }

        if(ImGui::MenuItem("Reset")) {
          nums.clear();
        }

        ImGui::EndMenu(); 
      }

      ImGui::EndMainMenuBar();
    }

    // Main window
    ImGuiWindowFlags w_flags = 0;
    w_flags |= ImGuiWindowFlags_NoResize;   
    w_flags |= ImGuiWindowFlags_NoMove;   
    w_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(ImVec2(0,ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2((float)window.windowWidth,(float)window.windowHeight - ImGui::GetFrameHeight()));
    ImGui::SetNextWindowBgAlpha(w_transparency); 

    ImGui::Begin("Main Window", NULL, w_flags);

    ImGui::Text("Enter the numbers");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(75.0f);
    if(ImGui::InputText("##nums", numStr, 10)) {
        try {
          size_t numLen = 0;
          number = std::stoi(numStr, &numLen);

          if (numLen == std::string(numStr).length()) {
            validateError = false;
          } else {
            validateError = true;
          }
        }
        catch (const std::invalid_argument&) {
          validateError = true;
        }
        catch (const std::out_of_range&) {
          validateError = true;
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Enter")) { 
      submited = false;
      if(!validateError && number != -1) {
        nums.push_back(number);
        numStr[0] = '\0';    
        number = -1;
        validated = true;
      }
    } 

    if(ImGui::Button("Submit")) {
      if(!nums.empty() && nums.size() > 1) {
       submited = true;
      }
    }

    if(ImGui::Button("Start")) {
      if(submited && validated) {
        started = true;
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Stop")) {
      started = false;
    }

    if(status) { 
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f ,1.0f), "Status:");
      ImGui::SameLine();
      if(started){
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f ,1.0f), "Running..");
      }
      else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f ,1.0f), "Stopped..");
      }
    }

    //
    if(validateError) {
      submited = false;
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f ,1.0f), "Invalid Input");
    }

    ImGui::End();

    //settings window 
    if(transparency) {

      ImGui::SetNextWindowPos(ImVec2(window.windowWidth * 0.2f, window.windowHeight * 0.2f));
      ImGui::SetNextWindowSize(ImVec2(window.windowWidth * 0.6f, window.windowHeight * 0.6f));

      ImGui::Begin("Transparency Settings", NULL);
      ImGui::SeparatorText("Transparency"); 

      ImGui::Text("Window");
      ImGui::SameLine();
      ImGui::SliderFloat("##w_transparency", &w_transparency, 0.0f, 1.0f);
      
      if(ImGui::Button("Close")) {
        transparency = false;
      }

      ImGui::End();

    }
    
    //Draws the bar
    shader.use();
    size = nums.size();
    for(int i = 0; i < size; i++) { 
      /// 
      float xPos = -0.90f + (i*0.04f);
      float yPos = -0.90f;

      glm::mat4 model = glm::mat4(1.0f); 
      model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));

      float yScale = (nums[i] == 0)? -0.009f : nums[i] * -0.015f;

      model = glm::scale(model,  glm::vec3(1.0f, yScale, 0.0f));
      ///     
      shader.setUniformMat4("model", model);

      if(!sorted) {
        if(i == swappedNum) {
          shader.setUniformVec4("barColor", barHighlight);
        } else {
          shader.setUniformVec4("barColor", barColor);
        }
      }
      else { 
        shader.setUniformVec4("barColor", barSorted);
      }

      bar.draw();   
    }
 
    // sorting
    if(started) {

      swappedNum = currentJ;
      if (nums[currentJ] > nums[currentJ+1]) {
        int temp = nums[currentJ];
        nums[currentJ] = nums[currentJ+1];
        nums[currentJ+1] = temp;  
      }
      currentJ++;
      if(currentJ >= size - 1) currentJ = 0;
    }

    //Check sorted or not
    sorted = true;
    for(int i = 0; i < size - 1; i++) {
      if(nums[i] > nums[i+1]) {
        sorted = false;
        break;
      }

    }
    if(sorted) started = false;


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    window.swapBuffers();
    window.pollEvents();
  }


  return 0;
}


