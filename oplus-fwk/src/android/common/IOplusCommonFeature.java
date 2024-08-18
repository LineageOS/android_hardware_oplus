/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package android.common;

public interface IOplusCommonFeature {
    default OplusFeatureList.OplusIndex index() {
        return OplusFeatureList.OplusIndex.End;
    }

    default IOplusCommonFeature getDefault() {
        return null;
    }
}
