/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.haptics

import android.service.quicksettings.Tile
import android.service.quicksettings.TileService

class HapticTileService : TileService() {
    override fun onStartListening() {
        super.onStartListening()
        updateTile()
    }

    override fun onClick() {
        super.onClick()
        val current = HapticUtils.getTouchStyle()
        val next =
            if (current == HapticUtils.STYLE_CRISP) {
                HapticUtils.STYLE_GENTLE
            } else {
                HapticUtils.STYLE_CRISP
            }
        HapticUtils.setTouchStyle(this, next)
        updateTile()
    }

    private fun updateTile() {
        val tile = qsTile ?: return
        val isCrisp = HapticUtils.getTouchStyle() == HapticUtils.STYLE_CRISP
        tile.state = Tile.STATE_ACTIVE
        tile.subtitle =
            if (isCrisp) {
                getString(R.string.haptic_style_crisp)
            } else {
                getString(R.string.haptic_style_gentle)
            }
        tile.updateTile()
    }
}
