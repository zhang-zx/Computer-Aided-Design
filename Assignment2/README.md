<center>  <h1> CAD Assignment 2 </h1></center>


**实现功能:**

1. 多个星星绕一个中心旋转
2. 星星颜色不同
3. 类似螺旋线的效果，星星不断向外圈扩散
4. 至少实现阿基米德螺线、对数螺线、费马螺线中一种的效果

```c++
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
```

产生随机色彩。

```c++
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
```

位置计算。