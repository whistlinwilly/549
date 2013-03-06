package projector.client;


import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

import projector.main.MainActivity;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

public class NetClient extends AsyncTask<MainActivity, MainActivity, MainActivity>{

    /**
     * Maximum size of buffer
     */
    public static final int BUFFER_SIZE = 16;
    public static final int REC_INIT = 0;
    public static final int REC_OBJ = 1;
    public static final int REC_TEXTURE = 2;
    
    public Socket socket = null;
    private static final String TAG = "NetClient";
    private String host = null;
    private int port;
    public boolean connected;
    public byte[] inBuffer= new byte[16];
    public String inString = null;
    public String outString;
    public byte[] outBuffer = new byte[1];
    public int commandReceived;
    private boolean stillListening;
    /**
     * Constructor with Host, Port and MAC Address
     * @param host
     * @param port
     * @param macAddress
     * @throws InterruptedException 
     */
    public NetClient(String host, int port) throws InterruptedException {
        this.host = host;
        this.port = port;
        connected = false;
        stillListening = true;
    }

    public void connectWithServer() throws InterruptedException {
    	try {
            InetAddress serverAddr = InetAddress.getByName(this.host);
            Log.d(TAG, "C: Connecting...");
            
            //connect to server (table) socket
            socket = new Socket(serverAddr, this.port);
            if(socket != null){
            	connected = true;
            	Log.i(TAG, "SOCKET CREATED SUCCESSFULLY");
            }
            //while connected to server
            while (stillListening) {
            	Log.i(TAG, "CONNECTED TO SERVER");
            	receiveMessageFromServer();
            }

        } catch (Exception e) {
           
            connected = false;
        }
 
    }

    public void receiveMessageFromServer(){
    	try {
			if(socket.getInputStream().read(inBuffer) != 0){
				inString = new String(inBuffer);
				Log.i(TAG, "Buffer: " + inBuffer);
				Log.i(TAG, "String: " + inString);
				//commandReceived = Integer.parseInt(inString);
				commandReceived = 0;
				Log.i(TAG, "Received: " + commandReceived);
				stillListening = false;
			}
			else {
				Log.i(TAG, "Waiting on message...");
			}
			
			
    } catch (Exception e) {
    	
    }
    }
    
    public void sendMessageToServer(String outString){
    	Log.i(TAG, "Sending to Server: " + outString);
    	this.outBuffer = outString.getBytes();
    	try {
			socket.getOutputStream().write(outBuffer);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	Log.i(TAG, "OUTPUT SENT TO SERVER!");
    }

/*	@Override
	public void run() {
		try {
			connectWithServer();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}*/

    @Override
	protected void onPostExecute(MainActivity params){
    	if(connected)
    		sendMessageToServer("G");
        	Log.i(TAG, "Socket Closing");
        try {
        	if(connected)
			socket.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

	@Override
	protected MainActivity doInBackground(MainActivity... params) {
		try {
			connectWithServer();
			if(connected)
			params[0].mGLView.renderer.setInitFlag(true);

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return params[0];
		}
		return params[0];
	}

}

