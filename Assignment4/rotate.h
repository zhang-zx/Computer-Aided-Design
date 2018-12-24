#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Rotation{
public:
  std::vector<glm::vec3> Positions;
  std::vector<GLfloat> Radius;
  GLfloat orbitRotateSpeed[10];
  GLfloat orbitAngle[10];

  Rotation(){
    Positions.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));  // Sun
    Positions.emplace_back(glm::vec3(0.2f, 0.0f, 0.0f));  // Mercury
    Positions.emplace_back(glm::vec3(0.4f, 0.0f, 0.0f));  // Venus
    Positions.emplace_back(glm::vec3(0.6f, 0.0f, 0.0f));  // Earth
    Positions.emplace_back(glm::vec3(0.04f, 0.0f, 0.0f));  // Moon
    Positions.emplace_back(glm::vec3(0.8f, 0.0f, 0.0f));  // Mars
    Positions.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));  // Jupiter
    Positions.emplace_back(glm::vec3(1.2f, 0.0f, 0.0f));  // Saturn
    Positions.emplace_back(glm::vec3(1.4f, 0.0f, 0.0f));  // Uranus
    Positions.emplace_back(glm::vec3(1.6f, 0.0f, 0.0f));  // Neptune
    Radius.emplace_back(0.2f);
    Radius.emplace_back(0.01f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.025f);
    Radius.emplace_back(0.005f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.04f);
    Radius.emplace_back(0.03f);
    Radius.emplace_back(0.02f);
    Radius.emplace_back(0.01f);
    orbitRotateSpeed[0] = 0.0f;
    orbitRotateSpeed[1] = 4.0f;
    orbitRotateSpeed[2] = 3.5f;
    orbitRotateSpeed[3] = 3.0f;
    orbitRotateSpeed[4] = 20.0f;
    orbitRotateSpeed[5] = 4.0f;
    orbitRotateSpeed[6] = 1.0f;
    orbitRotateSpeed[7] = 2.0f;
    orbitRotateSpeed[8] = 1.5f;
    orbitRotateSpeed[9] = 2.3f;

    for (int i=0; i<10; i++){
      orbitAngle[i] = 0.0f;
    }
  }

  void PositionUpdate(GLfloat deltaTime){
    for (int i=0; i<10; i++){
      orbitAngle[i] += orbitRotateSpeed[i] * deltaTime;
      if (orbitAngle[i] > 360.0f)
        orbitAngle[i] -= 360.0f;
    }
  }
};

