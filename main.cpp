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
GLfloat angle = 0,tea_p = -40 , tea_face = 100 , donut_size = 3 , seat_pos = -60, board_pos = -50;
GLboolean deskLight = false, air_open = false;

clock_t clock_agl1 = 0;// 分针
clock_t clock_agl2 = 90;// 时针

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
    char filename2[] = "../src/outside.bmp";
    loadTexture(filename2, texName[1]);
//    char filename3[] = "../src/wall.bmp";
//    loadTexture(filename3, texName[2]);

    // OpenGL把现实世界中的光照系统近似归为三部分，分别是光源、材质和光照环境
    // 创建广光源GL_LIGHT1 并设置三个属性GL_POSITION GL_AMBIENT GL_DIFFUSE
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

    quadobj = gluNewQuadric();

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

    // TODO:
    // 此处是核心绘制板块

    // 墙面绘制
    // 后墙
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.4f, 0.7f);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();
    // 天花板
    glBegin(GL_QUADS);
    glColor3f(0.695, 0.703, 0.699);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();
    // 地板
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//纹理和材质混合方式
    glBindTexture(GL_TEXTURE_2D, texName[0]);//floor
    glBegin(GL_QUADS);
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
    // 右墙
    glBegin(GL_QUADS);
    glColor3f(0.6000, 0.7804, 0.9059);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glEnd();
    // 左墙
    glBegin(GL_QUADS);
    glColor3f(0.6000, 0.7804, 0.9059);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, 220.0f);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glEnd();

    // 窗台绘制
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matsafe); // 环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matsafe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack); // 自发光
    // 垫台
    glPushMatrix();
    glColor3f(0.695, 0.703, 0.699);
    glTranslatef(-0, -40, 5);
    glScalef(400, 10, 5);
    glutSolidCube(1);
    glPopMatrix();
    // 垫台下方
    glPushMatrix();
    glColor3f(0.2431, 0.2431, 0.2706);
    glTranslatef(-0, -80, 5);
    glScalef(400, 90, 5);
    glutSolidCube(1);
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
    glVertex3f(-5, -165, -219);
    glVertex3f(5, -165, -219);
    glVertex3f(5, 165, -219);
    glEnd();
    // 窗外
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName[1]); // outside
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_POLYGON);
    glColor3f(0.75f, 0.75f, 0.75f);
    glTexCoord2f(0, 1);
    glVertex3f(-265, 155, -219);
    glTexCoord2f(0, 0);
    glVertex3f(-265, -155, -219);
    glTexCoord2f(1, 0);
    glVertex3f(265, -155, -219);
    glTexCoord2f(1, 1);
    glVertex3f(265, 155, -219);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    // 完成窗台绘制
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
        case'\033'://press 'esc' to quit
            exit(0);
            break;

            // TODO:
            // Add keyboard control here

    }
}

void idle()
{

}


int main(int argc, char** argv)
{

    /*Initialization of GLUT Library */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

    /*Create a window with title specified */
    glutCreateWindow("072110112 冉中益");
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    init();	/*not GLUT call, initialize several parameters */

    /*Register different CALLBACK function for GLUT to response
    with different events, e.g. window sizing, mouse click or
    keyboard stroke */
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    /*Enter the GLUT event processing loop which never returns.
    it will call different registered CALLBACK according
    to different events. */
    glutMainLoop();

    return 0;
}