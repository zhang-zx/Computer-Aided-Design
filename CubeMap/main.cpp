#include <GL/glew.h>
#include <GLUT/glut.h>
#include <SOIL.h>
#include <OpenGL/gl.h>
#include <iostream>
#include <ctime>
#include "model.h"
#include <math.h>
int curWin;
Model model;
bool fullscreen;
clock_t lastTickCount;
#define pi 3.1415
double ratio = 1.0;
GLfloat x_pos=0.28;
GLfloat y_pos=0.73;
GLfloat z_pos=1.3;
GLfloat seita=-2.6;
GLfloat fai=1.15;//视角方向
GLuint sky_texture[5];
GLuint sea_texture;
GLuint heightmap;
GLuint terrain;
GLuint detail;
int oldmy=0,oldmx=0;
int mytime=0;
GLfloat lightAmbient[] = {0.5, 0.5, 0.5, 1.0};
GLfloat lightDiffuse[] = {0.7, 0.7, 0.7, 1.0};
GLfloat lightPos[] = {0.0, 0.0, -4.0, 1.0};
GLenum polyMode = GL_FILL;
bool LoadTexture()
{
    sky_texture[0] = SOIL_load_OGL_texture("Data/SkyBox0.bmp",
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    sky_texture[1] = SOIL_load_OGL_texture("Data/SkyBox1.bmp",
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    sky_texture[2] = SOIL_load_OGL_texture("Data/SkyBox2.bmp",
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    sky_texture[3] = SOIL_load_OGL_texture("Data/SkyBox3.bmp",
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    sky_texture[4] = SOIL_load_OGL_texture("Data/SkyBox4.bmp",
                                           SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    sea_texture = SOIL_load_OGL_texture("Data/SkyBox5.bmp",
                                        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    heightmap = SOIL_load_OGL_texture("Data/heightmap.bmp",
                                      SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    terrain = SOIL_load_OGL_texture("Data/terrain-texture3.bmp",
                                    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    detail = SOIL_load_OGL_texture("Data/detail.bmp",
                                   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y| SOIL_FLAG_MIPMAPS);
    glBindTexture(GL_TEXTURE_2D, terrain);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return true;
}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

Model::Model()
{
    vertices.clear();
    faces.clear();
}

Model::~Model()
{

}

bool Model::LoadHeightMap(std::string fileName, int xscale, int yscale, int zscale)
{
    unsigned char* heightData;
    int width, height, channels;

    heightData = SOIL_load_image(fileName.c_str(), &width, &height, &channels, SOIL_LOAD_L);
    channels = 1;

    Vertex curVertex;
    std::stringstream convStream;

    TexCoord curTexCoord;
    double stepi = 1.0 / (double)(height - 1);
    double stepj = 1.0 / (double)(width - 1);

    Vertex corner0, corner1;
    corner0.x = -1e30; corner0.y = -1e30; corner0.z = -1e30;
    corner1.x = 1e30; corner1.y = 1e30; corner1.z = 1e30;

    // Construct 3D mesh from image -> vertices
    // Set height as y coordinate
    for (int i = 0;i < height;++i)
    {
        convStream.clear();
        convStream << (i-height/2)*zscale;
        double curZ;
        convStream >> curZ;
        for (int j = 0;j < width;++j)
        {
            convStream.clear();
            convStream << (j-width/2)*xscale;
            double curX;
            convStream >> curX;

            convStream.clear();
            double curY = heightData[channels*(i*width+j)];

            curVertex.x = curX;
            curVertex.y = curY;
            curVertex.z = curZ;

            curTexCoord.u = j * stepj;
            curTexCoord.v = 1 - i * stepi;

            corner0.x = max(corner0.x, curVertex.x);
            corner0.y = max(corner0.y, curVertex.y);
            corner0.z = max(corner0.z, curVertex.z);
            corner1.x = min(corner1.x, curVertex.x);
            corner1.y = min(corner1.y, curVertex.y);
            corner1.z = min(corner1.z, curVertex.z);

            vertices.push_back(curVertex);
            texcoords.push_back(curTexCoord);
        }
    }

    // Calculate norm vector of each face -> faces
    for (int i = 0;i < height-1;++i)
    {
        for (int j = 0;j < width - 1;++j)
        {
            int v00 = i*width+j;
            int v10 = v00+1;
            int v11 = v10+width;
            int v01 = v11-1;

            Face curFace;
            curFace.vertices[0] = v00;
            curFace.vertices[1] = v10;
            curFace.vertices[2] = v01;
            Vector3D vec0(vertices[v10].x-vertices[v00].x, vertices[v10].y-vertices[v00].y, vertices[v10].z-vertices[v00].z);
            Vector3D vec1(vertices[v01].x-vertices[v10].x, vertices[v01].y-vertices[v10].y, vertices[v01].z-vertices[v10].z);
            curFace.normal = (vec0 ^ vec1) * (1.0); curFace.normal.normalize();
            faces.push_back(curFace);


            curFace.vertices[0] = v01;
            curFace.vertices[1] = v10;
            curFace.vertices[2] = v11;
            Vector3D vec2(vertices[v10].x-vertices[v01].x, vertices[v10].y-vertices[v01].y, vertices[v10].z-vertices[v01].z);
            Vector3D vec3(vertices[v11].x-vertices[v10].x, vertices[v11].y-vertices[v10].y, vertices[v11].z-vertices[v10].z);
            curFace.normal = (vec2 ^ vec3) * (1.0); curFace.normal.normalize();
            faces.push_back(curFace);
        }
    }

    // Transform and Scale
    center.x = (corner0.x + corner1.x)/2.0;
    center.y = (corner0.y + corner1.y)/2.0;
    center.z = (corner0.z + corner1.z)/2.0;
    scaleD = corner0.x - corner1.x;
    scaleD = max(scaleD, corner0.y - corner1.y);
    scaleD = max(scaleD, corner0.z - corner1.z);

    for (int i = 0; i < vertices.size(); ++i)
    {
        vertices[i].x = (vertices[i].x - center.x) / scaleD;
        vertices[i].y = (vertices[i].y - center.y) / scaleD;
        vertices[i].z = (vertices[i].z - center.z) / scaleD;
    }

    return true;
}

void Model::Render(int sign)//sign=1绘制海岛，sign=0绘制倒影
{
    // Draw each face of the 3D mesh
    float sea=-0.05;//海平面
    if(sign==1)
        for (int i = 0;i < faces.size();++i)
        {
            glBegin(GL_TRIANGLES);
            glNormal3f(faces[i].normal.x, faces[i].normal.y, faces[i].normal.z);
            for (int j = 0;j < 3;++j){
                if(vertices[faces[i].vertices[j]].y>=sea)//低于海平面的不绘制
                {
                    glTexCoord2f(texcoords[faces[i].vertices[j]].u,texcoords[faces[i].vertices[j]].v);
                    glVertex3d(vertices[faces[i].vertices[j]].x, vertices[faces[i].vertices[j]].y, vertices[faces[i].vertices[j]].z);
                }
                else
                {
                }
            }
            glEnd();
        }
    else
    {
        for (int i = 0;i < faces.size();++i)
        {
            glBegin(GL_TRIANGLES);
            glNormal3f(faces[i].normal.x, -faces[i].normal.y, faces[i].normal.z);
            for (int j = 0;j < 3;++j){
                if(vertices[faces[i].vertices[j]].y>=sea)//低于海平面的不绘制
                {
                    glTexCoord2f(texcoords[faces[i].vertices[j]].u,texcoords[faces[i].vertices[j]].v);
                    glVertex3d(vertices[faces[i].vertices[j]].x, -1.0*vertices[faces[i].vertices[j]].y+2.0*sea, vertices[faces[i].vertices[j]].z);
                }
                else
                {
                }
            }
            glEnd();
        }
    }
}

void Update(clock_t deltaClock)
{
    double seconds = (double)deltaClock / CLOCKS_PER_SEC;
    seconds /= ratio;
}

void DrawSky() {
    float d=2.0;
    // skybox0
    glBindTexture(GL_TEXTURE_2D, sky_texture[0]);//绑定贴图
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, 2.0*d, d);//左上
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-d, 0.0, d);//左下
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, 0.0, -d);//右下
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, 2.0*d, -d);//右上
    glEnd();
    //skybox1
    glBindTexture(GL_TEXTURE_2D, sky_texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, 2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-d, 0.0,-d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, 0.0, -d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(d,2.0*d, -d);
    glEnd();
    //skybox2
    glBindTexture(GL_TEXTURE_2D, sky_texture[2]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(d, 2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 0.0, -d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, 0.0, d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(d, 2.0*d, d);
    glEnd();
    //skybox3
    glBindTexture(GL_TEXTURE_2D, sky_texture[3]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(d, 2.0*d, d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 0.0, d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, 0.0, d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, 2.0*d, d);
    glEnd();
    //skybox4
    glBindTexture(GL_TEXTURE_2D, sky_texture[4]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, 2.0*d, d);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, 2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 2.0*d, -d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, 2.0*d, d);
    glEnd();
}

void DrawReflection()//把天空盒的y坐标取相反数
{
    float d=2.0;
    // skybox0
    glBindTexture(GL_TEXTURE_2D, sky_texture[0]);//绑定贴图
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -2.0*d, d);//左上
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-d, 0.0, d);//左下
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, 0.0, -d);//右下
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -2.0*d, -d);//右上
    glEnd();
    //skybox1
    glBindTexture(GL_TEXTURE_2D, sky_texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-d, 0.0,-d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, 0.0, -d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(d,-2.0*d, -d);
    glEnd();
    //skybox2
    glBindTexture(GL_TEXTURE_2D, sky_texture[2]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(d, -2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 0.0, -d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, 0.0, d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(d, -2.0*d, d);
    glEnd();
    //skybox3
    glBindTexture(GL_TEXTURE_2D, sky_texture[3]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(d, -2.0*d, d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 0.0, d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, 0.0, d);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -2.0*d, d);
    glEnd();
    //skybox4
    glBindTexture(GL_TEXTURE_2D, sky_texture[4]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, -2.0*d, d);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-d, -2.0*d, -d);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(d, -2.0*d, -d);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(d, -2.0*d, d);
    glEnd();
}

void DrawSea()
{
    float d=2.0;
    float repeatx=10.0;//贴图的时候的变量
    float repeaty=10.0;
    repeatx+=0.5*sin((float)mytime*0.5);
    glBindTexture(GL_TEXTURE_2D, sea_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repeat模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);//重复贴图
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, 0.0, d);
    glTexCoord2f(repeatx, 0.0f); glVertex3f(d, 0.0, d);
    glTexCoord2f(repeatx, repeaty); glVertex3f(d, 0.0, -d);
    glTexCoord2f(0.0f, repeaty); glVertex3f(-d, 0.0, -d);
    glEnd();
}

void DrawMountain()
{
    glBindTexture(GL_TEXTURE_2D, terrain);
    glPolygonMode(GL_FRONT_AND_BACK, polyMode);
    model.Render(1);
}

void DrawMountainReflection()
{
    glBindTexture(GL_TEXTURE_2D, terrain);
    glPolygonMode(GL_FRONT_AND_BACK, polyMode);
    model.Render(0);
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);//清除屏幕和缓存
    glMatrixMode(GL_MODELVIEW);
    clock_t tickCount = clock();
    Update(tickCount - lastTickCount);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity ();
    mytime++;
    gluLookAt(x_pos,y_pos,z_pos,x_pos+100.0*sin(seita)*cos(fai),y_pos+100.0*sin(seita)*sin(fai),z_pos+100.0*cos(seita), 0.0, 1.0, 0.0);
    //从pos看往seita、fai方向
    //-----------天空与倒影绘制
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);DrawSky();//天空正常绘制
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);DrawReflection();//反向天空倒影
    //-----------海洋绘制
    glDepthMask(GL_FALSE);//深度缓冲区设置为只读形式，绘制半透明物体
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);DrawSea();DrawMountainReflection();
    glDepthMask(GL_TRUE);// 完成半透明物体的绘制，将深度缓冲区恢复为可读可写的形式
    //-----------小岛的绘制
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);DrawMountain();
    lastTickCount = tickCount;
    glutSwapBuffers();
    glutPostRedisplay();
}

void myReshape(int width, int height)
{
    if(height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)width/(GLdouble)height, 0.1, 1000.0);
}

void myKeyboard(unsigned char key, int x, int y)
{
    GLfloat dx,dy,dz;
    dx=sin(seita)*cos(fai);
    dy=sin(seita)*sin(fai);
    dz=cos(seita);
    GLfloat ratio=0.02;
    switch(key)
    {
        case 27:
            glutDestroyWindow(curWin);
            exit(0);
            break;
        case 'a':
            z_pos-=dx*ratio;
            x_pos+=dz*ratio;
            break;
        case 'w':
            x_pos+=dx*ratio;
            y_pos+=dy*ratio;
            z_pos+=dz*ratio;
            break;
        case 's':
            x_pos-=dx*ratio;
            y_pos-=dy*ratio;
            z_pos-=dz*ratio;
            break;
        case 'd':
            z_pos+=dx*ratio;
            x_pos-=dz*ratio;
            break;
    }
    glutPostRedisplay();
}

void mySpecial(int key, int x, int y)
{
    GLfloat dx,dy,dz;
    dx=sin(seita)*cos(fai);
    dy=sin(seita)*sin(fai);
    dz=cos(seita);
    GLfloat ratio=0.02;
    switch(key)
    {
        case GLUT_KEY_F1:
            if(!fullscreen)
            {
                glutFullScreen();
                fullscreen = true;
            }
            else
            {
                glutReshapeWindow(640, 480);
                fullscreen = false;
            }
            break;
        case GLUT_KEY_UP:
            x_pos+=dx*ratio;
            y_pos+=dy*ratio;
            z_pos+=dz*ratio;
            break;
        case GLUT_KEY_DOWN:
            x_pos-=dx*ratio;
            y_pos-=dy*ratio;
            z_pos-=dz*ratio;
            break;
        case GLUT_KEY_LEFT:
            z_pos-=dx*ratio;
            x_pos+=dz*ratio;
            break;
        case GLUT_KEY_RIGHT:
            z_pos+=dx*ratio;
            x_pos-=dz*ratio;
            break;
    }
    glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) //处理鼠标点击
{
    if(state==GLUT_DOWN) //第一次鼠标按下时,记录鼠标在窗口中的初始坐标
        oldmx=x,oldmy=y;
}

void onMouseMove(int x,int y) //处理鼠标拖动
{
    float dx=x-oldmx,dy=y-oldmy;
    seita-=dx*0.003;
    fai+=dy*0.003;//鼠标拖动与方向角的变化
    oldmx=x,oldmy=y;//把此时的鼠标坐标作为旧值，为下一次计算增量做准备
}

int myInit()
{
    if(!model.LoadHeightMap("Data/heightmap.bmp", 5, 1,5)) return -1;//10 1 10
    if(!LoadTexture()) return -1;
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    fullscreen = false;
    lastTickCount = clock();
    return 0;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    curWin = glutCreateWindow("CubeMap");

    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);
    glutMouseFunc(Mouse);
    glutMotionFunc(onMouseMove);
    if(myInit() < 0) return -1;
    glutMainLoop();
    return 0;
}
