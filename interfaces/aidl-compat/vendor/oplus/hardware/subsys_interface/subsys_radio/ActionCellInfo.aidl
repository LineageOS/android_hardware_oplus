/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.CellIdentitiesInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.HdrCellPropertyType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.LocationInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.RsCellPropertyType;

@VintfStability
parcelable ActionCellInfo {
    CellIdentitiesInfo cellIds;
    boolean causeValid = false;
    int causeCode = 0;
    int[] expiryTimeSecPerCause;
    boolean nbrValid = false;
    int nbrClusterId = 0;
    boolean anchorValid = false;
    int anchorCell = 0;
    boolean priorityValid = false;
    int priority = 0;
    boolean locationValid = false;
    LocationInfo location;
    boolean hdrValid = false;
    HdrCellPropertyType hdrProperty;
    boolean rsValid = false;
    RsCellPropertyType rsProperty;
}
