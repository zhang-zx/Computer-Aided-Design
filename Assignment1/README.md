<center>  <h1> CAD Assignment 1</h1></center>


操作说明：

```C++
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
```

