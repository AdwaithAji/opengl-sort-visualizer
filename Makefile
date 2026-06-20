# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Iinclude -Iinclude/imgui_headers

# Linker Flags
LDFLAGS = -lglfw -lGL

# Source Files
SRC = src/glad.c src/window.cpp src/mesh.cpp src/shader.cpp src/main.cpp \
      src/imgui_src/imgui.cpp \
      src/imgui_src/imgui_demo.cpp \
      src/imgui_src/imgui_draw.cpp \
      src/imgui_src/imgui_impl_glfw.cpp \
      src/imgui_src/imgui_impl_opengl3.cpp \
      src/imgui_src/imgui_tables.cpp \
      src/imgui_src/imgui_widgets.cpp

OBJ_DIR = build
# This line shifts all .o files down into a dedicated build/obj/ subdirectory
SUB_OBJ_DIR = $(OBJ_DIR)/obj

# Convert source paths to target object paths inside build/obj/
OBJS = $(patsubst src/%,$(SUB_OBJ_DIR)/%,$(SRC:.cpp=.o))
OBJS := $(OBJS:.c=.o)

TARGET = $(OBJ_DIR)/SortViz

# Default target
all: $(TARGET)

# Rule to link object files and copy assets
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Copying shaders to build directory..."
	@mkdir -p $(OBJ_DIR)/shaders
	@cp -r shaders/* $(OBJ_DIR)/shaders/ 2>/dev/null || true

# Rule to compile C++ source files into build/obj/
$(SUB_OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C source files into build/obj/
$(SUB_OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile and run automatically
run: all
	./$(TARGET)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR)
