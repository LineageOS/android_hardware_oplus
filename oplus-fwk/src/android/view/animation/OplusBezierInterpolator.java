package android.view.animation;

public class OplusBezierInterpolator extends BaseInterpolator {

    private static final float ABOVE_ZERO = 0.0001f;
    private static final float BELOW_ONE = 0.9999f;

    private static final double ABOVE_ONE = 1.0d;
    private static final double EPSILON = 0.0000625d;

    private OplusUnitBezier mOplusUnitBezier;

    private boolean mLimit;

    public OplusBezierInterpolator(double p1x, double p1y, double p2x, double p2y, boolean limit) {
        mLimit = limit;
        mOplusUnitBezier = new OplusUnitBezier(p1x, p1y, p2x, p2y);
    }

    @Override
    public float getInterpolation(float input) {
        double interpolation = mOplusUnitBezier.solve(input, EPSILON);
        if (mLimit) {
            final boolean above = input >= ABOVE_ZERO && input <= BELOW_ONE;
            if (interpolation > ABOVE_ONE && !above) {
                interpolation = ABOVE_ONE;
            }
        }
        return (float) interpolation;
    }
}
