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

	texture = new GLuint[10];

	
	texture[0] = createTexture("Tub1.bmp");
	texture[1] = createTexture("Tub2.bmp");
	texture[2] = createTexture("Tub3.bmp");
	texture[3] = createTexture("Tub4.bmp");
	texture[4] = createTexture("Tub5.bmp");
	texture[5] = createTexture("Tub6.bmp");
	texture[6] = createTexture("Tub7.bmp");
	texture[7] = createTexture("Tub8.bmp");
	texture[8] = createTexture("Tub9.bmp");
	texture[9] = createTexture("Tub10.bmp");
	
//SDL_Surface *surface;	// This surface will tell us the details of the image
//GLenum texture_format;
//GLint  nOfColors;
// 
//if ( (surface = SDL_LoadBMP("slody.bmp")) ) { 
// 
//	// Check that the image's width is a power of 2
//	if ( (surface->w & (surface->w - 1)) != 0 ) {
//		printf("warning: image.bmp's width is not a power of 2\n");
//	}
// 
//	// Also check if the height is a power of 2
//	if ( (surface->h & (surface->h - 1)) != 0 ) {
//		printf("warning: image.bmp's height is not a power of 2\n");
//	}
// 
//        // get the number of channels in the SDL surface
//        nOfColors = surface->format->BytesPerPixel;
//        if (nOfColors == 4)     // contains an alpha channel
//        {
//                if (surface->format->Rmask == 0x000000ff)
//                        texture_format = GL_RGBA;
//                else
//                        texture_format = GL_BGRA;
//        } else if (nOfColors == 3)     // no alpha channel
//        {
//                if (surface->format->Rmask == 0x000000ff)
//                        texture_format = GL_RGB;
//                else
//                        texture_format = GL_BGR;
//        } else {
//                printf("warning: the image is not truecolor..  this will probably break\n");
//                // this error should not go unhandled
//        }
// 
//	// Have OpenGL generate a texture object handle for us
//	glGenTextures( 1, &test1 );
// 
//	// Bind the texture object
//	glBindTexture( GL_TEXTURE_2D, test1 );
// 
//	// Set the texture's stretching properties
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// 
//	// Edit the texture object's image data using the information SDL_Surface gives us
//	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
//                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
//} 
//else {
//	printf("SDL could not load image.bmp: %s\n", SDL_GetError());
//	SDL_Quit();
//	return;
//}    
// 
//// Free the SDL_Surface only if it was successfully created
//if ( surface ) { 
//	SDL_FreeSurface( surface );
//}
//
//
///////////////////////////////FOR WALL TEXTURE
//
//
//if ( (surface = SDL_LoadBMP("stonewall.bmp")) ) { 
// 
//	// Check that the image's width is a power of 2
//	if ( (surface->w & (surface->w - 1)) != 0 ) {
//		printf("warning: image.bmp's width is not a power of 2\n");
//	}
// 
//	// Also check if the height is a power of 2
//	if ( (surface->h & (surface->h - 1)) != 0 ) {
//		printf("warning: image.bmp's height is not a power of 2\n");
//	}
// 
//        // get the number of channels in the SDL surface
//        nOfColors = surface->format->BytesPerPixel;
//        if (nOfColors == 4)     // contains an alpha channel
//        {
//                if (surface->format->Rmask == 0x000000ff)
//                        texture_format = GL_RGBA;
//                else
//                        texture_format = GL_BGRA;
//        } else if (nOfColors == 3)     // no alpha channel
//        {
//                if (surface->format->Rmask == 0x000000ff)
//                        texture_format = GL_RGB;
//                else
//                        texture_format = GL_BGR;
//        } else {
//                printf("warning: the image is not truecolor..  this will probably break\n");
//                // this error should not go unhandled
//        }
// 
//	// Have OpenGL generate a texture object handle for us
//	glGenTextures( 1, &test2 );
// 
//	// Bind the texture object
//	glBindTexture( GL_TEXTURE_2D, test2 );
// 
//	// Set the texture's stretching properties
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// 
//	// Edit the texture object's image data using the information SDL_Surface gives us
//	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
//                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
//} 
//else {
//	printf("SDL could not load image.bmp: %s\n", SDL_GetError());
//	SDL_Quit();
//	return;
//}    
// 
//// Free the SDL_Surface only if it was successfully created
//if ( surface ) { 
//	SDL_FreeSurface( surface );
//}



glDisable(GL_TEXTURE_2D);


	
}

GLuint Projector::createTexture(char* filename){
SDL_Surface *surface;	// This surface will tell us the details of the image
GLenum texture_format;
GLint  nOfColors;
GLuint texture;
 
if ( (surface = SDL_LoadBMP(filename)) ) { 
 
	// Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("warning: image.bmp's width is not a power of 2\n");
	}
 
	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("warning: image.bmp's height is not a power of 2\n");
	}
 
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
 
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &texture );
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture );
 
	// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
} 
else {
	printf("SDL could not load image.bmp: %s\n", SDL_GetError());
	SDL_Quit();
	exit(0);
}    
 
// Free the SDL_Surface only if it was successfully created
if ( surface ) { 
	SDL_FreeSurface( surface );
}
	return texture;
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
 
   // glEnable(GL_TEXTURE_2D);
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
 
 //   glEnable(GL_TEXTURE_2D);
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
		glBegin(GL_TRIANGLE_FAN);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2, 0.0);
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

void Projector::renderPatternWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float twist){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
  //  glEnable(GL_TEXTURE_2D);
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

glRotatef(twist,0,0,1);
glTranslatef(0.0,0.0,distanceFromTable);
glRotatef(projRotation,0,0,1);
glRotatef(incidentAngle,1,0,0);

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
		x1 = deltaX, y1=deltaY;

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
	    glVertex3f(x2,y2,1.9);
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













void Projector::renderInitWithPerspective(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float twist){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
 //   glEnable(GL_TEXTURE_2D);
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
glRotatef(projRotation,0,0,1);
glRotatef(incidentAngle,1,0,0);
glRotatef(twist,0,0,1);




glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		
	//	glRotatef(100.8,0,0,1);
	//	glTranslatef(0.0,0.0,90.0);
		glRectf(-640.0f,deltaY, deltaX, 400.0f);
		glRectf(deltaX,-400.0f, 640.0f, deltaY);
		glPopMatrix();

		glColor3f(0.0f, 0.75f, 1.0f);

		float x1,y1,x2,y2;
		float angle;
		double radius=4.0;
 
		x1 = deltaX,y1=deltaY;

		glPushMatrix();
	//	glTranslatef(10.0,10.0,0.0);
	//	glRotatef(-73.0,1,0,0);
		glBegin(GL_LINE_STRIP);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex3f(x2,y2,0.0);
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















void Projector::renderInitPattern2(float distanceFromTable, float incidentAngle, float projRotation, float deltaX, float deltaY, float xDist, float yDist, float twist){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
 
    glMatrixMode(GL_MODELVIEW);
 
 //   glEnable(GL_TEXTURE_2D);
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
//glTranslatef(deltaX, deltaY, 0.0f);
glRotatef(projRotation,0,0,1);
glRotatef(incidentAngle,1,0,0);
glRotatef(twist,0,0,1);

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
 
		x1 = deltaX-xDist,y1= deltaY + yDist;

		glPushMatrix();
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

		x1 = deltaX - xDist,y1= deltaY - yDist;

		glPushMatrix();
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


		x1 = deltaX + xDist,y1=deltaY + yDist;

		glPushMatrix();
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

		x1 = deltaX + xDist,y1= deltaY - yDist;

		glPushMatrix();
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

//		glEnable(GL_TEXTURE_2D);
//
//		glPushMatrix();
//		glRotatef(25.0f,0.0,0.0,1.0);
//		//CODY FACE CODE
//		glBindTexture(GL_TEXTURE_2D, codyTex);
//		glBegin( GL_QUADS );
//	//Top-left vertex (corner)
//	glTexCoord2i( 0, 0 );
//	glVertex3f( -4.70f, 0.0f, 6.5f );
// 
//	//Top-right vertex (corner)
//	glTexCoord2i( 1, 0 );
//	glVertex3f( 4.70f, 0.0f, 6.5f );
// 
//	//Bottom-right vertex (corner)
//	glTexCoord2i( 1, 1 );
//	glVertex3f( 4.70f, 0.0f, 0.0f );
//	
// 
//	//Bottom-left vertex (corner)
//	glTexCoord2i( 0, 1 );
//	glVertex3f( -4.70f, 0.0f, 0.0f );
//	
//glEnd();
//glPopMatrix();
//
//glPushMatrix();
//glTranslatef(3.0,-4.0f, 0.0f);
//		glRotatef(-30.0f,0.0,0.0,1.0);
//		
//		//STONEWALL
//		glBindTexture(GL_TEXTURE_2D, wallTex);
//		glBegin( GL_QUADS );
//	//Top-left vertex (corner)
//	glTexCoord2i( 0, 0 );
//	glVertex3f( -2.60f, 0.0f, 3.0f );
// 
//	//Top-right vertex (corner)
//	glTexCoord2i( 1, 0 );
//	glVertex3f( 2.60f, 0.0f, 3.0f );
// 
//	//Bottom-right vertex (corner)
//	glTexCoord2i( 1, 1 );
//	glVertex3f( 2.60f, 0.0f, 0.0f );
//	
// 
//	//Bottom-left vertex (corner)
//	glTexCoord2i( 0, 1 );
//	glVertex3f( -2.60f, 0.0f, 0.0f );
//	
//glEnd();
//glPopMatrix();
//
//
//		glDisable(GL_TEXTURE_2D);

		
    SDL_GL_SwapBuffers();
}


void Projector::renderBathtub(float distanceFromTable, float incidentAngle, float projRotation,  float twist, float deltaX, float deltaY, float distX, float distY){

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(PROJ_FOV,1.6f,0.1f,1000.0f);
    glMatrixMode(GL_MODELVIEW);
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); 
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_TEXTURE_2D);

	glTranslatef(0.0,0.0,distanceFromTable);
	glRotatef(projRotation,0,0,1);
	glRotatef(incidentAngle,1,0,0);
	glRotatef(twist,0,0,1);
	
	for(int i = 0; i < 1; i++){
	
	glPushMatrix();
	
	glBindTexture(GL_TEXTURE_2D, texture[i % 10]);

	//BOTTOM OF BOX
	glBegin( GL_QUADS );
	
	glTexCoord2i( 0, 1 );
	glVertex3f( -2.62 + distX, -1.5 + distY, 2.0f );
	glTexCoord2i( 1, 1 );
	glVertex3f( 2.62 + distX, -1.5 + distY, 2.0f );
	glTexCoord2i( 1, 0 );
	glVertex3f( 2.62 + distX, 1.5 + distY, 2.0f );
	glTexCoord2i( 0, 0 );
	glVertex3f( -2.62 + distX, 1.5 + distY, 2.0f );
	glEnd();	
	glPopMatrix();

	

	SDL_GL_SwapBuffers();

	Sleep(80);
	}

	glDisable(GL_TEXTURE_2D);
}