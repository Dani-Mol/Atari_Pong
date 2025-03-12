//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations it also includes gl.h and glu.h for the openGL library calls           
#include <math.h>
#include <string>
#include <GL/freeglut.h>

#define PI 3.1415926535898 

// Tamanio de pantalla y tasa de refresco
int width = 900, height = 500;
int interval = 16; 

// Puntacion
int scoreLeft = 0, scoreRight = 0;

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


// Estado de teclas
bool keyLeft[256] = {false};
bool keyRight[256] = {false};

// Datos de bola
double xpos = width / 2;
double ypos = height / 2; 
float ydir = -1.0f; 
float xdir = (rand() % 2 == 0) ? -1.0f : 1.0f;         // x and y position for house to be drawn
int ballSize = 8;
double sx = 1.0, sy = 1.0, squash = 0.8;          // xy scale factors
double rot, rdir;             // rotation 
float ballSp = 10.0f;        // ballSp of timer call back in msecs
float speedIncr = 0.2f;
float maxSp= 25.0f;
GLint circle_points = 100; 


/**
 * Dibujar circulo 
 */


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

/**
 * Configuracion  de GL
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

/**
 * Funcion para mostrar marcador
 */

void displayText(float x, float y, std::string text) {
  glRasterPos2f(x, y);
  for (size_t i = 0; i < text.size(); ++i) {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
  }
}


/**
 * Funcion para mostrar raquetas
 */

void displayRaq(float x, float y, float width, float height){
  glBegin (GL_QUADS);
  glVertex2f(x,y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();
}


// Funciones para entrada de las raquetas

void keyPressLeft(unsigned char key, int x, int y) { keyLeft[key] = true; }
void keyPressDropLeft(unsigned char key, int x, int y) { keyLeft[key] = false; }
void keyPressRight(int key, int x, int y) { keyRight[key] = true; }
void keyPressDropRight(int key, int x, int y) { keyRight[key] = false; }


/**
 * Configuracion de las telcas
 */

void keyboard(){
  glutKeyboardFunc(keyPressLeft);
  glutKeyboardUpFunc(keyPressDropLeft);
  glutSpecialFunc(keyPressRight);
  glutSpecialUpFunc(keyPressDropRight);
}


/**
 * Normalizacion de un vector (para la velocidad cte)
 */

void vec2_norm(float& x, float &y) {
  float length = sqrt((x * x) + (y * y));
  if (length != 0.0f) {
      length = 1.0f / length;
      x *= length;
      y *= length;
  }
}


/**
 * Funcion para actualizar la posición de la bola
 */
void updateBall() {
  xpos += xdir * ballSp;
  ypos += ydir * ballSp;

    // Rebote con la raqueta izquierda
    if (xpos < raqLeftX + raqW && xpos > raqLeftX && ypos < raqLeftY + raqH && ypos > raqLeftY) {
        float t = ((ypos - raqLeftY) / raqH) - 0.5f;
        xdir = fabs(xdir); 
        ydir = t * 1.5f + ((rand() % 10 - 5) / 20.0f);
        ballSp = fmin(ballSp + speedIncr, maxSp);

        
        sx = squash; // Squash horizontal al chocar con la raqueta
        sy = 1.2; // Se estira verticalmente
    }
   
    // Rebote con la raqueta derecha
    if (xpos > raqRightX && xpos < raqRightX + raqW && ypos < raqRightY + raqH && ypos > raqRightY) {
        float t = ((ypos - raqRightY) / raqH) - 0.5f;
        xdir = -fabs(xdir);
        ydir = t * 1.5f + ((rand() % 10 - 5) / 20.0f);
        ballSp = fmin(ballSp + speedIncr, maxSp);

        
        sx = squash; // Squash horizontal
        sy = 1.2; // Se estira verticalmente
    }

    // Si la bola toca la pared izquierda (punto para la derecha)
    if (xpos < 0) {
      ++scoreRight;  // Suma un punto para el jugador derecho
      xpos = width / 2;
      ypos = height / 2;
      xdir = fabs(xdir); // Asegura que se mueva hacia la derecha
      ydir = 0;
      sx = sy = 1.0;  // Reset de squash
      ballSp = 10.0f; // Reset de la velocidad 
    }

    // Si la bola toca la pared derecha (punto para la izquierda)
    if (xpos > width) {
        ++scoreLeft;  // Suma un punto para el jugador izquierdo
        xpos = width / 2;
        ypos = height / 2;
        xdir = -fabs(xdir); // Asegura que se mueva hacia la izquierda
        ydir = 0;
        sx = sy = 1.0;  // Reset de squash
        ballSp = 10.0f; // Reset de la velocidad 
    }

    // Rebote con la pared superior
    if (ypos > height) {
        ydir = -fabs(ydir);
        sx = 1.2;  // Se estira horizontalmente
        sy = squash;
    }

    // Rebote con la pared inferior
    if (ypos < 0) {
        ydir = fabs(ydir);
        sx = 1.2;
        sy = squash;
    }

    vec2_norm(xdir, ydir);


}


/**
 * Funcion que actualiza periodicamente
 */

void update (int value){
  if(keyLeft['w'] && raqLeftY + raqH < height) raqLeftY += raqSp;
  if(keyLeft['s'] && raqLeftY > 0) raqLeftY -= raqSp;

  if(keyRight[GLUT_KEY_UP] && raqRightY + raqH < height) raqRightY += raqSp;
  if(keyRight[GLUT_KEY_DOWN] && raqRightY > 0) raqRightY -= raqSp;

 
  updateBall();
  glutTimerFunc(interval, update, 0); 
  glutPostRedisplay(); 
}


/**
 * La funcion Display renderiza la pantalla
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

  // Bola 
  glPushMatrix();
  glTranslatef(xpos, ypos, 0);
  glScalef(sx, sy, 1.0);  // Aplicar squash/stretch
  MyCircle2f(0, 0, ballSize);
  glPopMatrix();

  // swap the buffers
  glutSwapBuffers(); 

}

/**
 * @brief Funcion principal del programa.
 * @return 1 si la ejecucion es exitosa.
 */

int main(int argc, char* argv[])
{

  glutInit(& argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);   
  glutCreateWindow("Pong - Atari");
  
  glutDisplayFunc(display);
  keyboard();
  glutTimerFunc(interval, update, 0);

  glutReshapeFunc(reshape);
  glColor3f(1.0f, 1.0f, 1.0f);
  glutMainLoop();

  return 1;
}