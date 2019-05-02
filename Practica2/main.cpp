#define GLUT_DISABLE_ATEXIT_HACK
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//g++ -o main main.cpp -lGL -lGLU -lglut -lpthread -lfreeimage

#include <GL/glut.h>
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
#define KEY_1 49
#define KEY_2 50


int timep=0;
int timebasep=0;

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

void printInstructions()
{
    system("CLS");
    cout<<"### Computacion Grafica - Practica 2 ###"<<endl<<endl;
    cout<<"Mostrar/Ocultar ejercicios"<<endl;
    cout<<" 1. Tetera"<<endl;
    cout<<" 2. Sistema solar"<<endl;
}
int main(int argc, char **argv)
{
    printInstructions();
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


float angle=0.0f, posx=-8.0f, desp=0.01f, giroVel=0.1f, solarAngle=0.0f, solarRot=0.0f;
bool exc1=0,exc2=0;

GLvoid window_display()
{
    timep = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
    float dt = float(timep -timebasep)/1000.0;// delta time
    timebasep = timep;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);



	/*dibujar aqui*/
	if (exc1)
    {
        glPushMatrix();
            glTranslatef(posx,0.0f,0.0f);
            glColor3f(0.0,0.0,1.0);
            glutSolidSphere(0.5f,100,100);
            glPushMatrix();
            glColor3f(1.0,0.0,0.0);
            glRotatef(angle,1.0f,0.0f,0.0f);
            glTranslatef(0.0f,3.0f,0.0f);
            glutSolidCube(2);
            glPopMatrix();
            glColor3f(1.0,1.0,1.0);
            glRotatef(angle,0.0f,0.0f,1.0f);
            glTranslatef(4.0f,0.0f,0.0f);
            glutSolidTeapot(1.5);
            angle+=0.1;
            posx+=desp;
            if(fabs(posx)>8)
            {
                desp*=-1;
            }
            glRotatef(angle*3,0.0f,1.0f,0.0f);
            glTranslatef(3.0f,0.0f,0.0f);
            glColor3f(0.0,1.0,0.0);
            glutSolidTorus(0.15,0.4,6,6);
        glPopMatrix();
    }
    if (exc2)
    {
        glPushMatrix();
            glColor3f(1.0,1.0,0);   ///Yellow
            glPushMatrix(); ///Sun Matrix
                glRotatef(solarRot,0,1,0);
                glutSolidSphere(4,8,8); ///Sun
            glPopMatrix();

            glPushMatrix(); ///Earth and moon matrix
                glRotatef(solarAngle,0,1,0);
                glTranslatef(10,0,0);
                glPushMatrix(); ///Earth matrix
                    glColor3f(0.372,0.692,0.904);
                    glRotatef(solarRot*3,0,1,0);
                    glutSolidSphere(2,8,8);  ///Earth
                glPopMatrix();
                glRotatef(solarAngle*2,0,1,1);
                glTranslatef(3,0,0);
                glPushMatrix(); ///Moon Matrix
                    glColor3f(0.764,0.804,0.808);
                    glRotatef(solarAngle*1.5,0,1,0);
                    glutSolidSphere(0.6,8,8);  ///Moon
                glPopMatrix();
            glPopMatrix();
            glRotatef(solarAngle,0,1,0);
            glTranslatef(18,0,0);
            glPushMatrix(); ///Mars matrix
                glColor3f(1.0,0,0);
                glRotatef(solarRot,0,1,0);
                glutSolidSphere(2,8,8);  ///Mars
            glPopMatrix();
        glPopMatrix();
        solarAngle+=25*dt;
        solarRot+=25*dt;
    }

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
    case KEY_1:
        exc1= !exc1;
        break;
    case KEY_2:
        exc2=!exc2;
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
