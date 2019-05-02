#define GLUT_DISABLE_ATEXIT_HACK
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


///Variables de la camara
double CamPosX = 0;
double CamPosY = 20;
double CamPosZ = 40;

double CentX = 0;
double CentY = 0;
double CentZ = 0;

double AngX = 0;
double AngY = 1;
double AngZ = 0;

double AngRotX = 0;
double AngRotY = 0;

double temDTX = 0;
double temDTY = 0;

double velCam = 1;
double VelAngRot = 1;

///variables
float angle = 0;
float posXG = 0;

int cantpoints = 100;
double angulo = 360/cantpoints;
double alpha = 0;

float MAX = 8;
float MIN = -8;
bool Sentido = 0;

int timep=0;
int timepbase=0;
float dt;

///Variables sistema solar
double velGlobal = 5;
double velGlobalGiro = 5;
///sol
double tamSol = 4;
double velGiroSol = velGlobalGiro;

///tierra
double posTierraX;
double posTierraZ;
double tamTierra = 2;
double distDelSolTirra = 10;
double velT = velGlobal;
double velGiroTierra = 3 * velGiroSol;

///luna
double tamLuna = 1;
double distATierraLuna = 4;
double velL = 2 * velGlobal;
double velGiroLuna = 1.5 * velGiroSol;

///marte
double tamMarte = 1.5;
double distDelSolMarte = 18;
double velM = velT;
double velGiroMarte = velGiroSol;


//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(0, 0);
	glVertex2d(50, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 50);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0 , 50);
	glEnd();
}

//function called on each frame
GLvoid window_idle();


///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		CamPosZ = CamPosZ - velCam;
		CentZ = CentZ - velCam;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_DOWN:
	    CamPosZ = CamPosZ + velCam;
	    CentZ = CentZ + velCam;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_LEFT:
	    CamPosX = CamPosX - velCam;
	    CentX = CentX - velCam;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_RIGHT:
	    CamPosX = CamPosX + velCam;
	    CentX = CentX + velCam;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
	}
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
        temDTX = 0;
        temDTY = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
    AngRotX = x - temDTX;
    AngRotY = y - temDTY;
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	glutSpecialFunc(&callback_special);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

void displayTeapot(){
    glColor3f(1.0,1.0,1.0);
	glutSolidTeapot(2);

}


void displaySphere(){
    glColor3f(0.988,0.88,0.44);
    glutSolidSphere(1,100,100);
}

void displayTorus(){
    glColor3f(1.0,1.0,1.0);
    glutSolidTorus(0.5,1,100,100);
}

void displayCubo(){
    glColor3f(1.0,1.0,1.0);
    glutSolidCube(2);
}

void rotarCubo(){
    glPushMatrix();
    glRotated(angle,1.0f,0.0f,0.0f);
    glTranslated(0,6,0);
    displayCubo();
    glPopMatrix();
}

void rotarTorus(){
    glRotated(angle*3,0.0f,1.0f,0.0f);
    glTranslated(6,0,0);
    displayTorus();
}

void rotarTeapot(){

    double radio = 10;
    glPushMatrix();
    ///

    double x2 = radio*sin(alpha*3.1416/180);
    double y2 = radio*cos(alpha*3.1416/180);
    //glRotated(angle,0.0f,0.0f,1.0f);
    glTranslated(x2,y2,0);
    alpha = alpha + 6 * angulo * dt;
    ///
    displayTeapot();
    angle = angle + 100 * dt;
    rotarTorus();
    glPopMatrix();


}

void moveTeapot(){
   glPushMatrix();
   glTranslatef(posXG,0,0);
   //posXG = posXG +0.01;
   rotarTeapot();
   rotarCubo();
   displaySphere();
   if(Sentido == 0 ){
        if(posXG < MAX){
            posXG = posXG +10*dt;
        }else{
            Sentido = 1;
        }
   }else{
        if(posXG > MIN){
            posXG = posXG - 10*dt;
        }else{
            Sentido = 0;
        }
   }


   glPopMatrix();

}


void displaySol(){
    glColor3f(0.988,0.88,0.44);
    glutSolidSphere(tamSol,8,8);
}

void displayTierra(){
    glColor3f(0.372,0.692,0.904);
    glutSolidSphere(tamTierra,8,8);
}

void displayLuna(){
    glColor3f(0.764,0.804,0.808);
    glutSolidSphere(tamLuna,8,8);
}

void displayMarte(){
    glColor3f(0.764,0,0);
    glutSolidSphere(tamMarte,8,8);
}

void sobreEjeSol(){
    glPushMatrix();
    glRotatef(angle*velGiroSol,0.0,1.0,0.0);
    displaySol();
    glPopMatrix();
}

void sobreEjeTierra(){
    glPushMatrix();
    glRotatef(angle*velGiroTierra,0.0,1.0,0.0);
    displayTierra();
    glPopMatrix();
}

void sobreEjeLuna(){
    glPushMatrix();
    glRotatef(angle*velGiroLuna,0.0,1.0,0.0);
    displayLuna();
    glPopMatrix();
}

void sobreEjeMarte(){
    glPushMatrix();
    glRotatef(angle*velGiroMarte,0.0,1.0,0.0);
    displayMarte();
    glPopMatrix();
}

void rotarMarte(){
    glPushMatrix();

    double x2 = distDelSolMarte*sin(alpha*3.1416/180);
    double y2 = distDelSolMarte*cos(alpha*3.1416/180);
    //glRotated(angle,0.0f,0.0f,1.0f);
    glTranslated(x2,0,y2);
    //alpha = alpha + velM* angulo * dt;

    //glRotatef(angle*velM,0.0f,1.0f,0.0f);
    //glTranslated(distDelSolMarte,0,0);
    sobreEjeMarte();
    glPopMatrix();
}

void rotarLuna(){
    glRotated(angle*velL,0.0f,1.0f,0.0f);
    glTranslated(distATierraLuna,0,0);
    sobreEjeLuna();
}

void rotarTierra(){

    glPushMatrix();

    posTierraX = distDelSolTirra*sin(alpha*3.1416/180);
    posTierraZ = distDelSolTirra*cos(alpha*3.1416/180);
    //glRotated(angle,0.0f,0.0f,1.0f);
    glTranslated(posTierraX,0,posTierraZ);
    alpha = alpha + velT* angulo * dt;

    //glRotated(angle*velT,0.0f,1.0f,0.0f);
    //glTranslated(distDelSolTirra,0,0);

    sobreEjeTierra();
    angle = angle + velT * dt;
    rotarLuna();
    glPopMatrix();


}



void DisplaySistemaSolar(){

    rotarTierra();
    rotarMarte();
    sobreEjeSol();
}

void RotateCamera(){
    glRotated(AngRotY,1.0f,0.0f,0.0f);
    glRotated(AngRotX,0.0f,1.0f,0.0f);


}
GLvoid window_display()
{
    timep = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
    dt = float(timep -timepbase)/1000.0;// delta timep
    timepbase = timep;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
    gluPerspective(45,1,0.1,100);
	//glTranslated(0,0,-30);

    ///Mirar la Tierra
	//gluLookAt(0,20,40,posTierraX,0,posTierraZ,0,1,0);

    ///Mover la esecena
    gluLookAt(CamPosX,CamPosY,CamPosZ,CentX,CentY,CentZ,AngX,AngY,AngZ);

    RotateCamera();

	/*dibujar aqui*/

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //displayTeapot();
	//rotarTeapot();
	//displaySphere();
	//displayTorus();
	//moveTeapot();
	DisplaySistemaSolar();
	displayGizmo();



	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{


	glutPostRedisplay();
}
