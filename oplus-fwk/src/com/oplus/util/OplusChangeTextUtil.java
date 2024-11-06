package com.oplus.util;

public class OplusChangeTextUtil {

    private static final String TAG = "OplusChangeTextUtil";

    private static final float H1 = 0.9f;
    private static final float H2 = 1.0f;
    private static final float H3 = 1.1f;
    private static final float H4 = 1.25f;
    private static final float H5 = 1.45f;
    private static final float H6 = 1.65f;

    public static final int G1 = 1;
    public static final int G2 = 2;
    public static final int G3 = 3;
    public static final int G4 = 4;
    public static final int G5 = 5;
    public static final int G6 = 6;

    public static final float[] SCALE_LEVEL = {H1, H2, H3, H4, H5, H6};

    public static float getSuitableFontSize(float textSize, float scale, int level) {
        if (level < 2) {
            return textSize;
        }
        if (level > SCALE_LEVEL.length) {
            level = SCALE_LEVEL.length;
        }
        final float textSizeNoScale = textSize / scale;
        switch (level) {
            case G2:
                if (scale < H3) {
                    return H2 * textSizeNoScale;
                }
                return H3 * textSizeNoScale;
            case G3:
                if (scale < H3) {
                    return H2 * textSizeNoScale;
                }
                if (scale < H5) {
                    return H3 * textSizeNoScale;
                }
                return H4 * textSizeNoScale;
            default:
                if (scale > SCALE_LEVEL[level - 1]) {
                    return SCALE_LEVEL[level - 1] * textSizeNoScale;
                }
                return textSizeNoScale * scale;
        }
    }
}
