#include <iostream>
#include <cmath>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
bool loadOBJ(
    const char * path,
    std::vector<glm::vec3> &out_vertices,
    std::vector<glm::vec2> &out_uvs,
    std::vector<glm::vec3> &out_normals
);
void normalCal(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> &normals);


GLuint WIDTH = 800, HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 DisplayColor(0.2f,0.8f,0.6f);
GLfloat colorStep = 0.02f;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glEnable(GL_DEPTH_TEST);
  Shader ourShader("main.vert.glsl", "main.frag.glsl");
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  bool res = loadOBJ("eight.uniform.obj", vertices, uvs, normals);
  if (!res){
    std::cout<<"Failed to load obj file"<<std::endl;
    return -1;
  }
  normalCal(vertices, normals);
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
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  glm::vec3 lightPos(1.0f, 1.0f, -0.2f);

  while (!glfwWindowShouldClose(window)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();
    Do_Movement();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSetWindowPos(window, 300, 100);

    ourShader.Use();


    glm::mat4 model(1);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
    GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
    GLint lightColorLoc  = glGetUniformLocation(ourShader.Program, "lightColor");
    GLint lightPosLoc    = glGetUniformLocation(ourShader.Program, "lightPos");
    GLint viewPosLoc     = glGetUniformLocation(ourShader.Program, "viewPos");

    glBindVertexArray(VAO);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(colorLoc, DisplayColor.x, DisplayColor.y, DisplayColor.z);
    glUniform3f(lightColorLoc,  0.8f, 0.8f, 0.8f);
    glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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

void Do_Movement() {
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  else if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  else if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  else if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
  else if (keys[GLFW_KEY_U])
    camera.ProcessKeyboard(UP, deltaTime);
  else if (keys[GLFW_KEY_N])
    camera.ProcessKeyboard(DOWN, deltaTime);
  else if (keys[GLFW_KEY_R])
    DisplayColor.x += (DisplayColor.x+colorStep > 1.0f)? colorStep-1.0f : colorStep;
  else if (keys[GLFW_KEY_G])
    DisplayColor.y += (DisplayColor.y+colorStep > 1.0f)? colorStep-1.0f : colorStep;
  else if (keys[GLFW_KEY_B])
    DisplayColor.z += (DisplayColor.z+colorStep > 1.0f)? colorStep-1.0f : colorStep;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}
bool loadOBJ(
    const char* path,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
) {
  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;
  FILE * file = std::fopen(path, "r");
  if( file == nullptr){
    printf("Impossible to open the file !\n");
    return false;
  }
  while(true) {
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
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        return false;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
    }
  }

  for (unsigned int vertexIndex : vertexIndices) {
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

void normalCal(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> &normals){
  for (int i=0;i<vertices.size();i += 3){
    glm::vec3 e1 = vertices[i+2] - vertices[i];
    glm::vec3 e2 = vertices[i+2] - vertices[i+1];

    glm::vec3 normal = glm::normalize(glm::cross(e1,e2));
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
  }
}
