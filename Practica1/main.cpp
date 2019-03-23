#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>

#define KEY_ESC 27
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_H 104
#define KEY_V 118
#define KEY_T 116
#define KEY_C 99
#define KEY_A 97
#define KEY_R 114

using namespace std;

bool exc1,exc2,exc3,exc4,exc5;
int hCenter=0, vCenter=0, edgeSize=10, reductionPercentage= 30, quantity=5, angle=15;


void printInstructions()
{
    system("CLS");
    cout<<"### Computacion Grafica - Practica 1 ###"<<endl<<endl;
    cout<<"Mostrar/Ocultar ejercicios"<<endl;
    cout<<" 1. Cuadrado"<<endl;
    cout<<" 2. Circulo"<<endl;
    cout<<" 3. Circulos con reduccion"<<endl;
    cout<<" 4. Circulos con reduccion en linea"<<endl;
    cout<<" 5. Circulos con reduccion en angulo"<<endl<<endl;
    cout<<"Cambios por consola"<<endl;
    cout<<" h. Coordenada horizontal del centro"<<endl;
    cout<<" v. Coordenada vertical del centro"<<endl;
    cout<<" t. Tamano de arista/radio"<<endl;
    cout<<" c. Cantidad de figuras"<<endl;
    cout<<" r. Porcentaje de reduccion"<<endl;
    cout<<" a. Angulo de inclinacion"<<endl<<endl;
    cout<<"Valores actuales"<<endl;
    cout<<" Centro: ("<<hCenter<<","<<vCenter<<")"<<endl;
    cout<<" Tamano de arista/radio: "<<edgeSize<<endl;
    cout<<" Porcentaje de reduccion: " <<reductionPercentage<<"%"<<endl;
    cout<<" Cantidad: "<<quantity<<endl;
    cout<<" Angulo de inclinacion: "<<angle<<"°"<<endl;
}

void square(int x, int y, int tam)
{
    int aux=tam/2;
    glBegin(GL_LINE_LOOP);
    glColor3d(0,0,250);
    glVertex2d(x-aux,y+aux); //Esquina superior izquierda
    glVertex2d(x+aux,y+aux); //Esquina superior derecha
    glVertex2d(x+aux,y-aux); //Esquina inferior derecha
    glVertex2d(x-aux,y-aux); //Esquina inferior izquierda
    glEnd();
}

void circle(double Xpos, double Ypos, double radio)
{
    int cantPoints=300;
    double angulo=360/cantPoints;
    glBegin(GL_LINE_LOOP);
    glColor3d(0,0,250);

    for(double alpha = 0 ; alpha < 360; alpha =alpha + angulo){
        double x2 = radio*sin(alpha*3.1416/180)+Xpos;
        double y2 = radio*cos(alpha*3.1416/180)+Ypos;
        glVertex2f(x2,y2);
    }
    glEnd();
}

void reductCircles(int Xpos,int Ypos,int cantCircles, double reduction)
{
    double r = edgeSize;
    double newR;
    for(int i = 0 ; i < cantCircles ; i++){
        circle(Xpos, Ypos, r);
        newR = r-ceil(r*reduction/100);
        Xpos = Xpos-(r-newR);
        r = newR;
    }
}

void linedCircles(int Xpos, int Ypos, int cantCircles, double reduction)
{
    double r = edgeSize;
    double newR;
    for(int i = 0 ; i < cantCircles ; i++){
        circle(Xpos, Ypos, r);
        newR = r-ceil(r*reduction/100);
        Xpos = Xpos+(r+newR);
        r = newR;
    }
}

void angledCircles(float Xpos, float Ypos, int cantCircles, double reduction, double angulo)
{
    double r = edgeSize;
    double newR;
    double anguloRad = (90-angulo)*3.1416/180;

    for(int i = 0 ; i < cantCircles ; i++){
        circle(Xpos, Ypos, r);
        newR = r-ceil(r*reduction/100);
        Xpos = Xpos+((r+newR)*sin(anguloRad));
        Ypos = Ypos+((r+newR)*cos(anguloRad));
        r = newR;

    }
}
void displayGizmo()
{
	//glBegin(GL_LINE_LOOP);
	//cuadrado(-40,40,10);
	if(exc1)
    {
        square(hCenter,vCenter,edgeSize);
    }
    if(exc2)
    {
        circle(hCenter,vCenter,edgeSize);
    }
    if(exc3)
    {
        reductCircles(hCenter,vCenter,quantity,reductionPercentage);
    }
    if(exc4)
    {
        linedCircles(hCenter,vCenter,quantity,reductionPercentage);
    }
    if(exc5)
    {
        angledCircles(hCenter,vCenter,quantity,reductionPercentage,angle);
    }

	//glEnd();
}

//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();

	//dibuja el gizmo
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f);
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
    int aux;
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
    case KEY_1:
        exc1= !exc1;
        break;
    case KEY_2:
        exc2= !exc2;
        break;
    case KEY_3:
        exc3= !exc3;
        break;
    case KEY_4:
        exc4= !exc4;
        break;
    case KEY_5:
        exc5= !exc5;
        break;
    case KEY_H:
        system("CLS");
        cout<<"Nueva coordenada horizontal: ";
        cin>>aux;
        hCenter=aux;
        printInstructions();
        break;
    case KEY_V:
        system("CLS");
        cout<<"Nueva coordenada vertical: ";
        cin>>aux;
        vCenter=aux;
        printInstructions();
        break;
    case KEY_T:
        system("CLS");
        cout<<"Nuevo tamano de arista/radio: ";
        cin>>aux;
        edgeSize=aux;
        printInstructions();
        break;
    case KEY_C:
        system("CLS");
        cout<<"Nueva cantidad de figuras: ";
        cin>>aux;
        quantity=aux;
        printInstructions();
        break;
    case KEY_A:
        system("CLS");
        cout<<"Nuevo angulo de inclinacion: ";
        cin>>aux;
        angle=aux;
        printInstructions();
        break;
    case KEY_R:
        system("CLS");
        cout<<"Nuevo porcentaje de reduccion: ";
        cin>>aux;
        reductionPercentage=aux;
        printInstructions();
        break;

	default:
		break;
	}
	//glutPostRedisplay();

}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	printInstructions();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(500, 100); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutIdleFunc(&idle);

	glutMainLoop(); //bucle de rendering

	return 0;
}
