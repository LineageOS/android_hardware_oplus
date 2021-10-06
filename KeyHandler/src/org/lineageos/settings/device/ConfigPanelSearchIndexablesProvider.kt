/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.database.Cursor
import android.database.MatrixCursor
import android.provider.SearchIndexableResource
import android.provider.SearchIndexablesProvider
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_CLASS_NAME
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_ICON_RESID
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_INTENT_ACTION
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_INTENT_TARGET_CLASS
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_INTENT_TARGET_PACKAGE
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_RANK
import android.provider.SearchIndexablesContract.COLUMN_INDEX_XML_RES_RESID
import android.provider.SearchIndexablesContract.INDEXABLES_RAW_COLUMNS
import android.provider.SearchIndexablesContract.INDEXABLES_XML_RES_COLUMNS
import android.provider.SearchIndexablesContract.NON_INDEXABLES_KEYS_COLUMNS

class ConfigPanelSearchIndexablesProvider : SearchIndexablesProvider() {
    override fun onCreate(): Boolean = true

    override fun queryXmlResources(projection: Array<String?>?): Cursor {
        val cursor = MatrixCursor(INDEXABLES_XML_RES_COLUMNS)
        INDEXABLE_RES.forEach {
            cursor.addRow(generateResourceRef(it))
        }
        return cursor
    }

    override fun queryRawData(projection: Array<String?>?): Cursor {
        return MatrixCursor(INDEXABLES_RAW_COLUMNS)
    }

    override fun queryNonIndexableKeys(projection: Array<String?>?): Cursor {
        return MatrixCursor(NON_INDEXABLES_KEYS_COLUMNS)
    }

    private fun generateResourceRef(sir: SearchIndexableResource): Array<Any?> {
        val ref = arrayOfNulls<Any>(7)
        ref[COLUMN_INDEX_XML_RES_RANK] = sir.rank
        ref[COLUMN_INDEX_XML_RES_RESID] = sir.xmlResId
        ref[COLUMN_INDEX_XML_RES_CLASS_NAME] = null
        ref[COLUMN_INDEX_XML_RES_ICON_RESID] = sir.iconResId
        ref[COLUMN_INDEX_XML_RES_INTENT_ACTION] = "com.android.settings.action.EXTRA_SETTINGS"
        ref[COLUMN_INDEX_XML_RES_INTENT_TARGET_PACKAGE] = "org.lineageos.settings.device"
        ref[COLUMN_INDEX_XML_RES_INTENT_TARGET_CLASS] = sir.className
        return ref
    }

    companion object {
        private const val TAG = "ConfigPanelSearchIndexablesProvider"

        private val INDEXABLE_RES = arrayOf<SearchIndexableResource>(
            SearchIndexableResource(
                1, R.xml.button_panel, ButtonSettingsActivity::class.java.name, 0
            )
        )
    }
}
