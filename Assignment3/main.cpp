#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "shader.h"
#include "snow.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint WIDTH = 800, HEIGHT = 600;
Snowing falling;
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snowing", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  Shader ourShader("main.vert.glsl", "main.frag.glsl");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLfloat vertices[] = {
      0.04f,  0.04f, 0.0f,   1.0f, 1.0f, // Top Right
      0.04f, -0.04f, 0.0f,   1.0f, 0.0f, // Bottom Right
     -0.04f, -0.04f, 0.0f,   0.0f, 0.0f, // Bottom Left
     -0.04f,  0.04f, 0.0f,   0.0f, 1.0f  // Top Left
  };
  GLuint indices[] = {  // Note that we start from 0!
      0, 1, 3, // First Triangle
      1, 2, 3  // Second Triangle
  };
  GLuint snowVBO, snowVAO, snowEBO;
  glGenVertexArrays(1, &snowVAO);
  glGenBuffers(1, &snowVBO);
  glGenBuffers(1, &snowEBO);
  glBindVertexArray(snowVAO);
  glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0); // Unbind snowVAO
  GLuint snowTexture;
  glGenTextures(1, &snowTexture);
  glBindTexture(GL_TEXTURE_2D, snowTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height;
  unsigned char* image = SOIL_load_image("snow.png", &width, &height, 0, SOIL_LOAD_RGBA);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
  GLfloat lakeVertices[] = {
       1.0f,  1.0f,  0.0f,        1.0f, 1.0f, // Top Right
       1.0f, -1.0f,  0.0f,        1.0f, 0.0f, // Bottom Right
      -1.0f, -1.0f,  0.0f,        0.0f, 0.0f, // Bottom Left
      -1.0f,  1.0f,  0.0f,        0.0f, 1.0f  // Top Left
  };
  GLuint lakeVBO, lakeVAO, lakeEBO;
  glGenVertexArrays(1, &lakeVAO);
  glGenBuffers(1, &lakeVBO);
  glGenBuffers(1, &lakeEBO);
  glBindVertexArray(lakeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lakeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lakeVertices), lakeVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lakeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0); // unbind lakeVAO
  GLuint lakeTexture;
  glGenTextures(1, &lakeTexture);
  glBindTexture(GL_TEXTURE_2D, lakeTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width2, height2;
  unsigned char* imagelake = SOIL_load_image("darklake.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imagelake);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(imagelake);
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done
  while (!glfwWindowShouldClose(window)) {
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, -0.1f, 100.0f);
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glm::mat4 model(1);
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSetWindowPos(window, 100, 100);
    ourShader.Use();
    glBindVertexArray(lakeVAO);
    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, lakeTexture);
    model = glm::mat4(1);
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(snowVAO);
    glBindTexture(GL_TEXTURE_2D, snowTexture);
    for (int i=0;i<falling.getFlakeSize();i++){
      model = glm::mat4(1);
      model = glm::translate(model, falling.Positions[i]);
      GLfloat tempSize = falling.Scales[i];
      model = glm::scale(model, glm::vec3(tempSize, tempSize, tempSize));
      glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    falling.positionUpdate(deltaTime);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &snowVAO);
  glDeleteBuffers(1, &snowVBO);
  glDeleteBuffers(1, &snowEBO);
  glDeleteVertexArrays(1, &lakeVAO);
  glDeleteBuffers(1, &lakeVBO);
  glDeleteBuffers(1, &lakeEBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
