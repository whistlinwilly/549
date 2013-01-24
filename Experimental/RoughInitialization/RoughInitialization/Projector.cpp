#include "Projector.h"
#include <opencv\cv.h>

Projector::Projector(){
}

void Projector::init(void){
	 SDL_Surface *screen;
    SDL_Event event;
  
    int keypress = 0; 
    int h=0; 
  
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return;

	SDL_putenv("SDL_VIDEO_WINDOW_POS=1920,0");

	  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
 
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,            32);
 
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
 
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
   
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)))
    {
        SDL_Quit();
        return;
    }

	glClearColor(0, 0, 0, 0);
 
    glViewport(0, 0, 1280, 800);
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    glOrtho(0, 1280, 800, 0, 1, -1000);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
 
    glLoadIdentity();
  
  //  while(!keypress) 
  //  {
		////RENDER LOOP
  //    
  //       while(SDL_PollEvent(&event)) 
  //       {      
  //            switch (event.type) 
  //            {
  //                case SDL_QUIT:
	 //             keypress = 1;
	 //             break;
  //                case SDL_KEYDOWN:
  //                     keypress = 1;
  //                     break;
  //            }
  //       }
  //  }

   // SDL_Quit();
}

void Projector::renderFrame(cv::Point2f point){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
 
	if(point.x == 0){
    //glBegin(GL_QUADS);
    //    glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
    //    glColor3f(1, 1, 0); glVertex3f(1280, 0, 0);
    //    glColor3f(1, 0, 1); glVertex3f(1280, 800, 0);
    //    glColor3f(1, 1, 1); glVertex3f(0, 800, 0);
    //glEnd();
	}
	else{

		 GLUquadric *sphere=gluNewQuadric();
    gluQuadricDrawStyle( sphere, GLU_FILL);
    gluQuadricNormals( sphere, GLU_SMOOTH);
    gluQuadricOrientation( sphere, GLU_OUTSIDE);
    gluQuadricTexture( sphere, GL_TRUE);

		glPushMatrix();
		glTranslatef(point.x,point.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();
	}

    SDL_GL_SwapBuffers();
}


void Projector::renderDisk(cv::Point2f point, cv::Mat transform){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    glOrtho(0, 1280, 800, 0, 1, -1000);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
 
    glLoadIdentity();

		 GLUquadric *sphere=gluNewQuadric();
    gluQuadricDrawStyle( sphere, GLU_FILL);
    gluQuadricNormals( sphere, GLU_SMOOTH);
    gluQuadricOrientation( sphere, GLU_OUTSIDE);
    gluQuadricTexture( sphere, GL_TRUE);

		glPushMatrix();
		glTranslatef(point.x,point.y,0.0);
		gluDisk(sphere,30.0f,50.0f,32,32);
		glPopMatrix();
	

    SDL_GL_SwapBuffers();
}

void Projector::renderStar(cv::Point2f point0, cv::Point2f point1, cv::Point2f point2, cv::Point2f point3, cv::Point2f point4, cv::Point2f point5, cv::Point2f point6, cv::Point2f point7, cv::Point2f point8){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

		 GLUquadric *sphere=gluNewQuadric();
    gluQuadricDrawStyle( sphere, GLU_FILL);
    gluQuadricNormals( sphere, GLU_SMOOTH);
    gluQuadricOrientation( sphere, GLU_OUTSIDE);
    gluQuadricTexture( sphere, GL_TRUE);

		glPushMatrix();
		glTranslatef(point0.x,point0.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point1.x,point1.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point2.x,point2.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point3.x,point3.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point4.x,point4.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(point5.x,point5.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(point6.x,point6.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point7.x,point7.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

				glPushMatrix();
		glTranslatef(point8.x,point8.y,0.0);
		gluSphere(sphere,2,200,200);
		glPopMatrix();

    SDL_GL_SwapBuffers();
}

void Projector::destroy(void){
	SDL_Quit();
}

