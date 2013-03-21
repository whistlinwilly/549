package projector.main;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;


public class BootUpReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context arg0, Intent intent) {
		// TODO Auto-generated method stub
		if (Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction()))
		{
			Intent startMain = new Intent("projector.main.MainActivity");
			arg0.startActivity(startMain);
		}
	}
	
}