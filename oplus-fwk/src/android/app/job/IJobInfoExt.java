package android.app.job;

public interface IJobInfoExt {

    public final class JobBuilderExt {

        public boolean mIsOplusJob;

        public JobBuilderExt setRequiresBattIdle(boolean requiresBattIdle, int extra) {
            return this;
        }
    }

    default boolean getBooleanConstraint(String type, boolean defValue) {
        return defValue;
    }

    default String getStringConstraint(String type, String defValue) {
        return defValue;
    }

    default int getIntConstraint(String type, int defValue) {
        return defValue;
    }

    default long getLongConstraint(String type, long defValue) {
        return defValue;
    }

    default void setBooleanConstraint(String type, boolean value) {}

    default void setStringConstraint(String type, String value) {}

    default void setIntConstraint(String type, int value) {}

    default void setLongConstraint(JobInfo job, String type, long value) {}

    default void initJobInfo(Object in) {}

    default void initJobInfoPure(JobBuilderExt jobBuilderExt) {}

    default void writeToParcelJobInfo(Object out, int flags) {}
}
