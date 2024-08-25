/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.osense.client;

import vendor.oplus.hardware.osense.client.OsenseAidlHalNotifyRequest;
import vendor.oplus.hardware.osense.client.OsenseAidlHalSaRequest;
import vendor.oplus.hardware.osense.client.OsenseControlInfo;

@VintfStability
interface IOsenseAidlHalReporter {
    int checkAccessPermission(String identity);
    void osenseClrSceneAction(String identity, long request);
    void osenseSetNotification(String identity, in OsenseAidlHalNotifyRequest osenseHalNotifyRequest);
    void osenseSetSceneAction(String identity, in OsenseAidlHalSaRequest osenseHalSaRequest);
    void osenseResetCtrlData(String identity);
    void osenseSetCtrlData(String identity, in OsenseControlInfo ctrldata);
}
