/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.evolution;

import java.util.HashMap;
import java.util.Map;

public class NetworklessDevice {
    public static final NetworklessDevice DEFAULT = new NetworklessDevice();

    public static final int CAPABILITY_ID_BR = 1;
    public static final int CAPABILITY_ID_BT = 2;
    public static final int CAPABILITY_ID_NB = 4;
    public static final int CAPABILITYR_ID_WB = 8;
    public static final int CAPABILITY_ID_BLR = 16;

    protected int mAbility = 0;
    protected Map<Integer, String> mAddress = new HashMap<>();
    protected int mCapabilitMask = 0;
    protected String mChangeMacHash = null;
    protected String mDeviceId = null;
    protected String[] mMsisdnHash = new String[0];
    protected String mNickName = null;
    protected int mOperatingMode = 0;
    protected byte mSalt = 0;
    protected String mShortMacHash = null;

    public int getAbility() {
        return mAbility;
    }

    public Map<Integer, String> getAddressMap() {
        return mAddress;
    }

    public int getCapabilityMask() {
        return mCapabilitMask;
    }

    public String getChangeMacHash() {
        return mChangeMacHash;
    }

    public String getDeviceId() {
        return mDeviceId;
    }

    public String[] getMsisdnHash() {
        return mMsisdnHash;
    }

    public String getNickName() {
        return mNickName;
    }

    public int getOperatingMode() {
        return mOperatingMode;
    }

    public byte getSalt() {
        return mSalt;
    }

    public String getShortMacHash() {
        return mShortMacHash;
    }

    public void setAbility(int ability) {
        this.mAbility = ability;
    }

    public void setAddressMap(Map<Integer, String> map) {
        this.mAddress = map != null ? map : new HashMap<>();
    }

    public void setCapabilityMask(int mask) {
        this.mCapabilitMask = mask;
    }

    public void setChangeMacHash(String hash) {
        this.mChangeMacHash = hash;
    }

    public void setDeviceId(String id) {
        this.mDeviceId = id;
    }

    public void setMsisdnHash(String[] arr) {
        this.mMsisdnHash = (arr != null) ? arr : new String[0];
    }

    public void setNickName(String name) {
        this.mNickName = name;
    }

    public void setOperatingMode(int mode) {
        this.mOperatingMode = mode;
    }

    public void setSalt(byte salt) {
        this.mSalt = salt;
    }

    public void setShortMacHash(String hash) {
        this.mShortMacHash = hash;
    }
}
