/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.commondcs;

import vendor.oplus.hardware.commondcs.StringPair;

@VintfStability
interface ICommonDcsAidlHalService {
    int notifyMsgToCommonDcs(in List<StringPair> data, String logTag, String eventId);
}
