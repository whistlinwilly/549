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
 
    glOrtho(0, 1280, 800, 0, 3000, -3000);
 
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


void Projector::renderDisk(){
   
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(20.5f,1.6f,0.1f,100.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);                         // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

//glRotatef(90.0, 0, 0, 1);

glTranslatef(0.0f,0.0f,-26.3f);   

glRotatef(-73.0, 1, 0, 0);

glBegin(GL_QUADS);                      // Draw A Quad
        glVertex3f(-1.75, 1.12f, 0.0f);              // Top Left
        glVertex3f( 1.75f, 1.12f, 0.0f);              // Top Right
        glVertex3f( 1.75f,-1.12f, 0.0f);              // Bottom Right
        glVertex3f(-1.75f,-1.12f, 0.0f);              // Bottom Left
    glEnd();

glBegin(GL_QUADS);                      // Draw A Quad
        glVertex3f(-1.70, 1.10f, 1.0f);              // Top Left
        glVertex3f( 1.70f, 1.10f, 1.0f);              // Top Right
        glVertex3f( 1.70f,-1.10f, 1.0f);              // Bottom Right
        glVertex3f(-1.70f,-1.10f, 1.0f);              // Bottom Left
    glEnd();
	

    SDL_GL_SwapBuffers();
}

void Projector::renderInitPattern(){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);                         // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
glColor3f(1.0f,0.0f,0.0f);

glTranslatef(0.0,0.0, -1.0 * DEFAULT_DISTANCE);

//glTranslatef(0.0,0.0,-52.5);
//glRotatef(-68.5,1,0,0);
//glRotatef(44.0,0,0,1);
//glTranslatef(0.255f, -1.95f, 0.0f);

		glPushMatrix();
		
		
	//	glRotatef(100.8,0,0,1);
	//	glTranslatef(0.0,0.0,90.0);
		glRectf(-640.0f,0.0f, 0.0f, 400.0f);
		glRectf(0.0f,-400.0f, 640.0f, 0.0f);
		glPopMatrix();

		glColor3f(0.0f, 0.75f, 1.0f);

		float x1,y1,x2,y2;
		float angle;
		double radius=1.0;
 
		x1 = 0.0,y1=0.0;

		glPushMatrix();
	//	glTranslatef(10.0,10.0,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_LINE_STRIP);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex2f(x2,y2);
	}
 
		glEnd();
		glPopMatrix();

//glColor3f(0.0f,1.0f,0.0f);
//		glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.75, 1.12f, -1.0f);              // Top Left
//        glVertex3f( 1.75f, 1.12f, -1.0f);              // Top Right
//        glVertex3f( 1.75f,-1.12f, -1.0f);              // Bottom Right
//        glVertex3f(-1.75f,-1.12f, -1.0f);              // Bottom Left
//    glEnd();
//
//glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.70, 1.10f, 1.0f);              // Top Left
//        glVertex3f( 1.70f, 1.10f, 1.0f);              // Top Right
//        glVertex3f( 1.70f,-1.10f, 1.0f);              // Bottom Right
//        glVertex3f(-1.70f,-1.10f, 1.0f);              // Bottom Left
//    glEnd();


	

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

void Projector::renderPatternWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);                         // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
glColor3f(1.0f,1.0f,1.0f);

//glTranslatef(0.0,0.0,-52.5);
//glRotatef(-68.5,1,0,0);
//glRotatef(44.0,0,0,1);
//glTranslatef(0.255f, -1.95f, 0.0f);

glTranslatef(0.0,0.0,distanceFromTable);
glRotatef(incidentAngle,1,0,0);
glRotatef(projRotation,0,0,1);
glTranslatef(deltaX, deltaY, 0.0f);

	//	glPushMatrix();
	//	
	//	
	////	glRotatef(100.8,0,0,1);
	////	glTranslatef(0.0,0.0,90.0);
	//	glRectf(-640.0f,0.0f, 0.0f, 400.0f);
	//	glRectf(0.0f,-400.0f, 640.0f, 0.0f);
	//	glPopMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);

		float x1,y1,x2,y2;
		float angle;
		double radius=1.656;
		//double radius = 2.12;
		x1 = 0.0,y1=0.0;

		glPushMatrix();
	//	glTranslatef(10.0,10.0,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
	}
 
		glEnd();
		glPopMatrix();

				glPushMatrix();
	//	glTranslatef(10.0,10.0,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);


		//NOTENOTENOTENOTE
		//RADIUS CHANGES HERE
		//radius = 2.12;
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,2.0);
	}
 
		glEnd();
		glPopMatrix();
//
//glColor3f(0.0f,1.0f,0.0f);
//		glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.75, 1.12f, 0.0f);              // Top Left
//        glVertex3f( 1.75f, 1.12f, 0.0f);              // Top Right
//        glVertex3f( 1.75f,-1.12f, 0.0f);              // Bottom Right
//        glVertex3f(-1.75f,-1.12f, 0.0f);              // Bottom Left
//    glEnd();
//
//glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.70, 1.10f, 1.0f);              // Top Left
//        glVertex3f( 1.70f, 1.10f, 1.0f);              // Top Right
//        glVertex3f( 1.70f,-1.10f, 1.0f);              // Bottom Right
//        glVertex3f(-1.70f,-1.10f, 1.0f);              // Bottom Left
//    glEnd();


	

    SDL_GL_SwapBuffers();
}













void Projector::renderInitWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);                         // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
glColor3f(1.0f,1.0f,1.0f);

//glTranslatef(0.0,0.0,-52.5);
//glRotatef(-68.5,1,0,0);
//glRotatef(44.0,0,0,1);
//glTranslatef(0.255f, -1.95f, 0.0f);

glTranslatef(0.0,0.0,distanceFromTable);
glRotatef(incidentAngle,1,0,0);
glRotatef(projRotation,0,0,1);
glTranslatef(deltaX, deltaY, 0.0f);

glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		
	//	glRotatef(100.8,0,0,1);
	//	glTranslatef(0.0,0.0,90.0);
		glRectf(-640.0f,0.0f, 0.0f, 400.0f);
		glRectf(0.0f,-400.0f, 640.0f, 0.0f);
		glPopMatrix();

		glColor3f(0.0f, 0.75f, 1.0f);

		float x1,y1,x2,y2;
		float angle;
		double radius=1.0;
 
		x1 = 0.0,y1=0.0;

		glPushMatrix();
	//	glTranslatef(10.0,10.0,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_LINE_STRIP);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex2f(x2,y2);
	}
 
		glEnd();
		glPopMatrix();

//glColor3f(0.0f,1.0f,0.0f);
//		glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.75, 1.12f, -1.0f);              // Top Left
//        glVertex3f( 1.75f, 1.12f, -1.0f);              // Top Right
//        glVertex3f( 1.75f,-1.12f, -1.0f);              // Bottom Right
//        glVertex3f(-1.75f,-1.12f, -1.0f);              // Bottom Left
//    glEnd();
//
//glBegin(GL_QUADS);                      // Draw A Quad
//        glVertex3f(-1.70, 1.10f, 1.0f);              // Top Left
//        glVertex3f( 1.70f, 1.10f, 1.0f);              // Top Right
//        glVertex3f( 1.70f,-1.10f, 1.0f);              // Bottom Right
//        glVertex3f(-1.70f,-1.10f, 1.0f);              // Bottom Left
//    glEnd();


	

    SDL_GL_SwapBuffers();
}















void Projector::renderInitPattern2(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float xDist, float yDist){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
    glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);                         // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
glColor3f(1.0f,0.0f,0.0f);

//glTranslatef(0.0,0.0,-52.5);
//glRotatef(-68.5,1,0,0);
//glRotatef(44.0,0,0,1);
//glTranslatef(0.255f, -1.95f, 0.0f);

glTranslatef(0.0,0.0,distanceFromTable);
glRotatef(incidentAngle,1,0,0);
glRotatef(projRotation,0,0,1);
glTranslatef(deltaX, deltaY, 0.0f);

	//	glPushMatrix();
	//	
	//	
	////	glRotatef(100.8,0,0,1);
	////	glTranslatef(0.0,0.0,90.0);
	//	glRectf(-640.0f,0.0f, 0.0f, 400.0f);
	//	glRectf(0.0f,-400.0f, 640.0f, 0.0f);
	//	glPopMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);

		float x1,y1,x2,y2;
		float angle;
		double radius=1.0;
 
		x1 = 0.0,y1=0.0;

		glPushMatrix();
		glTranslatef(-xDist,-yDist,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
	}
 
		glEnd();
		glPopMatrix();

				glPushMatrix();
		glTranslatef(-xDist,yDist,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
	}
 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(xDist,-yDist,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
	}
 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(xDist,yDist,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
	}
 
		glEnd();
		glPopMatrix();

		
    SDL_GL_SwapBuffers();
}
