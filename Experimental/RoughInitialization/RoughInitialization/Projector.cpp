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
		gluSphere(sphere,8,200,200);
		glPopMatrix();
	}

    SDL_GL_SwapBuffers();
}

void Projector::destroy(void){
	SDL_Quit();
}

