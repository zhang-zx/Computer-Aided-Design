#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include "glad.c"
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "mesh.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
);

GLuint WIDTH = 800, HEIGHT = 600;

MeshControl MeshControl;

int displayMode=4;

bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
  srand((unsigned)time(nullptr));
  glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glEnable(GL_DEPTH_TEST);
  Shader ourShader("main.vert.glsl", "main.frag.glsl");
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec2 > uvs;
  std::vector< glm::vec3 > normals;
  bool res = loadOBJ("eight.uniform.obj", vertices, uvs, normals);
  if (!res){
    std::cout<<"Failed to load obj file"<<std::endl;
    return -1;
  }
  glm::vec3 tempColor;
  std::vector< glm::vec3 > vertexColors;
  for (int i=0;i<vertices.size();i=i+3){
    tempColor.x = (GLfloat)random()/RAND_MAX;
    tempColor.y = (GLfloat)random()/RAND_MAX;
    tempColor.z = (GLfloat)random()/RAND_MAX;
    for (int j=0;j<3;j++) {
      vertexColors.push_back(tempColor);
    }
  }
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexColors.size() * sizeof(glm::vec3), &vertexColors[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glBindVertexArray(0);
  while (!glfwWindowShouldClose(window))
  {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();
    do_movement();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ourShader.Use();
    glm::mat4 model(1);
    model = glm::rotate(model, glm::radians(MeshControl.Roll), glm::vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model, glm::radians(MeshControl.Pitch), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(MeshControl.Yaw), glm::vec3(0.0f,0.0f,1.0f));
    model = glm::translate(model, MeshControl.Position);
    glm::mat4 view(1);
    view = glm::lookAt(glm::vec3(0.0f,0.0f,-10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 projection(1);
    projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, -2.0f, 2.0f);
    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
    GLint displayModeLoc = glGetUniformLocation(ourShader.Program, "displayMode");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(displayModeLoc, displayMode);
    if (displayMode == 1 || displayMode == 2){
      glBindVertexArray(VAO);
      glDisableVertexAttribArray(1);
      GLint triangleColorLocation = glGetUniformLocation(ourShader.Program, "myColor");
      glUniform3f(triangleColorLocation, MeshControl.DisplayColor.x,MeshControl.DisplayColor.y,MeshControl.DisplayColor.z);
      glBindVertexArray(0);
    } else {
      glBindVertexArray(VAO);
      glEnableVertexAttribArray(1);
      glBindVertexArray(0);
    }
    switch (displayMode){
      case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
      case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
      case 3:
      case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
      default:
        break;
    }
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    if (displayMode == 3){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      keys[key] = true;
    else if (action == GLFW_RELEASE)
      keys[key] = false;
  }
}

void do_movement()
{
  if (keys[GLFW_KEY_W])
    MeshControl.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_S])
    MeshControl.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    MeshControl.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D])
    MeshControl.ProcessKeyboard(RIGHT, deltaTime);
  if (keys[GLFW_KEY_T])
    MeshControl.ProcessKeyboard(LIFT, deltaTime);
  if (keys[GLFW_KEY_N])
    MeshControl.ProcessKeyboard(SINK, deltaTime);
  if (keys[GLFW_KEY_LEFT])
    MeshControl.ProcessKeyboard(UPWARD, deltaTime);
  if (keys[GLFW_KEY_RIGHT])
    MeshControl.ProcessKeyboard(DOWNWARD, deltaTime);
  if (keys[GLFW_KEY_UP])
    MeshControl.ProcessKeyboard(CLOCKWISE, deltaTime);
  if (keys[GLFW_KEY_DOWN])
    MeshControl.ProcessKeyboard(ANTICLOCKWISE, deltaTime);
  if (keys[GLFW_KEY_J])
    MeshControl.ProcessKeyboard(LEFTWARD, deltaTime);
  if (keys[GLFW_KEY_K])
    MeshControl.ProcessKeyboard(RIGHTWARD, deltaTime);
  if (keys[GLFW_KEY_1])
    displayMode = 1;
  if (keys[GLFW_KEY_2])
    displayMode = 2;
  if (keys[GLFW_KEY_3])
    displayMode = 3;
  if (keys[GLFW_KEY_4])
    displayMode = 4;
  if (keys[GLFW_KEY_R] && displayMode<3)
    MeshControl.ProcessKeyboard(R_CHANGE, deltaTime);
  if (keys[GLFW_KEY_G] && displayMode<3)
    MeshControl.ProcessKeyboard(G_CHANGE, deltaTime);
  if (keys[GLFW_KEY_B] && displayMode<3)
    MeshControl.ProcessKeyboard(B_CHANGE, deltaTime);
}

bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
) {
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;
  FILE * file = std::fopen(path, "r");
  if( file == NULL ){
    printf("cannot open the file !\n");
    return false;
  }
  while( 1 ) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break;
    if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3];
      int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
      if (matches != 3) {
        printf("No Fucking way\n");
        return false;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
    }
  }
  for (unsigned int i=0; i<vertexIndices.size(); i++ ){
    unsigned int vertexIndex = vertexIndices[i];
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    out_vertices.push_back(vertex);
  }

  for (unsigned int i=0; i<uvIndices.size(); i++){
    unsigned int uvIndex = uvIndices[i];
    glm::vec2 uv = temp_uvs[uvIndex-1];
    out_uvs.push_back(uv);
  }

  for (unsigned int i=0; i<normalIndices.size(); i++){
    unsigned int normalIndex = vertexIndices[i];
    glm::vec3 normal = temp_normals[normalIndex-1];
    out_normals.push_back(normal);
  }

  return true;
}
