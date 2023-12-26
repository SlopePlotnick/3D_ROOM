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
//#include <OpenGL/glaux.h>

int i;

/* Do  animation*/
GLfloat angle = 0, cangle = 0, donut_size = 3, Y = 0, cll=0;
GLfloat chairws = 0, chairad = 0;
GLUquadricObj* quadobj;

//光源参数
//0号光源
GLfloat light_pos0[] = { 0, 0, -220, 1 };
//GLfloat light_dir0[] = { 0, -1, 0 };
//GLfloat light_cutoff0 = 45;
GLfloat light_amb0[] = { 0.01, 0.01, 0.01, 0 };
GLfloat light_dif0[] = { 0.01, 0.01, 0.01, 0.01 };
GLfloat light_spc0[] = { 0.01, 0.01, 0.01, 0 };
//1号光源
GLfloat light_pos1[] = { 0, 164, 0, 1 };
GLfloat light_dir1[] = { 0, -1, 0 };
GLfloat light_cutoff1 = 50;
GLfloat light_amb1[] = { 0.5, 0.5, 0.5, 0 };
GLfloat light_dif1[] = { 0.5, 0.5, 0.5, 0 };
GLfloat light_spc1[] = { 0.5, 0.5, 0.5, 0 };
//2号光源
GLfloat light_pos2[] = { 0, 165, 0, 1 };
GLfloat light_dir2[] = { 0, -16.5, 0 };
GLfloat light_cutoff2 = 50;
GLfloat light_amb2[] = { 0.2, 0.15, 0, 1.0 };
GLfloat light_dif2[] = { 0.8, 0.75, 0, 1.0 };
GLfloat light_spc2[] = { 1.0, 1.0, 0, 1.0 };
bool dsopen = false;
bool aRoomLight = true;
bool aCeilingLight = false;
int mouseX, mouseY;
bool mouseLeftDown, mouseRightDown;
GLfloat cameraAngleY = 0, cameraAngleX = 0, cameraDistance = 0;
GLuint picture;
GLuint chuanghu;
GLuint dianshi;
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}
void ChairLeg()
{
    //glBegin(GL_QUAD_STRIP);//填充凸多边形4个点绘制一个四边形，6个点绘制2个四边形，8个点绘制3个四边形；
    //glVertex3f(0, 0, 0);
    //glVertex3f(0, 10, 0);
    //glVertex3f(10, 0, 0);
    //glVertex3f(10, 10, 0);
    //glVertex3f(10, 0, -10);
    //glVertex3f(10, 10, -10);
    //glVertex3f(0, 0, -10);
    //glVertex3f(0, 10, -10);
    //glEnd();
    //glBegin(GL_QUAD_STRIP);
    //glVertex3f(0, 0, 0);
    //glVertex3f(10, 0, 0);
    //glVertex3f(0, 0, -10);
    //glVertex3f(10, 0, -10);
    //glVertex3f(0, 10, 0);
    //glVertex3f(10, 10, 0);
    //glVertex3f(0, 10, -10);
    //glVertex3f(10, 10, -10);
    //glEnd();
    glRotatef(90, 0, -1, 0);
    glRotatef(20, -1, 0, 0);
    glBegin(GL_QUAD_STRIP);//填充凸多边形4个点绘制一个四边形，6个点绘制2个四边形，8个点绘制3个四边形；
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);
    glVertex3f(10, 0, 0);
    glVertex3f(10, 10, 0);
    glVertex3f(7, 3, -60);
    glVertex3f(7, 7, -60);
    glVertex3f(3, 3, -60);
    glVertex3f(3, 7, -60);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);
    glVertex3f(3, 3, -60);
    glVertex3f(7, 3, -60);
    glVertex3f(0, 10, 0);
    glVertex3f(10, 10, 0);
    glVertex3f(3, 7, -60);
    glVertex3f(7, 7, -60);
    glEnd();
}
void setLight0(bool flag)
{
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir0);
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light_cutoff0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif0);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_spc0);

    if (flag)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
}

void setLight1(bool flag)
{
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light_cutoff1);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_dif1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_spc1);

    if (flag)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
}

void setLight2(bool flag)
{
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light_cutoff2);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_amb2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_dif2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_spc2);

    if (flag)
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);
}/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID = 0, texture_ID = 0;

    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if (pFile == 0)
        return 0;

    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    printf("%d %d", width, height);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        fclose(pFile);
        return 0;
    }

    // 读取像素数据
    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;

            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            // 进行像素缩放
            gluScaleImage(GL_RGB,
                          width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }

    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID = last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}
void Initial(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glFrontFace(GL_CCW);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


}
void myDisplay(void)
{


    quadobj = gluNewQuadric();
    //创建光源
    //GLfloat lmodel_ambient[] = { 0.55,0.55,0.55,1 };//微弱环境光，使物体可见
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    setLight0(aRoomLight);
    setLight1(aCeilingLight);
    //setLight2(aAndroidLight);
    //清空缓存
    //glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //初始化材质属性
    GLfloat matBlack[] = { 0, 0, 0, 1 };
    GLfloat matWhite[] = { 1, 1, 1, 1 };
    GLfloat matPink[] = { 1, 0.8, 0.2, 1 };
    GLfloat matBrown[] = { 1, 0.6, 0.3, 1 };
    GLfloat matYellowGreen[] = { 0.5, 1, 0.5, 1 };
    GLfloat matRed[] = { 1, 0, 0, 1 };
    GLfloat matGreen[] = { 0, 1, 0, 1 };
    GLfloat matBlue[] = { 0, 0, 1, 1 };
    GLfloat matYellow[] = { 1, 1, 0, 1 };
    GLfloat matShininess[] = { 50 };
    GLfloat mat_diffuse[] = { 0.9,0.9,0.9,1.0 };
    GLfloat mat_specular[] = { 0.3,0.3,0.3,1.0 };

    GLfloat mood[] = { 0.8, 0.61, 0.11 };
    GLfloat matsafe[] = { 0.45, 0.65, 0.65, 1 };
    GLfloat matszz[] = { 0.93, 0.65, 0.65, 1};
    GLfloat matsck[] = { 1, 0.984, 0.941, 1 };
    GLfloat matglass[] = { 0.341, 0.98, 1, 0.3 };
    GLfloat matcl[] = { 0.96, 0.83, 0.851, 0.5 };


    glPushMatrix();      //入栈，将当前状态入栈
    glTranslatef(0, 0 + Y, -550);      //平移函数

    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    //窗户
    //窗框
    glPushMatrix();
    glColor3f(1, 0.984, 0.941);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matsck);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matsck);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matsck);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glBegin(GL_QUADS);
    glVertex3f(-212, -159, -219.8);
    glVertex3f(-212, -165, -219.8);
    glVertex3f(212, -165, -219.8);
    glVertex3f(212, -159, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-212, 135, -219.8);
    glVertex3f(-212, 129, -219.8);
    glVertex3f(212, 129, -219.8);
    glVertex3f(212, 135, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-212, 129, -219.8);
    glVertex3f(-212, -159, -219.8);
    glVertex3f(-206, -159, -219.8);
    glVertex3f(-206, 129, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(206, 129, -219.8);
    glVertex3f(206, -159, -219.8);
    glVertex3f(212, -159, -219.8);
    glVertex3f(212, 129, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-2, 129, -219.8);
    glVertex3f(-2, -159, -219.8);
    glVertex3f(2, -159, -219.8);
    glVertex3f(2, 129, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-106, 129, -219.8);
    glVertex3f(-106, -159, -219.8);
    glVertex3f(-102, -159, -219.8);
    glVertex3f(-102, 129, -219.8);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(102, 129, -219.8);
    glVertex3f(102, -159, -219.8);
    glVertex3f(106, -159, -219.8);
    glVertex3f(106, 129, -219.8);
    glEnd();
    glPopMatrix();
    //玻璃
    glPushMatrix();
    glColor3f(0.341, 0.98, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matglass);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matglass);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matglass);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    /*glBegin(GL_QUADS);
    glVertex3f(-206, 129, -219.8);
    glVertex3f(-206, -159, -219.8);
    glVertex3f(-106, -159, -219.8);
    glVertex3f(-106, 129, -219.8);
    glEnd();*/
    //glBegin(GL_QUADS);
    //glVertex3f(-102, 129, -219.8);
    //glVertex3f(-102, -159, -219.8);
    //glVertex3f(-2, -159, -219.8);
    //glVertex3f(-2, 129, -219.8);
    //glEnd();
    //glBegin(GL_QUADS);
    //glVertex3f(2, 129, -219.8);
    //glVertex3f(2, -159, -219.8);
    //glVertex3f(102, -159, -219.8);
    //glVertex3f(102, 129, -219.8);
    //glEnd();
    //glBegin(GL_QUADS);
    //glVertex3f(106, 129, -219.8);
    //glVertex3f(106, -159, -219.8);
    //glVertex3f(206, -159, -219.8);
    //glVertex3f(206, 129, -219.8);
    //glEnd();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, chuanghu);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_POLYGON);
    glColor3f(0.75f, 0.75f, 0.75f);
    glTexCoord2f(1, 1); glVertex3f(206, 129, -219.9);
    glTexCoord2f(0, 1); glVertex3f(-206, 129, -219.9);
    glTexCoord2f(0, 0); glVertex3f(-206, -159, -219.9);
    glTexCoord2f(1, 0); glVertex3f(206, -159, -219.9);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //glBegin(GL_QUADS);
    //glVertex3f(-206, 129, -219.9);
    //glVertex3f(-206, -159, -219.9);
    //glVertex3f(206, -159, -219.9);
    //glVertex3f(206, 129, -219.9);
    //glEnd();
    glPopMatrix();

    //窗帘
    glPushMatrix();
    glColor3f(0.96, 0.83, 0.851);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matcl);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matcl);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matcl);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glBegin(GL_QUADS);
    glVertex3f(-275, 165, -219.7);
    glVertex3f(-275, -165, -219.7);
    glVertex3f(0 - cll, -165, -219.7);
    glVertex3f(0 - cll, 165, -219.7);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(0 + cll, 165, -219.7);
    glVertex3f(0 + cll, -165, -219.7);
    glVertex3f(275, -165, -219.7);
    glVertex3f(275, 165, -219.7);
    glEnd();
    glPopMatrix();

    //椅子
    glPushMatrix();
    glTranslatef(-180 - chairad, -150, 200 - chairws);
    glRotatef(cangle, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    //表面
    glPushMatrix();
    glRotatef(angle, 0, 1, 0);
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matWhite);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glPushMatrix();
    glTranslatef(0, 50, 0);
    glScalef(55, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(40, 47, 0);
    glRotatef(12, 0, 0, -1);
    glScalef(30, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(10, 0, 0);
    glPushMatrix();
    glTranslatef(-43, 55, 0);
    glRotatef(24, 0, 0, -1);
    glScalef(20, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-53, 65, 0);
    glRotatef(60, 0, 0, -1);
    glScalef(20, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-56, 75, 0);
    glRotatef(85, 0, 0, -1);
    glScalef(20, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-56, 120, 0);
    glRotatef(90, 0, 0, -1);
    glScalef(80, 10, 85);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    //支撑
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matWhite);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glTranslatef(0, 0, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadobj, 6, 6, 50, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matWhite);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glTranslatef(0, 0, 0);
    glPushMatrix();//1
    ChairLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(55, -20, 5);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPushMatrix();//1
    glRotatef(72, 0, -1, 0);
    ChairLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(14, -20, 52);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPushMatrix();//1
    glRotatef(144, 0, -1, 0);
    ChairLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-48, -20, 29);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPushMatrix();//1
    glRotatef(216, 0, -1, 0);
    ChairLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-45, -20, -38);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPushMatrix();//
    glRotatef(288, 0, -1, 0);
    ChairLeg();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(20, -20, -52);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    //沙发
    //主体
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matsafe);//环境光反射
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matsafe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);  //自发光
    glColor3f(0.5, 0.7, 0.70);
    glPushMatrix();
    glTranslatef(-215, -153, -70);
    glScalef(90, 24, 260);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-100, -153, -155);
    glScalef(140, 24, 90);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-217, -129, -72);
    glScalef(86, 24, 256);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-106, -129, -157);
    glScalef(136, 24, 86);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-248, -120, -72);
    glScalef(24, 90, 256);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-137, -130, -188);
    glScalef(198, 70, 24);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-205, -130, 44);
    glScalef(62, 70, 24);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //桌子
    //桌子腿
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matszz);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matszz);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matszz);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matBlack);
    glColor3f(0.98, 0.7, 0.70);
    glPushMatrix();
    glTranslatef(-2, -141, 78);
    glScalef(4, 48, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-88, -141, 78);
    glScalef(4, 48, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-88, -141, -78);
    glScalef(4, 48, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2, -141, -78);
    glScalef(4, 48, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //桌面
    glPushMatrix();
    glColor3f(0.98, 0.7, 0.70);
    glPushMatrix();
    glTranslatef(-45, -120, 0);
    glScalef(90, 6, 160);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-45, -142, 0);
    glScalef(90, 6, 160);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //一个桌子上的茶壶
    glPushMatrix();
    glColor4f(0.5, 0.0, 0.0, 0.5);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glTranslatef(-60, -110, 40);
    glRotatef(0, 0, 1, 0);
    glScalef(10, 10, -10);
    glutSolidTeapot(1);
    glPopMatrix();

    //一个盘子
    glPushMatrix();
    glColor3f(0.7, 0.7, 0.7);
    glTranslatef(-30, -115, -40);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadobj, 3, 10, 2, 30, 30);
    glPopMatrix();

    //一个甜甜圈
    glPushMatrix();
    glColor3f(0.4, 0.2, 0);
    glTranslatef(-30, donut_size - 115, -40);
    glRotatef(-90, 1, 0, 0);
    glutSolidTorus(donut_size - 1, donut_size, 110, 110);
    glPopMatrix();

    //柜子
    //柜子腿
    glPushMatrix();
    glColor3f(0.8, 0.61, 0.11);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glPushMatrix();
    glTranslatef(273, -147.5, 98);
    glScalef(4, 35, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(227, -147.5, 98);
    glScalef(4, 35, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(227, -147.5, -98);
    glScalef(4, 35, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(273, -147.5, -98);
    glScalef(4, 35, 4);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //抽屉和把手
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glPushMatrix();//1
    glTranslatef(249, -139, 0);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -139, 0);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//2
    glTranslatef(249, -152, 0);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -152, 0);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//3
    glTranslatef(249, -139, 65);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -139, 65);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//4
    glTranslatef(249, -152, 65);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -152, 65);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//5
    glTranslatef(249, -139, -65);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -139, -65);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();//6
    glTranslatef(249, -152, -65);
    glColor3f(0, 1, 0.1);
    glScalef(50, 10, 60);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(223, -152, -65);
    glColor3f(0.5, 1, 0.1);
    glScalef(2, 2, 2);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //主体
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glColor3f(0.8, 0.61, 0.11);
    glTranslatef(250, -145, 0);
    glScalef(50, 30, 200);
    glutSolidCube(1);
    glPopMatrix();

    //电视机
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glPushMatrix();
    glColor3f(0.25, 0.25, 0.25);
    glTranslatef(273, -50, 0);
    glScalef(7, 80, 150);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0., 0., 0.);
    glTranslatef(273, -50, 0);
    glScalef(6, 90, 160);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    if(dsopen)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, dianshi);
        glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
        glBegin(GL_POLYGON);
        glColor3f(0.75f, 0.75f, 0.75f);
        glTexCoord2f(1, 1); glVertex3f(269.4, -10, 75);
        glTexCoord2f(0, 1); glVertex3f(269.4, -10, -75);
        glTexCoord2f(0, 0); glVertex3f(269.4, -90, -75);
        glTexCoord2f(1, 0); glVertex3f(269.4, -90, 75);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    //灯泡
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.8f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    if (aCeilingLight) glMaterialfv(GL_FRONT, GL_EMISSION, matWhite);
    glTranslatef(0, 155, 0);
    glScalef(100, 10, 100);
    glutSolidCube(1);
    glPopMatrix();
    //完成灯泡

    //画
    //画框
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    glBegin(GL_POLYGON);
    glColor3f(0.6, 0.4, 0.12);
    glVertex3f(-274, 50, 90);
    glVertex3f(-274, 50, -90);
    glVertex3f(-274, -50, -90);
    glVertex3f(-274, -50, 90);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, picture);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_POLYGON);
    glColor3f(0.75f, 0.75f, 0.75f);
    glTexCoord2f(1, 1); glVertex3f(-273, 48, 88);
    glTexCoord2f(0, 1); glVertex3f(-273, 48, -88);
    glTexCoord2f(0, 0); glVertex3f(-273, -48, -88);
    glTexCoord2f(1, 0); glVertex3f(-273, -48, 88);
    glEnd();
    glDisable(GL_TEXTURE_2D);



    //墙
    //后墙
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.4f, 0.7f);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();

    //天花板
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.7f);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glEnd();

    //地板
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.7f);
    glVertex3f(-275.f, -165.f, -220.f);
    glVertex3f(-275.f, -165.f, 220.f);
    glVertex3f(275.f, -165.f, 220.f);
    glVertex3f(275.f, -165.f, -220.f);
    glEnd();

    //右墙
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.7f, 0.7f);
    glVertex3f(275.0f, 165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, -220.0f);
    glVertex3f(275.0f, -165.0f, 220.0f);
    glVertex3f(275.0f, 165.0f, 220.0f);
    glEnd();

    //左墙
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.7f, 0.7f);
    glVertex3f(-275.0f, 165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, -220.0f);
    glVertex3f(-275.0f, -165.0f, 220.0f);
    glVertex3f(-275.0f, 165.0f, 220.0f);
    glEnd();

    ////前墙
    //glBegin(GL_POLYGON);
    //glColor3f(0.4f, 0.4f, 0.7f);
    //glVertex3f(-275.0f, 165.0f, 200.0f);
    //glVertex3f(-275.0f, -165.0f, 200.0f);
    //glVertex3f(275.0f, -165.0f, 200.0f);
    //glVertex3f(275.0f, 165.0f, 200.0f);
    //glEnd();
    //完成墙
    glPopMatrix();

    glutSwapBuffers();  //换两个缓冲区指针
    glFlush();     //刷新缓冲

    //刷新整个框架
    glutPostRedisplay();
}
void reshape(int w, int h)        // 改变windows窗口大小  w=width, h=height
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);   //视口变换

    glMatrixMode(GL_PROJECTION);               //投影矩阵
    glLoadIdentity();                    //把当前矩阵设为单位矩阵
    // Frustum Setting
    glFrustum(-100, 100, -100, 100, 150., 790.);  //创建一个透视投影矩阵并乘以当前矩阵

    //glMatrixMode(GL_MODELVIEW);                     //模型视图
    //glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y) // 处理键盘输入
{
    switch (key)
    {
        case '\033':
            exit(0);
            break;
        case 'p': //暂停
        {
            angle = 0;
            cangle = 0;
            donut_size = 3;
            Y = 0;
            chairws = 0;
            chairad = 0;
            for (i = 0; i <= 3; i++) {
                light_amb1[i] = 0.5;
                light_dif1[i] = 0.5;
                light_spc1[i] = 0.2;
            }
            light_cutoff1 = 50;
            aRoomLight = true;
            aCeilingLight = false;
            cameraAngleY = 0;
            cameraAngleX = 0;
            cameraDistance = 0;
            break;
        }
        case '1': //开关灯光
            aCeilingLight = !aCeilingLight;
            printf("1 %d\n", aCeilingLight);
            break;
        case '2': //减弱灯光
            if (aCeilingLight)
                if (light_amb1[0] >= 0) {
                    for (i = 0; i <= 3; i++) {
                        light_amb1[i] -= 0.01;
                        light_dif1[i] -= 0.01;
                        light_spc1[i] -= 0.01;
                        //printf("2 %d\n", aCeilingLight);
                    }
                    //glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
                    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
                    //glEnable(GL_LIGHT0);
                }
            break;

        case '3':  //增强灯光
            if (aCeilingLight)
                if (light_amb1[0] <= 1) {
                    for (i = 0; i <= 3; i++) {
                        light_amb1[i] += 0.01;
                        light_dif1[i] += 0.01;
                        light_spc1[i] += 0.01;
                    }
                    //glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
                    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
                    //glEnable(GL_LIGHT0);
                }
            break;
        case '4':    //逆时针旋转
            aRoomLight = !aRoomLight;
            printf("4 %d\n", aRoomLight);
            break;
        case '5':       //顺时针旋转
            if (donut_size <= 5)
                donut_size += 0.2;
            break;
        case '6':
            if (donut_size > 3)
                donut_size -= 0.2;
            break;
        case '7':
            Y += 10;
            break;
        case '8':
            Y -= 10;
            break;
        case '9':
            dsopen = !dsopen;
            break;
        case 'r':
            if (cll <= 200)
            {
                cll += 10;
                if (aRoomLight)
                    if (light_amb1[0] >= 0) {
                        for (i = 0; i <= 3; i++) {
                            light_amb0[i] += 0.01;
                            light_dif0[i] += 0.01;
                            light_spc0[i] += 0.01;
                            //printf("2 %d\n", aCeilingLight);
                        }
                    }

            }
            break;
        case 't':
            if (cll >= 0)
            {
                cll -= 10;
                if (aRoomLight)
                    if (light_amb1[0] >= 0) {
                        for (i = 0; i <= 3; i++) {
                            light_amb0[i] -= 0.01;
                            light_dif0[i] -= 0.01;
                            light_spc0[i] -= 0.01;
                            //printf("2 %d\n", aCeilingLight);
                        }
                    }

            }
            break;
        case 'q':
            if (angle >= 360)
                angle = 0;
            angle += 10;
            break;
        case 'e':
            if (angle <= -360)
                angle = 0;
            angle -= 10;
            break;
        case 'w':
            chairws += 10;
            break;
        case 's':
            chairws -= 10;
            break;
        case'a':
            chairad += 10;
            break;
        case'd':
            chairad -= 10;
            break;
        case'z':
            if (cangle >= 360)
                cangle = 0;
            cangle += 10;
            break;
        case'c':
            if (cangle >= 360)
                cangle = 0;
            cangle -= 10;
            break;
    }
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
void timeFunc(int value) {
    myDisplay();
    // Present frame every 40 ms
    glutTimerFunc(40, timeFunc, 0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //初始化窗口的显示模式

    glutInitWindowPosition(100, 100);                //设置窗口的位置
    glutInitWindowSize(800, 800);                   //设置窗口的大小
    glutCreateWindow("第一个OpenGL程序");       //创建窗口

    Initial();                                     //完成窗口初始化
    picture = load_texture("picture1.bmp");  //加载纹理
    chuanghu = load_texture("chuangwai.bmp");  //加载纹理
    dianshi = load_texture("dianshi.bmp");  //加载纹理
    //picture = load_texture("D:\\桌面\\南航智能图形学\\hf3\\ground2.bmp");  //加载纹理
    //texWall = load_texture("C:\\Users\\hf\\source\\repos\\hf3\\wall2.bmp");
    glutReshapeFunc(reshape);                   //窗口改变的时候调用的函数，在这个里面的函数可以根据缩放后的窗口重新设置
    glutDisplayFunc(&myDisplay);                  //设置当前窗口的显示回调函数
    glutTimerFunc(40, timeFunc, 0);
    glutKeyboardFunc(keyboard);                 //键盘控制
    glutMouseFunc(mouseCB);                      //鼠标键位响应
    glutMotionFunc(mouseMotionCB);                    // 鼠标键摁下时拖动的事件

    glutMainLoop();                              //启动主 GLUT事件处理循环
    return 0;
}