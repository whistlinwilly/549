package projector.main;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;



import projector.client.NetClient;
import projector.rendering.GLRenderer;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import projector.rendering.*;
public class MainActivity extends Activity {

	private static final String TAG = "NetClient";
	public volatile GLView mGLView;
	private NetClient netClient;
	private boolean initialize;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		initialize = false;
		mGLView = new GLView(this);
		mGLView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);
		setContentView(mGLView);
		//start client networking
		Toast.makeText(this, "Started Projector APP", Toast.LENGTH_SHORT).show();
		Thread cThread = null;
		try {
			//starts the networking thread
			netClient = new NetClient("10.0.1.3", 6882);
			netClient.execute(this);
			//cThread = new Thread(netClient);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
       // while (netClient.inString == null){};
        

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

	