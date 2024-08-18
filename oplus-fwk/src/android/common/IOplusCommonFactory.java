/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package android.common;

public interface IOplusCommonFactory {
    boolean isValid(int i);

    default <T extends IOplusCommonFeature> T getFeature(T def, Object... vars) {
        verityParams(def);
        return def;
    }

    default <T extends IOplusCommonFeature> void verityParams(T def) {
        if (def == null) {
            throw new IllegalArgumentException("def can not be null");
        }
        if (def.index() == OplusFeatureList.OplusIndex.End) {
            throw new IllegalArgumentException(def + "must override index() method");
        }
    }

    default void verityParamsType(String key, Object[] vars, int num, Class... types) {
        if (vars == null || types == null || vars.length != num || types.length != num) {
            throw new IllegalArgumentException(key + " need +" + num + " params");
        }
        for (int i = 0; i < num; i++) {
            if (vars[i] != null && !types[i].isInstance(vars[i])) {
                throw new IllegalArgumentException(types[i].getName() + " is not instance " + vars[i]);
            }
        }
    }
}
