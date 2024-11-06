package com.oplus.flexiblewindow;

import android.content.ComponentName;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.Region;
import android.util.AttributeSet;
import android.view.SurfaceView;

public class FlexibleTaskView extends SurfaceView {

    public FlexibleTaskView(Context context) {
        this(context, null);
    }

    public FlexibleTaskView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FlexibleTaskView(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, 0);
    }

    public FlexibleTaskView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes, true);
    }

    public interface Listener {
        default void onInitialized(boolean isStartSuccess) {}
        default void onReleased() {}
        default void onTaskCreated(int taskId, ComponentName name) {}
        default void onTaskChanged(int taskId, ComponentName name, Rect rect) {}
        default void onTaskVisbilityChanged(int taskId, boolean visible) {}
        default void onTaskRemovalStarted(int taskId) {}
        default void onBackPressedOnTaskRoot(int taskId) {}
        default void updateTouchRegion(Region region) {}
    }
}
