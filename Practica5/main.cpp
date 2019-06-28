#define GLUT_DISABLE_ATEXIT_HACK
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glut.h>
#include <GL/glext.h>

#include <FreeImage.h>


using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0;
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -1200.0;

///posisicon de la iluminacion
GLfloat position[] = { 0.0, -150.0 , 0.0 , 1.0 };

///Normal Roof
    vector<float> NormalFrontRoof;
    vector<float> NormalBackRoof;

///Texture
GLint textureMesh;

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



int str_to_int(string s)
{
    string str = s;
    istringstream buffer(str);
    int value;
    buffer >> value;
    return value;
}

typedef struct /// Definicion de un punto del terreno
{
    GLfloat s, t; /// Coordenadas de texturas dividir entre 101
    GLfloat nx, ny, nz; /// Coordenadas de la normal
    GLfloat x, y, z; /// posicion del vertice
} PuntoTerreno;


class Terreno
{
    public :
        void load(string filename) ; /// carga de un archivo de modelo digital de terreno
        void display(); /// Visualizacion del terreno
        void computeNormals(); /// calcula las normales de cada vertice
    private:
        int nb_pt_x, nb_pt_z; /// cantidad de punto en X y Z
        float dist_x, dist_z; /// distancia entre dos puntos segun X y Z
        int coord_x, coord_y;///coordenada lambert

    GLfloat Modulo(GLfloat N[]);

    GLvoid Normaliza(GLfloat []);
    vector<GLuint> lista_indices;
    vector<GLfloat> lista_normales;
    vector<PuntoTerreno> lista_puntos;
    void calcularNormal();


};

Terreno terreno;

void Terreno::display()
{
    //glutSolidCube(4);
    glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(PuntoTerreno),&lista_puntos[0].s);
    glDrawElements(GL_TRIANGLES,100*100*6, GL_UNSIGNED_INT,(void*)&lista_indices[0]);
}

void Terreno::load(string filename)
{
    ifstream input(filename);
    string input_text;
    int n_line=0;

    int cont = 0;
    int cont_x = -2525,cont_z = 2525;
    float cont_s = 0.0, cont_t = 0.0;


    getline(input,input_text);
    coord_x = 790000;
    coord_y = 165000;
    dist_x  = 50.0;
    dist_z  = -50.0;
    nb_pt_x = 101;
    nb_pt_z = 101;
    while (getline(input, input_text))
    {
      istringstream iss(input_text);
      string word;
      while(iss>>word)
      {
          if(cont==nb_pt_x)
          {
              cont_z += dist_z;
              cont_t += 0.0099;
              cont_s = 0.0;
              cont_x = -2525;
              cont = 0;

          }

          PuntoTerreno point;
          point.x = cont_x;
          point.y = str_to_int(word)/**2*/;
          point.z = cont_z;
          point.s = cont_s;
          point.t = cont_t;
          lista_puntos.push_back(point);
          cont+=1;
          cont_x+=dist_x;//distancia x
          cont_s+=1.0f/101.0f;
        }
    }
    input.close();
    int r=0;
    lista_indices.reserve(100*100*6);
    for(int i=0; i<nb_pt_z-1; i++)
    {
        ///triangulos
        for(int j=0; j<nb_pt_x-1; j++)
        {
            ///primer triangulo
            lista_indices[r]=(i+1)*nb_pt_z+(j+1);
            r++;
            lista_indices[r]=(i+1)*nb_pt_z+j;
            r++;
            lista_indices[r]=i*nb_pt_z+j;
            r++;

            ///segundo triangulo
            lista_indices[r]=i*nb_pt_z+(j+1);
            r++;
            lista_indices[r]=(i+1)*nb_pt_z+(j+1);
            r++;
            lista_indices[r]=i*nb_pt_z+j;
            r++;
        }
    }

}


void Terreno::computeNormals()
{
    GLfloat V1x;
    GLfloat V1y;
    GLfloat V1z;

    GLfloat V2x;
    GLfloat V2y;
    GLfloat V2z;
    int cnt=0;
    for(int i=0;i<nb_pt_z;i++)
    {
        for(int j=0;j<nb_pt_x;j++)
        {
            int posicion=i*nb_pt_z+j;
            if(j+1<nb_pt_x && i+1<nb_pt_z)
            {
                V1x=lista_puntos[posicion+1].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion+1].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion+1].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion+nb_pt_x+1].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion+nb_pt_x+1].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion+nb_pt_x+1].z-lista_puntos[posicion].z;

                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;

                V1x=lista_puntos[posicion+nb_pt_x].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion+nb_pt_x].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion+nb_pt_x].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion+nb_pt_x+1].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion+nb_pt_x+1].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion+nb_pt_x+1].z-lista_puntos[posicion].z;

                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;
                cnt+=2;
            }
            if(j-1>=0 && i+1<nb_pt_z)
            {
                V1x=lista_puntos[posicion-1].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion-1].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion-1].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion+nb_pt_x].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion+nb_pt_x].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion+nb_pt_x].z-lista_puntos[posicion].z;
                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;
                cnt+=1;
            }
            if(j-1>=0 && i-1>=0)
            {
                V1x=lista_puntos[posicion-1].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion-1].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion-1].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion-nb_pt_x-1].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion-nb_pt_x-1].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion-nb_pt_x-1].z-lista_puntos[posicion].z;
                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;

                V1x=lista_puntos[posicion-nb_pt_x].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion-nb_pt_x].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion-nb_pt_x].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion-nb_pt_x-1].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion-nb_pt_x-1].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion-nb_pt_x-1].z-lista_puntos[posicion].z;
                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;
                cnt+=2;
            }
            if(j+1<nb_pt_x && i-1>=0)
            {
                V1x=lista_puntos[posicion+1].x-lista_puntos[posicion].x;
                V1y=lista_puntos[posicion+1].y-lista_puntos[posicion].y;
                V1z=lista_puntos[posicion+1].z-lista_puntos[posicion].z;

                V2x=lista_puntos[posicion-nb_pt_x].x-lista_puntos[posicion].x;
                V2y=lista_puntos[posicion-nb_pt_x].y-lista_puntos[posicion].y;
                V2z=lista_puntos[posicion-nb_pt_x].z-lista_puntos[posicion].z;
                lista_puntos[posicion].nx=V1y*V2z-V1z*V2y;
                lista_puntos[posicion].ny=V1z*V2x-V1x*V2z;
                lista_puntos[posicion].nz=V1x*V2y-V1y*V2x;
                cnt+=1;
            }
            lista_puntos[posicion].nx/=cnt;
            lista_puntos[posicion].ny/=cnt;
            lista_puntos[posicion].nz/=cnt;

            float X=lista_puntos[posicion].nx;
            float Y=lista_puntos[posicion].ny;
            float Z=lista_puntos[posicion].nz;
            double distancia=sqrt(X*X+Y*Y+Z*Z);
            lista_puntos[posicion].nx/=distancia;
            lista_puntos[posicion].ny/=distancia;
            lista_puntos[posicion].nz/=distancia;
        }
    }
}


vector<float> getNormal(vector<float> Vec1, vector<float> Vec2, vector<float> Vec3)
{
   vector<float> Normal;
   float Qx, Qy, Qz, Px, Py, Pz, vecUnit;

   Qx = Vec1[0]-Vec2[0];
   Qy = Vec1[1]-Vec2[1];
   Qz = Vec1[2]-Vec2[2];

   Px = Vec3[0]-Vec2[0];
   Py = Vec3[1]-Vec2[1];
   Pz = Vec3[2]-Vec2[2];

   Normal.push_back((Py*Qz) - (Pz*Qy));
   Normal.push_back((Pz*Qx) - (Px*Qz));
   Normal.push_back((Px*Qy) - (Py*Qx));

   vecUnit = sqrt(pow(Normal[0],2) +  pow(Normal[1],2)+ pow(Normal[2],2));
   Normal[0] = Normal[0] / vecUnit;
   Normal[1] = Normal[1] / vecUnit;
   Normal[2] = Normal[2] / vecUnit;

   return Normal;
}


///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 25;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 25;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 25;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 25;
		glutPostRedisplay();
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
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{

	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();




GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glClearColor(RED, GREEN, BLUE, ALPHA);

	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la correcci�n de perspectiva

    GLfloat mat_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat mat_defused[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat lightIntensity[] = {5,5,5,5};
    //GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

}


void Gizmo3D(){


	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();



	return;

}



GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(100.0f, 1.0f, 0.0f, 1.0f);


	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


        ///Light
        glLightfv (GL_LIGHT0, GL_POSITION, position);
        glPushMatrix();
            glTranslated(position[0],position[1],position[2]);
            glutSolidCube(0.1);
        glPopMatrix();

    glTranslatef(var_x, -700, var_z);
    terreno.display();
	//Gizmo3D();




	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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
    case 'w':
        position[2]-=10;
        break;
    case 's':
        position[2]+=10;
        break;
    case 'a':
        position[0]-=10;
        break;
    case 'd':
        position[0]+=10;
        break;
	case 'q':
        position[1]-=10;
        break;
	case 'e':
        position[1]+=10;
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




int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 5");


	initGL();
	init_scene();

    textureMesh = LoadTexture("fontvieille.tga", GL_BGR_EXT, GL_RGB);
    terreno.load("fontvieille.txt");
    terreno.computeNormals();

	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);


	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}
