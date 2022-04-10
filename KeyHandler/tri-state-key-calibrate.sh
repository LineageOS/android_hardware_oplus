#!/vendor/bin/sh
if [[ -f /mnt/vendor/persist/engineermode/tri_state_hall_data ]]; then
    CALIBRATION_DATA="$(cat /mnt/vendor/persist/engineermode/tri_state_hall_data)"
    CALIBRATION_DATA="${CALIBRATION_DATA//;/,}"
    echo -n $CALIBRATION_DATA > /proc/tristatekey/hall_data_calib
fi
