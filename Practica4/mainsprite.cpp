#define GLUT_DISABLE_ATEXIT_HACK
//#include <windows.h>
#include <math.h>
#include <iostream>


#include "GL/glut.h"
#include <FreeImage.h>
//#include "TextureManager.h"
#define KEY_ESC 27

using namespace std;


GLint sprites;
GLint texture;
int timep = 0;
int timepbase = 0;
int anim = 0;
int i = 0;
double x = 0.1678;
double y = 0.665;//factor de cambio de posicion en la textura
//para recuperar los sprites, no es una solucion muy precisa
//lo mejor seria de pre-calcular cual es la posicion del sprite en la textura
//


GLuint LoadTexture(const char* filename, GLenum image_format, GLint internal_format, GLint level=0, GLint border=0)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint textureID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &textureID);


	glBindTexture(GL_TEXTURE_2D, textureID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, bits);


	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



	//glGenerateMipmap(GL_TEXTURE_2D);
	FreeImage_Unload(dib);

	//return success
	return textureID;
}


//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	glEnable(GL_BLEND);//utilizar transparencia
	timep = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	int dt = timep -timepbase;// delta timep
	timepbase = timep;
	anim += dt;//duracion de la animacion entre dos cambios de Sprite

	if (anim / 1000.0 > 0.15)// si el tiempo de animacion dura mas 0.15s cambiamos de sprite
	{
		i++;
		anim = 0.0;
	}

	if (i == 6) i = 0;

	glBindTexture(GL_TEXTURE_2D, sprites);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f + x*i, y);//coordenadas de textura
	glVertex3d(-3, -5, 0);

	glTexCoord2f(0.0f + x*i, 1.0f);
	glVertex3d(-3, 5, 0);

	glTexCoord2f(x*(i + 1.0), 1.0f);
	glVertex3d(3, 5, 0);

	glTexCoord2f(x*(i + 1.0), y);
	glVertex3d(3, -5, 0);
	glEnd();

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);//coordenadas de textura
	glVertex3d(8, -2, 0);

	glTexCoord2f(0.0, 1.0f);
	glVertex3d(8, 2, 0);

	glTexCoord2f(1.0, 1.0f);
	glVertex3d(16, 2, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex3d(16, -2, 0);
	glEnd();


	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.4f, 0.4f, 0.4f, 0.2f); //(R, G, B, transparencia) en este caso un fondo negro
	glEnable(GL_TEXTURE_2D);
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Get a handle for our "myTextureSampler" uniform
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 50.0f, width / 50.0f, -height / 50.0f, height / 50.0f, -1.0f, 1.0f);

	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//function called on each frame
GLvoid window_idle()
{


	glutPostRedisplay();
}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Textura: Animacion"); //titulo de la ventana

	init_GL(); //fucnion de inicializacion de OpenGL

	sprites = LoadTexture("sprites.png", GL_BGRA_EXT, GL_RGBA);
	//GL_RGBA : en caso de una textura con transparencia sino GL_RGB
	//GL_BGRA_EXT o GL_BGR_EXT : utilizar en caso de inversion de colores sino GL_RGBA o GL_RGB
	texture = LoadTexture("background.png", GL_BGR_EXT, GL_RGB);
	cout << texture << endl;
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutIdleFunc(&window_idle);
	glutMainLoop(); //bucle de rendering

	return 0;
}
