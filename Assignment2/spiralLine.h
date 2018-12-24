#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "shader.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>


#define PI 3.14159265358979323846
#define N 999999


class SpiralLine{
public:
    GLfloat angleStep = static_cast<GLfloat>(PI / 8.0);
    GLfloat range;
    std::vector<double> lengthLog;
    GLfloat startAngle;
    GLint startAngleIndex;
    std::vector<glm::vec3> positions;
    glm::vec3 colorType[16];
    std::vector<int> colors;
    SpiralLine(GLfloat r = 2.0f, GLfloat InitA = static_cast<GLfloat>(-2 * PI),
               GLint StartAI = 0): range(r), startAngle(InitA), startAngleIndex(StartAI){
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i=0;i<16;i++){
            float x = random() % (N + 1) / (float)(N + 1);
            float y = random() % (N + 1) / (float)(N + 1);
            float z = random() % (N + 1) / (float)(N + 1);
            float sum = x + y + z;
            colorType[i].x = x / sum;
            colorType[i].y = y / sum;
            colorType[i].z = z / sum;
        }
    }
    void logUpd(GLfloat deltaTime){
        GLfloat temp = 0;
        startAngleIndex = - static_cast<GLint>(std::ceil(startAngle / angleStep));
        GLfloat angle = startAngle + startAngleIndex * angleStep;
        while (temp<range){
            temp = std::exp(angle) / 1000;
            lengthLog.push_back(temp);
            angle += angleStep;
        }
        startAngle += deltaTime * 0.2;
        if (startAngle > 4 * PI)
            startAngle -= 6 * PI;
        positions.resize(lengthLog.size());
        colors.resize(lengthLog.size());
    }
    void PositionUpdate(GLfloat deltaTime){
        glm::vec3 tempPos(0.0f, 0.0f, 0.0f);
        logUpd(deltaTime);
        for (int i=0;i<lengthLog.size();i++){
            positions[i].x = static_cast<float>(lengthLog[i] * std::cos(angleStep * (i + startAngleIndex)));
            positions[i].y = static_cast<float>(lengthLog[i] * std::sin(angleStep * (i + startAngleIndex)));
            positions[i].z = 0.0f;
            colors[i] = (i + startAngleIndex + 16) % 16;
        }
        lengthLog.clear();
    }
};

