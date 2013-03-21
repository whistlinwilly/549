package projector.main;

import java.io.IOException;

import android.os.Bundle;
import android.app.Activity;
import projector.client.NetClient;
import android.util.Log;
import android.view.View;
import android.view.WindowManager.LayoutParams;
import android.widget.Toast;
import projector.rendering.*;

public class MainActivity extends Activity {

	public static final int INIT = 0;
    public static final int INITCOORDS = 1;
    public static final int OBJECT = 2;
    public static final int STOP = 3;
    public static final int PAUSE = 4;
    
	private static final String TAG = "NetClient";
	public volatile GLView mGLView;
	private NetClient netClient;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mGLView = new GLView(this);
		mGLView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);
		mGLView.setKeepScreenOn(true);
		setContentView(mGLView);
		//start client networking
		Toast.makeText(this, "Started Projector APP", Toast.LENGTH_SHORT).show();
		try {
			//starts the networking thread
			netClient = new NetClient("192.168.1.111", 6881);
			netClient.execute(this);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
        
       // while (netClient.inString == null){};
        

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

	