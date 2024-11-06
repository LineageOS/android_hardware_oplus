package com.oplus.orms;

import com.oplus.orms.info.OrmsSaParam;

public class OplusResourceManager {

    private static OplusResourceManager sOplusResourceManager = null;

    public static OplusResourceManager getInstance(Class clazz) {
        if (sOplusResourceManager == null) {
            sOplusResourceManager = new OplusResourceManager();
        }
        return sOplusResourceManager;
    }

    public long ormsSetSceneAction(OrmsSaParam ormsSaParam) {
        return -1L;
    }
}
