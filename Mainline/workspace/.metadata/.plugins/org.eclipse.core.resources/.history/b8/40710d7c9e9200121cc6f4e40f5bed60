package org.example.opengl;

import java.nio.ByteBuffer;

public class Surface {
	
	byte[] indices;
	ByteBuffer index;
	int type;
	int QUAD = 0;
	int TRI = 1;
	
	public Surface(int index1, int index2, int index3){
		indices = new byte[3];
		indices[0] = (byte) index1;
		indices[1] = (byte) index2;
		indices[2] = (byte) index3;
		type = TRI;
	}
	
	public Surface(int index1, int index2, int index3, int index4){
		indices = new byte[4];
		indices[0] = (byte) index1;
		indices[1] = (byte) index2;
		indices[2] = (byte) index3;
		indices[3] = (byte) index4;
		type = QUAD;
	}
	
//	public void setIndices(int index1, int index2, int index3){
//		indices[0] = (byte) index1;
//		indices[1] = (byte) index2;
//		indices[2] = (byte) index3;
//	}
	
	

}
