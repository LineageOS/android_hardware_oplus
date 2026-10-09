/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.content.Context
import java.text.NumberFormat

data class PenPopupContent(
    val title: String,
    val subtitle: String,
    val levelText: String? = null,
    val isCharging: Boolean = false,
    val isError: Boolean = false,
    val durationMs: Long? = null,
    val onClick: (() -> Unit)? = null,
)

class PenPopupContents(private val context: Context) {
    fun attached(name: String, level: Int) =
        PenPopupContent(
            title = name,
            subtitle =
                context.getString(
                    if (level == FULL_LEVEL) R.string.pen_fully_charged else R.string.pen_charging
                ),
            levelText = formatLevel(level),
            isCharging = true,
            durationMs = BATTERY_DURATION_MS,
        )

    fun lowBattery(name: String, level: Int) =
        PenPopupContent(
            title = name,
            subtitle = context.getString(R.string.pen_battery_low),
            levelText = formatLevel(level),
            isError = true,
            durationMs = LOW_BATTERY_DURATION_MS,
        )

    private fun formatLevel(level: Int) =
        if (level in 0..FULL_LEVEL) {
            NumberFormat.getPercentInstance().format(level / FULL_LEVEL.toDouble())
        } else {
            null
        }

    companion object {
        private const val FULL_LEVEL = 100

        private const val BATTERY_DURATION_MS = 2500L
        private const val LOW_BATTERY_DURATION_MS = 4000L
    }
}
