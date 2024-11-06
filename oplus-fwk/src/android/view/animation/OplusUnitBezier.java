package android.view.animation;

public class OplusUnitBezier {

    private double ax;
    private double ay;
    private double bx;
    private double by;
    private double cx;
    private double cy;

    public OplusUnitBezier(double p1x, double p1y, double p2x, double p2y) {
        final double d = p1x * 3.0d;
        final double d2 = (p2x - p1x) * 3.0d - d;
        final double d3 = p1y * 3.0d;
        final double d4 = (p2y - p1y) * 3.0d - d3;

        ax = (1.0d - d) - d2;
        ay = (1.0d - d3) - d4;
        bx = d2;
        by = d4;
        cx = d;
        cy = d3;
    }

    public double sampleCurveX(double t) {
        return ((ax * t + bx) * t + cx) * t;
    }

    public double sampleCurveY(double t) {
        return ((ay * t + by) * t + cy) * t;
    }

    public double sampleCurveDerivativeX(double t) {
        return (ax * 3.0d * t + bx * 2.0d) * t + cx;
    }

    public double solveCurveX(double x, double epsilon) {
        double t = x;
        for (int i = 0; i < 8; i++) {
            double x2 = sampleCurveX(t) - x;
            if (Math.abs(x2) < epsilon) {
                return t;
            }
            double d2 = sampleCurveDerivativeX(t);
            if (Math.abs(d2) < 0.000001d) {
                break;
            }
            t -= x2 / d2;
        }
        double t0 = 0.0d;
        double t1 = 1.0d;
        double t2 = x;
        if (t2 < 0.0d) {
            return 0.0d;
        }
        if (t2 > 1.0d) {
            return 1.0d;
        }
        while (t0 < t1) {
            double x2 = sampleCurveX(t2);
            if (Math.abs(x2 - x) < epsilon) {
                return t2;
            }
            if (x > x2) {
                t0 = t2;
            } else {
                t1 = t2;
            }
            t2 = (t1 - t0) * 0.5d + t0;
        }
        return t2;
    }

    public double solve(double x, double epsilon) {
        return sampleCurveY(solveCurveX(x, epsilon));
    }
}
