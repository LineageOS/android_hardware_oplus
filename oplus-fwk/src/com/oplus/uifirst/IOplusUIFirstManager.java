/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.uifirst;

public interface IOplusUIFirstManager {
    default void setBinderThreadUxFlag(int pid, int flag) {}
}
