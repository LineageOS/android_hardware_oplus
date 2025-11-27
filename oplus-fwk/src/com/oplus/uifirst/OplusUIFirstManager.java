/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.uifirst;

public class OplusUIFirstManager implements IOplusUIFirstManager {
    private static OplusUIFirstManager sInstance = null;

    public static OplusUIFirstManager getInstance() {
        if (sInstance == null) {
            sInstance = new OplusUIFirstManager();
        }
        return sInstance;
    }
}
