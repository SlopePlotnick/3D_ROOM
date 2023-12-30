/*****************************************************************************
        FILE : submit.c (Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
		Student Information
		Student ID:
		Student Name:
*****************************************************************************/

#include <cstdlib>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <unistd.h>
#include <cmath>

using namespace std;

const GLdouble FRUSTDIM = 100.0f;

int option = 0, i = 0;
bool enableLight = 1;

/* Do  animation 动画*/
GLfloat angle = 0,tea_p = -40 , tea_face = 100 , donut_size = 3 , seat_pos = 150, board_pos = 0, curtain_pos = 0, quilt_pos = 0;
GLboolean enable_desklight = false, air_open = false, computer_on = true;
int mouseX, mouseY;
bool mouseLeftDown, mouseRightDown;
GLfloat cameraAngleY = 0, cameraAngleX = 0, cameraDistance = 0;

GLUquadricObj *quadobj;

/* For lighting*/
GLfloat light0pos[] = {0.0f, 1.0f, 0.0f, 0.0f};
GLfloat light0_mat1[] = {1.0, 1.0, 1.0, 1.f};
GLfloat light0_diff[] = {1.0, 1.0, 1.0, 0.3};
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };// 镜面反射光

//设置材质颜色
float blackMat[] = { 0.0f,0.0f,0.0f,1.0f };
float ambientMat[] = { 0.1f,0.1f,0.1f,1.0f };
float diffuseMat[] = { 0.4f,0.4f,0.4f,1.0f };
float specularMat[] = { 0.9f,0.9f,0.9f,1.0f };

GLfloat spot_position[] = { -1.0, -1.0, 0};// 指定聚光灯的方向

void update(int value)
{
    angle += 1.0f; // 增加旋转角度
    if (angle > 360.0f)
        angle = 0.0f;

    glutPostRedisplay(); // 请求窗口更新

    glutTimerFunc(16, update, 0); // 设置定时器
}

static GLuint texName[4];//存放纹理
void loadTexture(char* filename, GLuint& texture)
{
    static GLint   ImageWidth;
    static GLint   ImageHeight;
    static GLint   PixelLength;
    static GLubyte* PixelData;

    FILE* pFile = fopen(filename, "rb");
    if (pFile == 0)
        exit(0);
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

    PixelLength = ImageWidth * 3;
    while (PixelLength % 4 != 0)
        ++PixelLength;
    PixelLength *= ImageHeight;

    PixelData = (GLubyte*)malloc(PixelLength);
    if (PixelData == 0)
        exit(0);
    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);

    fclose(pFile);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
}

void init(void) // All Setup For OpenGL Goes Here
{
    // 读取地板纹理
    char filename1[] = "../src/floor.bmp";
    loadTexture(filename1, texName[0]);
    // 窗外纹理
    char filename2[] = "../src/out.bmp";
    loadTexture(filename2, texName[1]);
    // 墙纸纹理
    char filename3[] = "../src/star.bmp";
    loadTexture(filename3, texName[2]);
    // 电脑纹理
    char filename4[] = "../src/computer.bmp";
    loadTexture(filename4, texName[3]);


    glLightfv(GL_LIGHT1, GL_POSITION, light0pos);// 设置光源的位置
    glLightfv(GL_LIGHT1, GL_AMBIENT, light0_mat1);// 设置光的环境强度 环境光Ambient
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diff);// 散色光 漫射光Diffuse
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);// 镜面反射光

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);//设置聚光灯的角度
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_position);//指定聚光灯的方向

    // 材质和颜色
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);

    // 打开光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);		/* Define Counter-Clockwise as front face */

    glEnable(GL_COLOR_MATERIAL);
}

void display(void) // Here's Where We Do All The Drawing
{
    // TODO:
    // 初始化
    // 清除缓存
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0, 0, -550);

    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);

    // 材质初始化
    GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
    GLfloat mat_diffuse[] = {0.9,0.9,0.9,1.0};
    GLfloat mat_specular[] = {0.3,0.3,0.3,1.0};
    GLfloat high_shininess[] = {20.0};
    GLfloat high_mat[] = {1.0,1.0,1.0,1.0};

    GLfloat matBlack[] = {0, 0, 0, 1};
    GLfloat matWhite[] = {1, 1, 1, 1};
    GLfloat matPink[] = {1, 0.8, 0.2, 1};
    GLfloat matBrown[] = {1, 0.6, 0.3, 1};
    GLfloat matYellowGreen[] = {0.5, 1, 0.5, 1};
    GLfloat matRed[] = {1, 0, 0, 1};
    GLfloat matGreen[] = {0, 1, 0, 1};
    GLfloat matBlue[] = {0, 0, 1, 1};
    GLfloat matYellow[] = {1, 1, 0, 1};
    GLfloat matShininess[] = {50};

    GLfloat mood[] = {0.8, 0.61, 0.11};
    GLfloat matsafe[] = {0.45, 0.65, 0.65, 1};
    GLfloat matszz[] = {0.93, 0.65, 0.65, 1};
    GLfloat matsck[] = {1, 0.984, 0.941, 1};
    GLfloat matglass[] = {0.341, 0.98, 1, 0.3};
    GLfloat matcl[] = {0.96, 0.83, 0.851, 0.5};

    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    quadobj = gluNewQuadric();

    // 此处是核心绘制板块

    // 墙面绘制 有时间可以做墙纸
    // 后墙
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.4f, 0.7f);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();
    glPopMatrix();
    // 天花板
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.695, 0.703, 0.699);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();
    glPopMatrix();
    // 地板
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[0]); // floor
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-275.0f, -165.f, -220.f);
    glTexCoord2f(0.0, 5.0);
    glVertex3f(-275.0f, -165.f, 220.f);
    glTexCoord2f(5.0, 5.0);
    glVertex3f(275.0f, -165.f, 220.f);
    glTexCoord2f(5.0, 0.0);
    glVertex3f(275.0f, -165.f, -220.f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    //此处还可以接着绘制线条增加美观程度
    // 右墙
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[2]); // wall
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glTexCoord2f(0, 0);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glTexCoord2f(1, 0);
    glVertex3f(275.0f, -165.0f, 220.0f);
    glTexCoord2f(1, 1);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    // 左墙
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[2]); // wall
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glTexCoord2f(0, 0);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glTexCoord2f(1, 0);
    glVertex3f(-275.0f, -165.0f, 220.0f);
    glTexCoord2f(1, 1);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 窗台绘制
    glPushMatrix();
    // 垫台
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.695, 0.703, 0.699);
    glVertex3f(-275, -50, -219);
    glVertex3f(-275, -60, -219);
    glVertex3f(275, -60, -219);
    glVertex3f(275, -50, -219);
    glPopMatrix();
    // 垫台下方
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.2431, 0.2431, 0.2706);
    glVertex3f(-275, -60, -219);
    glVertex3f(-275, -165, -219);
    glVertex3f(275, -165, -219);
    glVertex3f(275, -60, -219);
    glPopMatrix();
    // 窗框
    glBegin(GL_QUADS);
    glColor3f(0.0549, 0.0980, 0.2078);
    glVertex3f(-275, 165, -219);
    glVertex3f(-275, -50, -219);
    glVertex3f(-265, -50, -219);
    glVertex3f(-265, 165, -219);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0549, 0.0980, 0.2078);
    glVertex3f(-275, 165, -219);
    glVertex3f(-275, 155, -219);
    glVertex3f(275, 155, -219);
    glVertex3f(275, 165, -219);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0549, 0.0980, 0.2078);
    glVertex3f(275, 165, -219);
    glVertex3f(275, -50, -219);
    glVertex3f(265, -50, -219);
    glVertex3f(265, 165, -219);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0549, 0.0980, 0.2078);
    glVertex3f(-275, -50, -219);
    glVertex3f(-275, -40, -219);
    glVertex3f(275, -40, -219);
    glVertex3f(275, -50, -219);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0549, 0.0980, 0.2078);
    glVertex3f(-5, 165, -219);
    glVertex3f(-5, -50, -219);
    glVertex3f(5, -50, -219);
    glVertex3f(5, 165, -219);
    glEnd();
    // 窗外
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[1]); // outside
    glBegin(GL_POLYGON);
    glColor3f(0.75f, 0.75f, 0.75f);
    glTexCoord2f(0, 1);
    glVertex3f(-265, 155, -219);
    glTexCoord2f(0, 0);
    glVertex3f(-265, -50, -219);
    glTexCoord2f(1, 0);
    glVertex3f(265, -50, -219);
    glTexCoord2f(1, 1);
    glVertex3f(265, 155, -219);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    // 窗帘
    glPushMatrix();
    glColor3f(0.7529, 0.6941, 0.5804);
    glBegin(GL_QUADS);
    glVertex3f(-275, 165, -218);
    glVertex3f(-275, -165, -218);
    glVertex3f(0 - curtain_pos, -165, -218);
    glVertex3f(0 - curtain_pos, 165, -218);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(275, 165, -218);
    glVertex3f(275, -165, -218);
    glVertex3f(0 + curtain_pos, -165, -218);
    glVertex3f(0 + curtain_pos, 165, -218);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    // 绘制灯泡
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, light0_mat1);
    glColor3f(0.8f, 0.8f, 0.8f);
    glTranslatef(0, 155, 0);
    glScalef(100, 10, 100);
    glutSolidCube(1);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // 右侧绘制
    glPushMatrix();
    // 桌面
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(220, -85, 20);
    glScalef(100, 15, 200);
    glutSolidCube(1);
    glPopMatrix();
    // 椅子
    glTranslatef(seat_pos, -165.f, 0.f);
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.f,0.f,0.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 77, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.f,0.f,80.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 77, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.f,0.f,0.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 38, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.f,0.f,80.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 38, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.f,20.f,0.f);
    glRotatef(90, 0.f, 1.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 30, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.f,20.f,80.f);
    glRotatef(90, 0.f, 1.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 30, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.f,60.f,0.f);
    glRotatef(0, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 80, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.f,37.f,80.f);
    glRotatef(-90, 0.f, 1.f, 0.f);
    gluCylinder(quadobj, 3.f, 3.f, 30, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.f,37.f,0.f);
    glRotatef(-90, 0.f, 1.f, 0.f);
    gluCylinder(quadobj, 3.f, 3.f, 30, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.f,37.f,-2.f);
    glRotatef(0, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 3.f, 3.f, 84, 20, 20);
    glPopMatrix();
    // 坐垫
    glPushMatrix();
    glTranslatef(15.f, 37.f, 40.f);
    glScalef(30.f, 5.0f, 82.f);
    glutSolidCube(1.f);
    glPopMatrix();
    glTranslatef(-seat_pos, 100.f, -98.f);
    // 台灯
    glPushMatrix();
    glColor3f(0.8902, 0.3843, 0.1647);
    glTranslatef(230, -15, 100);
    glScalef(20, 10, 1);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.8902, 0.3843, 0.1647);
    glTranslatef(230, -15, 100);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 45, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.8902, 0.3843, 0.1647);
    glTranslatef(210, 25, 100);
    glScalef(50, 5, 1);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, light0_mat1);
    glColor4f(0.8f, 0.8f, 0.7f,0.1f);
    glTranslatef(195.f, 20.f, 100.f);
    glutSolidSphere(2.5, 12.5, 50.f);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    // 抽屉和把手
    glTranslatef(0, 0, 200);
    glPushMatrix();
    glPushMatrix(); // 1
    glTranslatef(249, 89, -60);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, 89, -60);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix(); // 2
    glTranslatef(249 - board_pos, 102, -60);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223 - board_pos, 102, -60);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix(); // 3
    glTranslatef(249, 89, 5);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, 89, 5);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix(); // 4
    glTranslatef(249, 102, 5);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, 102, 5);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix(); // 5
    glTranslatef(249, 89, -125);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, 89, -125);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix(); // 6
    glTranslatef(249, 102, -125);
    glColor3f(0.6510, 0.8196, 0.9255);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, 102, -125);
    glColor3f(0, 0, 0);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glTranslatef(0, 0, -200);
    // 抽屉主体
    glPushMatrix();
    glColor3f(0.0549, 0.0980, 0.2078);
    glTranslatef(250, 30, 250);
    glScalef(50, 30, 200);
    glutSolidCube(1);
    glPopMatrix();
    // 完成右侧绘制
    glPopMatrix();

    // 左侧绘制
    glPushMatrix();
    // 床
    glTranslatef(-220, -165.f, -40.f);
    glPushMatrix();
    // 支撑
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.f,0.f,0.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 10.f, 10.f, 35, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.f,0.f,120.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 10.f, 10.f, 35, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(70.f,0.f,0.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 10.f, 10.f, 35, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(70.f,0.f,120.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 10.f, 10.f, 35, 20, 20);
    glPopMatrix();
    // 床板
    glPushMatrix();
    glColor3f(0.8706, 0.8000, 0.6824);
    glTranslatef(35, 42, 20);
    glScalef(100, 15, 230);
    glutSolidCube(1);
    glPopMatrix();
    // 被子
    glPushMatrix();
    glColor3f(0.6, 0.1686, 0.1608);
    glTranslatef(35, 54, 60.5);
    glScalef(100, 10, 150);
    glutSolidCube(1);
    glPopMatrix();
    // 枕头
    glPushMatrix();
    glColor3f(0.8118, 0.7922, 0.7725);
    glTranslatef(40, 54, -25);
    glScalef(50, 30, 15);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glTranslatef(220, 165.f, 40.f);
    // 空调
    glPushMatrix();
    glTranslatef(-270, 25, 0);
    glScalef( 2, 2, 2);
    if (air_open){// 开空调
        glPushMatrix();
        glTranslatef(0, 10, -2);
        glScalef( 8.3, 20, 86);
        glColor3f(1.0, 1.0, 1.0);// 白色
        glutSolidCube(1);
        glLineWidth(4);
        glColor3f(1, 1, 1);
        glutWireCube(1);
        glLineWidth(1);
        glPopMatrix();


        glPushMatrix();
        glTranslatef(0, -1, 0);
        glScalef( 0.001, 0.001, 83);
        glColor3f(0.0, 0.0, 0.0);// 黑色
        glutSolidCube(1);
        glLineWidth(3);
        glColor3f(0.0, 0.0, 0.0);
        glutWireCube(1);
        glLineWidth(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3.0, -0.5, 0);
        glScalef( 0.001, 0.001, 83);
        glColor3f(0.0, 0.0, 0.0);// 黑色
        glutSolidCube(1);
        glLineWidth(3);
        glColor3f(0.0, 0.0, 0.0);
        glutWireCube(1);
        glLineWidth(1);
        glPopMatrix();

    }
    else
    {// 关空调
        glPushMatrix();
        glTranslatef(0, 10, 0);
        glScalef( 8.3, 20, 86);
        glColor3f(1.0, 1.0, 1.0);// 白色
        glutSolidCube(1);
        glLineWidth(4);
        glColor3f(1, 1, 1);
        glutWireCube(1);
        glLineWidth(1);
        glPopMatrix();
    }
    glPopMatrix();

    // 完成左侧绘制
    glPopMatrix();

    // 茶桌
//    glTranslatef(0, 0, 100);
    glPushMatrix();
    glTranslatef(0.f, -75.f,-50.f);
    glPushMatrix();
    glColor3f(0.168f, 0.23f, 1.0f);
    glTranslatef(0.f, -90.f, 130.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 10.f, 5.f, 40.f, 20.f, 20.f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.168f, 0.23f, 1.0f);
    glTranslatef(0.f, -50.f, 130.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 50.f, 50.f, 10.2f, 30.f, 30.f);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.168f, 0.23f, 1.0f);
    glTranslatef(0.f, -40.f, 130.f);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluDisk(quadobj, 0.f, 50.7f, 20.f, 20.f);
    glPopMatrix();
    glTranslatef(0.f, -40.f, 130.f);
    glRotatef(angle, 0.f, 1.f, 0.f);
    glPushMatrix();
    glColor4f(0.5f, 0.0f, 0.0f,0.5f);
    glTranslatef(-30.f, 10.f, 0.f);
    glRotatef(-90.f, 0.f, 1.f, 0.f);
    glScalef(100.f, 100.0f, -100.f);
    glutSolidTeapot(0.1);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(10.f,1, 0.f);
    glScalef(10, 2, 10);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(10, -5, 0);
    glRotatef(-90, 1.f, 0.f, 0.f);
    gluCylinder(quadobj, 2.f, 2.f, 20, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(10.f,30, 0.f);
    glScalef(40, 30, 5);
    glutSolidCube(1);
    glPopMatrix();
    if (computer_on) {
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texName[3]); // wall
        glColor3f(1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 1);
        glVertex3f(-10, 45, 3);
        glTexCoord2f(0, 0);
        glVertex3f(-10, 15, 3);
        glTexCoord2f(1, 0);
        glVertex3f(29.5, 15, 3);
        glTexCoord2f(1, 1);
        glVertex3f(29.5, 45, 3);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    glPopMatrix();

    // 完成所有绘制
    glPopMatrix();

    glutSwapBuffers();
    glFlush();

    // 刷新整个框架
    glutPostRedisplay();

}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-100., 100., -100., 100., 165., 790.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
    switch (key)
    {
        case '\033': //press 'esc' to quit
            exit(0);
            break;
        case '1': // 减弱灯光
            if (enableLight)
                if(light0_mat1[0] >= 0){
                    for (i=0; i<=3 ; i++){
                        light0_mat1[i] -= 0.1;
                        light0_diff[i] -= 0.1;
                        light_specular[i] -= 0.1;
                    }
                    glLightfv(GL_LIGHT1, GL_AMBIENT, light0_mat1);
                    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diff);
                    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);// 镜面反射光
                    glEnable(GL_LIGHT1);
                }
            break;
        case '2': // 增强灯光
            if (enableLight)
                if(light0_mat1[0] <= 1){
                    for (i=0; i<=3 ; i++){
                        light0_mat1[i] += 0.1;
                        light0_diff[i] += 0.1;
                        light_specular[i] += 0.1;
                    }
                    glLightfv(GL_LIGHT1, GL_AMBIENT, light0_mat1);
                    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diff);
                    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);// 镜面反射光
                    glEnable(GL_LIGHT1);
                }
            break;
        case '3': // 打开窗帘
            if (curtain_pos <= 200)
            {
                curtain_pos += 10;
                if (enableLight)
                    if (light0_mat1[0] >= 0)
                    {
                        for (i = 0; i <= 3; i++)
                        {
                            light0_mat1[i] += 0.1;
                            light0_diff[i] += 0.1;
                            light_specular[i] += 0.1;
                            // printf("2 %d\n", aCeilingLight);
                        }
                    }
            }
            break;
        case '4': // 关闭窗帘
            if (curtain_pos >= 0)
            {
                curtain_pos -= 10;
                if (enableLight)
                    if (light0_mat1[0] >= 0)
                    {
                        for (i = 0; i <= 3; i++)
                        {
                            light0_mat1[i] -= 0.1;
                            light0_diff[i] -= 0.1;
                            light_specular[i] -= 0.1;
                            // printf("2 %d\n", aCeilingLight);
                        }
                    }
            }
            break;
        case '5': // 推椅子
            if (seat_pos <= 150)
                seat_pos++;
            break;
        case '6': // 拉椅子
            if (seat_pos >= 130)
                seat_pos--;
            break;
        case 's': // 拉抽屉
            if (board_pos == 0)
                board_pos = 20;
            break;
        case 'd': // 推抽屉
            if (board_pos == 20)
                board_pos = 0;
            break;
        case 'a': // 开关灯光
            if(enableLight)
            {
                enableLight = false;
                for (i = 0; i <= 3; i++){
                    light0_mat1[i] = 0.0;
                }
                glLightfv(GL_LIGHT1, GL_AMBIENT, light0_mat1);
                glDisable(GL_LIGHT1);
            }
            else
            {
                enableLight = true;
                for (i = 0; i <= 3; i++){
                    light0_mat1[i] = 1.0;
                    light0_diff[i] = 1.0;
                }
                glLightfv(GL_LIGHT1, GL_AMBIENT, light0_mat1);
                glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diff);// 散色光 漫射光Diffuse

                glEnable(GL_LIGHT1);
            }
            break;
        case 'f':
            air_open = !air_open;
            break;
        case 'g':
            computer_on = !computer_on;
            break;
        case 'h':
            if(angle >= 360)
                angle = 0;
            angle++;
            break;
        case 'j':
            if(angle <= -360)
                angle = 0;
            angle--;
    }
}

void mouseMotionCB(int x, int y)
{
    if (mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if (state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if (state == GLUT_UP)
            mouseRightDown = false;
    }
}

int main(int argc, char** argv)
{

    /*Initialization of GLUT Library */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

    /*Create a window with title specified */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("072110112 冉中益");

    init();	/*not GLUT call, initialize several parameters */

    /*Register different CALLBACK function for GLUT to response
    with different events, e.g. window sizing, mouse click or
    keyboard stroke */
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
//    glutTimerFunc(0, update, 0); // 启动定时器
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCB);        // 鼠标键位响应
    glutMotionFunc(mouseMotionCB); // 鼠标键摁下时拖动的事件

    /*Enter the GLUT event processing loop which never returns.
    it will call different registered CALLBACK according
    to different events. */
    glutMainLoop();

    return 0;
}