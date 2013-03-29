package com.projector;

import java.io.IOException;

import projector.client.NetClient;
import projector.rendering.GLView;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {
	

	public volatile GLView mGLView;
	private NetClient netClient;
	private static final String TAG = "NetClient";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mGLView = new GLView(this);
		mGLView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);
		mGLView.setKeepScreenOn(true);
		setContentView(mGLView);
		
		//start client networking
		//Toast.makeText(this, "USE TEXT TO FOCUS PROJECTOR", Toast.LENGTH_LONG).show();
		try {
			//starts the networking thread
			netClient = new NetClient("10.0.1.2", 6881);
			netClient.execute(this);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void onStop(){
		super.onStop();
		if (netClient.socket.isClosed()){
			Log.i(TAG, "Socket is Closed, App is exiting");
		}
		else {
			try {
				Log.i(TAG, "Socket was not closed, closing now and exiting...");
				netClient.socket.close();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		netClient.isListening = false;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	public void onResume(){
		super.onResume();
		mGLView.onResume();
	}
	
	@Override
	public void onPause(){
		super.onPause();
		mGLView.onPause();
	}

}
