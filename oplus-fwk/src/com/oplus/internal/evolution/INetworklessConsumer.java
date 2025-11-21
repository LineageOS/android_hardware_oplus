/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.internal.evolution;

import android.os.Bundle;
import android.os.IInterface;

public interface INetworklessConsumer extends IInterface {
    default void accept(long token, int resultCode, Bundle data) {}
}
