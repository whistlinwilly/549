package com.example.imagestitcher;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.opengles.GL10;

public class Object {
	
	int uid;
	int classifiedType;
	float x, y, theta;
	FloatBuffer vertices;
	ByteBuffer textures;

	public Object(int uid) {
		this.uid = uid;
	}
	
	
	public void draw(GL10 gl) { 
	      gl.glVertexPointer(3, GL10.GL_FIXED, 0, vertices);

	      gl.glColor4f(0, 0, 1, 1);
	      gl.glNormal3f(0, 0, 1);
	      gl.glDrawArrays(GL10.GL_LINE_LOOP, 0, 3);
	    
	   }

}