/***
 * Excerpted from "Hello, Android!",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/eband for more book information.
***/

package org.example.opengl;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Toast;

public class OpenGL extends Activity {
   GLView view;
   @Override
   public void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      view = new GLView(this);
      setContentView(view);
   }

   @Override
   protected void onPause() {
       super.onPause();
       view.onPause();
   }

   @Override
   protected void onResume() {
       super.onResume();
       view.onResume();
   }
   
   @Override
   public boolean onKeyDown(int keyCode, KeyEvent event) {
       switch (keyCode) {
           case KeyEvent.KEYCODE_A:
        	   view.setValue(view.X_ANGLE, -7.0f);
               return true;
           case KeyEvent.KEYCODE_D:
        	   view.setValue(view.X_ANGLE, 7.0f);
               return true;
           case KeyEvent.KEYCODE_W:
        	   view.setValue(view.Y_ANGLE, 7.0f);
               return true;
           case KeyEvent.KEYCODE_S:
        	   view.setValue(view.Y_ANGLE, -7.0f);
               return true;
           case KeyEvent.KEYCODE_Q:
        	   view.setValue(view.Z_ANGLE, -7.0f);
               return true;
           case KeyEvent.KEYCODE_E:
        	   view.setValue(view.Z_ANGLE, 7.0f);
               return true;
           case KeyEvent.KEYCODE_Z:
        	   view.setValue(view.Z_DIST, -7.0f);
               return true;
           case KeyEvent.KEYCODE_X:
        	   view.setValue(view.Z_DIST, 7.0f);
               return true;
           case KeyEvent.KEYCODE_4:
        	   view.setValue(view.X_EYE, -2.0f);
        	   return true;
           case KeyEvent.KEYCODE_6:
        	   view.setValue(view.X_EYE, 2.0f);
               return true;
           case KeyEvent.KEYCODE_2:
        	   view.setValue(view.Y_EYE, -2.0f);
        	   return true;
           case KeyEvent.KEYCODE_8:
        	   view.setValue(view.Y_EYE, 2.0f);
               return true;
           case KeyEvent.KEYCODE_7:
        	   view.setValue(view.Z_EYE, -2.0f);
        	   return true;
           case KeyEvent.KEYCODE_9:
        	   view.setValue(view.Z_EYE, 2.0f);
               return true;
           default:
               super.onKeyUp(keyCode, event);
               return false;
       }
   }
}
