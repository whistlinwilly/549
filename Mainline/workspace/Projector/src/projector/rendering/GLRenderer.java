package projector.rendering;


import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;

/**
 * This class implements our custom renderer. Note that the GL10 parameter passed in is unused for OpenGL ES 2.0
 * renderers -- the static class GLES20 is used instead.
 */
public class GLRenderer implements GLSurfaceView.Renderer {
	   private static final String TAG = "GLRenderer";
	   private final Context context;
	   
	   
	   private final GLCube cube = new GLCube();
	   private final GLCircle circle = new GLCircle(0,0,1,100);
	   private final GLCircle circle1 = new GLCircle(-6,0,1,100);
	   private final GLCircle circle2 = new GLCircle(6,0,1,100);
	   private final GLCircle circle3 = new GLCircle(0,-4,1,100);
	   private final GLCircle circle4 = new GLCircle(0,4,1,100);
	   private float distanceFromTable, incidentAngle, rotation, deltaX, deltaY, twist;
	   
	   private long startTime;
	   private long fpsStartTime;
	   private long numFrames;
	   private volatile int stage;

	   public GLRenderer(Context context) {
	      this.context = context;
	      this.stage = 0;
	      this.distanceFromTable = -24.0f;
	      this.incidentAngle = 0.0f;
	      this.deltaX = 0.0f;
	      this.deltaY = 0.0f;
	      this.rotation = 0.0f;
	      this.twist = 0.0f;
	   }

	   
	   
	   public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	      
	      // ...
	      
	      
	      
	      boolean SEE_THRU = true;
	      gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	      
	      startTime = System.currentTimeMillis();
	      fpsStartTime = startTime;
	      numFrames = 0;
	      

	      
	      // Define the lighting
	      float lightAmbient[] = new float[] { 0.2f, 0.2f, 0.2f, 1 };
	      float lightDiffuse[] = new float[] { 1, 1, 1, 1 };
	      float[] lightPos = new float[] { 1, 1, 1, 1 };
	      gl.glEnable(GL10.GL_LIGHTING);
	      gl.glEnable(GL10.GL_LIGHT0);
	      gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_AMBIENT, lightAmbient, 0);
	      gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_DIFFUSE, lightDiffuse, 0);
	      gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_POSITION, lightPos, 0);
	      

	      
	      // What is the cube made of?
	      float matAmbient[] = new float[] { 1, 1, 1, 1 };
	      float matDiffuse[] = new float[] { 1, 1, 1, 1 };
	      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_AMBIENT,
	            matAmbient, 0);
	      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_DIFFUSE,
	            matDiffuse, 0);
	      

	      
	      // Set up any OpenGL options we need
	      gl.glEnable(GL10.GL_DEPTH_TEST); 
	      gl.glDepthFunc(GL10.GL_LEQUAL);
	      gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);

	      // Optional: disable dither to boost performance
	      // gl.glDisable(GL10.GL_DITHER);
	      

	      
	      // ...
	      if (SEE_THRU) {
	         gl.glDisable(GL10.GL_DEPTH_TEST);
	         gl.glEnable(GL10.GL_BLEND);
	         gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE);
	      }
	      
	      
	      // Enable textures
	   //   gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
	   //   gl.glEnable(GL10.GL_TEXTURE_2D);

	      // Load the cube's texture from a bitmap
	   //   GLCube.loadTexture(gl, context, R.drawable.android);
	      
	      
	      
	      
	   }
	   
	   public void setStage(int stage){
		   this.stage = stage;
		   Log.i(TAG, "STAGE: " + this.stage);
	   }

	   public void setValues(float[] vals){
		   if (vals != null && vals.length > 5){
			   distanceFromTable = vals[0];
			   incidentAngle = vals[1];
			   rotation = vals[2];
			   deltaX = vals[3];
			   deltaY = vals[4];
			   twist = vals[5];
			   Log.i(TAG, "Values BAHHHH: " + distanceFromTable + "," + incidentAngle + "," + rotation + "," + deltaX + "," + deltaY + "," + twist);
		   }
	   }
	   
	   public void onSurfaceChanged(GL10 gl, int width, int height) {
	      
	      // ...
	      
	      
	      // Define the view frustum
	      gl.glViewport(0, 0, width, height);
	      gl.glMatrixMode(GL10.GL_PROJECTION);
	      gl.glLoadIdentity();
	      float ratio = (float) width / height;
	      GLU.gluPerspective(gl, 17.5f, ratio, 0.1f, 1000f); 
	     // GLU.gluOrtho2D(gl, 0, width, 0, height);
	      
	   }
	   

	   
	   
	   
	   public void onDrawFrame(GL10 gl) {
	      
	      // ...
	      
	      
		   gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	      
	      // Clear the screen to black
	      gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
	      
	      
	    	  // Position model so we can see it
	    	  gl.glMatrixMode(GL10.GL_MODELVIEW);
	    	  gl.glLoadIdentity();
	 
	    	  gl.glRotatef(twist, 0, 0, 1);
	    	  gl.glTranslatef(0.0f,0.0f, distanceFromTable);
	    	  gl.glRotatef(rotation, 0, 0, 1);
	    	  gl.glRotatef(incidentAngle, 1, 0, 0);
	    //	  if (initialize){
	    	  // Other drawing commands go here...
	      
	      
	    	  // Set rotation angle based on the time
	    	  //  long elapsed = System.currentTimeMillis() - startTime;
	    	  //  gl.glRotatef(elapsed * (30f / 1000f), 0, 1, 0);
	    	  //  gl.glRotatef(elapsed * (15f / 1000f), 1, 0, 0);

	    
	    	  // Draw the model
	    	  
	    	  //ok for now but change TODO
	    	  if (stage == 1) circle.draw(gl);
	    	  else if (stage == 2){
	    		  circle.draw(gl);
	    		  circle1.draw(gl);
	    		  circle2.draw(gl);
	    		  circle3.draw(gl);
	    		  circle4.draw(gl);
	    	  }
	    
	      
	      
	    	  // Keep track of number of frames drawn
	    	  numFrames++;
	    	  long fpsElapsed = System.currentTimeMillis() - fpsStartTime;
	    	  if (fpsElapsed > 5 * 1000) { // every 5 seconds
	    		  float fps = (numFrames * 1000.0F) / fpsElapsed;
	    		  Log.d(TAG, "Frames per second: " + fps + " (" + numFrames
	    				  + " frames in " + fpsElapsed + " ms)");
	    		  fpsStartTime = System.currentTimeMillis();
	    		  numFrames = 0;
	    	  }
	     // }
	      
	      
	      
	      
	      
	   }
	   
	   
	   
}