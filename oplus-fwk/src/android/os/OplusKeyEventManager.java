package android.os;

import android.view.KeyEvent;

public class OplusKeyEventManager {

    public interface OnKeyEventObserver {
        void onKeyEvent(KeyEvent event); 
    }
}
