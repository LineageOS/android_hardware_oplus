/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.pogo_keyboard;

@VintfStability
interface IPogoKeyboard {
    /**
     * Close POGO keyboard device
     */
    void kb_close(int fd);

    /**
     * Send a command and receive response (UART transaction)
     *
     * @param fd        device handle from kb_open()
     * @param cmd       protocol command ID (mapped inside libpogo_keyboard.so)
     * @param request   raw request payload (already framed or partially framed)
     * @param response  raw response buffer from MCU
     */
    void kb_getfeature(
        int fd,
        int cmd,
        in byte[] request,
        out byte[] response
    );

    /**
     * Open POGO keyboard UART device
     * returns file descriptor or handle (>0)
     */
    int kb_open();

    /**
     * Send a command without response or with short ACK
     *
     * @param fd        device handle
     * @param data      raw payload (UART frame or payload)
     * @param cmd       protocol command ID
     *
     * @return status code
     */
    int kb_setfeature(
        int fd,
        in byte[] data,
        int cmd
    );
}
