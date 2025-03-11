//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations // it also includes gl.h and glu.h for the openGL library calls           
#include <math.h>
#include <string>
#include <GL/freeglut.h>

#define PI 3.1415926535898 

double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation 
//int SPEED = 50;        // speed of timer call back in msecs

// Tamanio de pantalla y tasa de refrescamiento
int width = 1024; // 500
int height = 576; // 200
int interval = 16; 

// Puntacion
int scoreLeft = 0;
int scoreRight = 0;

// Tamanio y velocidad raquetas
int raqW = 20;
int raqH = 150;
int raqSp = 4;

// Posicion raqueta izquierda
float raqLeftX = 10.0f;
float raqLeftY = 240.0f;

// Posicion raqueta derecha
float raqRightX = width - raqW - 10;
float raqRightY = 240.0f;


// Matrices de transformación

GLfloat T1[16] = {1.,0.,0.,0.,\
                  0.,1.,0.,0.,\
                  0.,0.,1.,0.,\
                  0.,0.,0.,1.};
GLfloat S[16] = {1.,0.,0.,0.,\
                 0.,1.,0.,0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};


/*
GLint circle_points = 100; 


// Draw the circle
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius){
  GLint i;
  GLdouble angle;
  glBegin(GL_POLYGON); 
  for (i = 0; i < circle_points; i++) {    
    angle = 2*PI*i/circle_points; 
    glVertex2f(centerx+radius*cos(angle), centery+radius*sin(angle)); 
  } 
  glEnd();
}

GLfloat RadiusOfBall = 15.;
// Draw the ball, centered at the origin


void draw_ball() {
  glColor3f(0.6,0.3,0.);
  MyCircle2f(0.,0.,RadiusOfBall);
  
}
*/




void reshape (int width, int height){
   // on reshape and on startup, keep the viewport to be the entire size of the window
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   // keep our logical coordinate system constant
   gluOrtho2D(0.0, width, 0.0, height);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
}


void displayText(float x, float y, std::string text) {
  glRasterPos2f(x, y);
  for (size_t i = 0; i < text.size(); ++i) {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
  }
}

void displayRaq(float x, float y, float width, float height){
  glBegin (GL_QUADS);
  glVertex2f(x,y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();
}


void keyPressLeft(unsigned char key, int x, int y) {
  switch (key) {
      case 'w':
          raqLeftY += raqSp;
          break;
      case 's':
          raqLeftY -= raqSp;
          break;
  }
}

void keyPressRight(int key, int x, int y) {
  switch (key) {
      case GLUT_KEY_UP:
          raqRightY += raqSp;
          break;
      case GLUT_KEY_DOWN:
          raqRightY -= raqSp;
          break;
  }
}

void keyboard(){
  glutKeyboardFunc(keyPressLeft);
  glutSpecialFunc(keyPressRight);
}



void update (int value){
  keyboard();
  glutTimerFunc(interval, update, 0); // llama update() otra vez en "intervalo" de milisegundos
  glutPostRedisplay(); 
}


/**
 * La funcion Display arroja todo lo de la pantalla para ver algo
 */
void display(void)
{

  //clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Raquetas
  displayRaq(raqLeftX, raqLeftY, raqW, raqH);
  displayRaq(raqRightX, raqRightY, raqW, raqH);

  // Marcador
  displayText(width / 2 - 10, height - 15, std::to_string(scoreLeft) + ":" + std::to_string(scoreRight));

  // swap the buffers
  glutSwapBuffers(); 

/*
  // 160 is max X value in our world
	// Define X position of the ball to be at center of window
	xpos = 80.;
 	
	// Shape has hit the ground! Stop moving and start squashing down and then back up 
	if (ypos == RadiusOfBall && ydir == -1  ) { 
		sy = sy*squash ; 
		
		if (sy < 0.8)
			// reached maximum suqash, now unsquash back up 
			squash = 1.1;
		else if (sy > 1.) {
			// reset squash parameters and bounce ball back upwards
			sy = 1.;
			squash = 0.9;
			ydir = 1;
		}
		sx = 1./sy;
	} 
	// 120 is max Y value in our world
	// set Y position to increment 1.5 times the direction of the bounce
	else {
	ypos = ypos+ydir *1.5 - (1.-sy)*RadiusOfBall;
	// If ball touches the top, change direction of ball downwards
  	if (ypos == 120-RadiusOfBall)
    	ydir = -1;
	// If ball touches the bottom, change direction of ball upwards
  	else if (ypos <RadiusOfBall)
		ydir = 1;
	}
*/


/* 
 //reset transformation state 
  glLoadIdentity();
  
  // apply translation
  glTranslatef(xpos,ypos, 0.);

  // Translate ball back to center
  glTranslatef(0.,-RadiusOfBall, 0.);
  // Scale the ball about its bottom
  glScalef(sx,sy, 1.);
  // Translate ball up so bottom is at the origin
  glTranslatef(0.,RadiusOfBall, 0.);
  // draw the ball
  draw_ball();
*/
 /*
  //Translate the bouncing ball to its new position
  T[12]= xpos;
  T[13] = ypos;
  glLoadMatrixf(T);

  T1[13] = -RadiusOfBall;
  // Translate ball back to center
  glMultMatrixf(T1);
  S[0] = sx;
  S[5] = sy;
  // Scale the ball about its bottom
  glMultMatrixf(S);
  T1[13] = RadiusOfBall;
  // Translate ball up so bottom is at the origin
  glMultMatrixf(T1);
  
  draw_ball();
  glutPostRedisplay(); 
  */
}






/*
void init(void){
  //set the clear color to be white
  glClearColor(0.0,0.8,0.0,1.0);
  // initial position set to 0,0
  xpos = 60; ypos = RadiusOfBall; xdir = 1; ydir = 1;
  sx = 1.; sy = 1.; squash = 0.9;
  rot = 0; 

}

*/




/**
 * @brief Función principal del programa.
 * @return 1 si la ejecución es exitosa.
 */


int main(int argc, char* argv[])
{

  glutInit(& argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);   
  glutCreateWindow("Pong - Atari");
  

  //init();
  glutDisplayFunc(display);
  glutTimerFunc(interval, update, 0);

  
  glutReshapeFunc(reshape);
  glColor3f(1.0f, 1.0f, 1.0f);
  glutMainLoop();

  return 1;
}