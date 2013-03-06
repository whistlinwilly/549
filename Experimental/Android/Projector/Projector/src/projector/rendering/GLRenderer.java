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
	   private final GLCircle circle = new GLCircle(0,0,1.0f,100);
	   private final GLCircle circle2 = new GLCircle(0,2,0.4f,100);
	   private final GLCircle circle3 = new GLCircle(0,-2,0.4f,100);
	   
	   
	   private long startTime;
	   private long fpsStartTime;
	   private long numFrames;
	   private volatile boolean initialize;

	   public GLRenderer(Context context) {
	      this.context = context;
	      this.initialize = false;
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
	   
	   public void setInitFlag(boolean flag){
		   this.initialize = flag;
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
	    	  gl.glTranslatef(0, 0, -38.2f);
	    	  gl.glRotatef(78.0f, 0, 0, 1);
	    	  gl.glRotatef(27.0f, 1,0,0);
	    	  
	    //	  if (initialize){
	    	  // Other drawing commands go here...
	      
	      
	    	  // Set rotation angle based on the time
	    	  //  long elapsed = System.currentTimeMillis() - startTime;
	    	  //  gl.glRotatef(elapsed * (30f / 1000f), 0, 1, 0);
	    	  //  gl.glRotatef(elapsed * (15f / 1000f), 1, 0, 0);

	    
	    	  // Draw the model
	    	  if(initialize){
	    	  circle.draw(gl);
	    	  circle2.draw(gl);
	    	  circle3.draw(gl);
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