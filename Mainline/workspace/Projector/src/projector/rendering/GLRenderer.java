package projector.rendering;


import java.io.FileNotFoundException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import projector.rendering.ObjectFactory;


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
	   
	   private static final int LOAD_MODELS = 0;
	   private static final int PATTERN1 = 1;
	   private static final int PATTERN2 = 2;
	   private static final int CONFIRM_PATTERN = 3;
	   
	   private final GLCircle bigCircle = new GLCircle(0,0,2,100);
	   private final GLCircle smallCircle = new GLCircle(0,3,0.5f,100);
	   private final GLCircle smallBigCircle = new GLCircle(0,0,0.2f,100);
	   private final GLCircle smallSmallCircle = new GLCircle(0,3,0.2f,100);
	   //North / West
	   private final GLCircle circ00 = new GLCircle(0,0,0.5f,100);
	   private final GLCircle circn20 = new GLCircle(-2,0,0.5f,100);
	   private final GLCircle circn40 = new GLCircle(-4,0,0.5f,100);
	   private final GLCircle circn22 = new GLCircle(-2,2,0.5f,100);
	   private final GLCircle circn24 = new GLCircle(-2,4,0.5f,100);
	   private final GLCircle circn42 = new GLCircle(-4,2,0.5f,100);
	   private final GLCircle circn44 = new GLCircle(-4,4,0.5f,100);
	   private final GLCircle circ02 = new GLCircle(0,2,0.5f,100);
	   private final GLCircle circ04 = new GLCircle(0,4,0.5f,100);
	   
	   //North / East
	   private final GLCircle circ20 = new GLCircle(2,0,0.5f,100);
	   private final GLCircle circ40 = new GLCircle(4,0,0.5f,100);
	   private final GLCircle circ22 = new GLCircle(2,2,0.5f,100);
	   private final GLCircle circ24 = new GLCircle(2,4,0.5f,100);
	   private final GLCircle circ42 = new GLCircle(4,2,0.5f,100);
	   private final GLCircle circ44 = new GLCircle(4,4,0.5f,100);
	   
	   //South / East
	   private final GLCircle circ0n2 = new GLCircle(0,-2,0.5f,100);
	   private final GLCircle circ0n4 = new GLCircle(0,-4,0.5f,100);
	   private final GLCircle circ2n2 = new GLCircle(2,-2,0.5f,100);
	   private final GLCircle circ2n4 = new GLCircle(2,-4,0.5f,100);
	   private final GLCircle circ4n2 = new GLCircle(4,-2,0.5f,100);
	   private final GLCircle circ4n4 = new GLCircle(4,-4,0.5f,100);
	   
	   //South / West
	   private final GLCircle circn2n2 = new GLCircle(-2,-2,0.5f,100);
	   private final GLCircle circn4n2 = new GLCircle(-4,-2,0.5f,100);
	   private final GLCircle circn2n4 = new GLCircle(-2,-4,0.5f,100);
	   private final GLCircle circn4n4 = new GLCircle(-4,-4,0.5f,100);
	   
	   private float distanceFromTable, incidentAngle, rotation, deltaX, deltaY, twist;
	   
	   private long startTime;
	   private long fpsStartTime;
	   private long numFrames;
	   private volatile int stage;
	private Object myObj;
	private int width;
	private int height;
	private float eyeX = 0.0f;
	private float eyeY = 0.0f;
	private float eyeZ = 24.0f;
	private float centerX = 0.0f;
	private float centerY = 0.0f;
	private float centerZ = 0.0f;
	private float upX = 0.0f;
	private float upY = 1.0f;
	private float upZ = 0.0f;

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
	      
		   boolean SEE_THRU = true;
		      
		      
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
		      // gl.glEnable(GL10.GL_DITHER);
		      

		      
		      // ...
		      if (SEE_THRU) {
		         gl.glDisable(GL10.GL_DEPTH_TEST);
		         gl.glEnable(GL10.GL_BLEND);
		         gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE);
		      }
		      
		      gl.glEnable (GL10.GL_BLEND);
		      gl.glBlendFunc (GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
		      
		     
		      
		      
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
			   eyeX = vals[0];
			   eyeY = vals[1];
			   eyeZ = vals[2];
			   centerX = vals[3];
			   centerY = vals[4];
			   centerZ = vals[5];
			   upX = vals[6];
			   upY = vals[7];
			   upZ = vals[8];
			   Log.i(TAG, "Values BAHHHH: " + distanceFromTable + "," + incidentAngle + "," + rotation + "," + deltaX + "," + deltaY + "," + twist);
		   }
	   }
	   
	   public void onSurfaceChanged(GL10 gl, int width, int height) {
		// Define the view frustum
		      gl.glViewport(0, 0, width, height);
		      this.width = width;
		      this.height = height; 
	      
	   }
	   

	   
	   
	   
	   public void onDrawFrame(GL10 gl) {
	      
		   gl.glMatrixMode(GL10.GL_PROJECTION);
		   gl.glLoadIdentity();
		   float ratio = (float) width / height;
		   if(stage == LOAD_MODELS || stage == PATTERN1 || stage == PATTERN2){
			  // GLU.gluOrtho2D(gl, 0, width, 0, height);
			   //used to be 17.5
			   GLU.gluPerspective(gl, 17.0f, ratio, 0.1f, 1000f); 
			   GLU.gluLookAt(gl, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
			      // Clear the screen to black
			      gl.glClear(GL10.GL_COLOR_BUFFER_BIT
			            | GL10.GL_DEPTH_BUFFER_BIT);

			      // Position model so we can see it
			      gl.glMatrixMode(GL10.GL_MODELVIEW);
			      gl.glLoadIdentity();
			      if(stage == PATTERN1){
			    	  smallBigCircle.draw(gl);
			    	  smallSmallCircle.draw(gl);
			      }
			      else if (stage == PATTERN2){
			    	  bigCircle.draw(gl);
					  smallCircle.draw(gl);
			      }
		   }
		   else{
			   GLU.gluPerspective(gl, 17.0f, ratio, 0.1f, 1000f); 
			   GLU.gluLookAt(gl, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
			   gl.glClear(GL10.GL_COLOR_BUFFER_BIT
			            | GL10.GL_DEPTH_BUFFER_BIT);

			      // Position model so we can see it
			      gl.glMatrixMode(GL10.GL_MODELVIEW);
			      gl.glLoadIdentity();
			      
			  //    myObj.draw(gl);
			      
			      //CIRCLE GRID TEST PATTERN
			      float matAmbient[] = new float[] { 1, 0, 0, 1 };
			      float matDiffuse[] = new float[] { 1, 0, 0, 1 };
			      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_AMBIENT,
			            matAmbient, 0);
			      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_DIFFUSE,
			            matDiffuse, 0);
			      
			      circ00.draw(gl);
			      
			      matAmbient = new float[] { 0, 0, 1, 1 };
			      matDiffuse = new float[] { 0, 0, 1, 1 };
			      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_AMBIENT,
			            matAmbient, 0);
			      gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_DIFFUSE,
			            matDiffuse, 0);
			      
			      circn20.draw(gl);
			      circn40.draw(gl);
			      circn22.draw(gl);
			      circn24.draw(gl);
			      circn42.draw(gl);
			      circn44.draw(gl);
			      circ02.draw(gl);
			      circ04.draw(gl);
			      circ20.draw(gl);
			      circ40.draw(gl);
			      circ22.draw(gl);
			      circ24.draw(gl);
			      circ42.draw(gl);
			      circ44.draw(gl);
			      circ0n2.draw(gl);
			      circ0n4.draw(gl);
			      circ2n2.draw(gl);
			      circ2n4.draw(gl);
			      circ4n2.draw(gl);
			      circ4n4.draw(gl);
			      circn2n2.draw(gl);
			      circn4n2.draw(gl);
			      circn2n4.draw(gl);
			      circn4n4.draw(gl);
		   }

	      
//	      gl.glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
//          gl.glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
//          gl.glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
    	 // myObj.draw(gl);
	      
	   }



	public void loadAllModels() {
		//for now just one simple model
		ObjectFactory factory = new ObjectFactory("/Objects");
	      try {
			myObj = factory.loadObject("iPhoneBox.obj", context);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}  
	}
	   
	   
	   
}