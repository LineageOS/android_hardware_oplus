#pragma once

#include <stdbool.h>
#include <stdint.h>

#define HAPTIC_PARAM_DRC_MARK 0x01
#define HAPTIC_PARAM_MAX_DRC 0x03

enum PATTERN_PERFORM_STATE {
    PATTERN_PERFORM_START = 1,
    PATTERN_PERFORM_INTERRUPT,
    PATTERN_PERFORM_RESUME,
    PATTERN_PERFORM_BUSY,
    PATTERN_PERFORM_END,
    INVALID_PATTERN_STATUS,
};

extern "C" {
extern int aac_vibra_init(uint32_t* deviceType);
extern int aac_vibra_on(unsigned int timeout_ms);
extern int aac_vibra_off();
extern int aac_vibra_setAmplitude(uint8_t amplitude);
extern int aac_vibra_perform(uint32_t effect, uint32_t strength, uint32_t* timeout_ms);
extern int aac_vibra_performEnvelope(void* vib_info, bool fast_flag);
extern int aac_vibra_performRtp(int fd, uint32_t* timeout_ms);
extern int aac_vibra_performPattern(void* pattern);
extern int aac_vibra_performHe(void* he);
extern int aac_vibra_performHe_with_len(void* he, int32_t len);
extern int aac_vibra_dynamic_scale(uint8_t scale);
extern int aac_vibra_setting_f0(int f0);
extern int aac_vibra_stop(int32_t* index);
extern bool aac_vibra_set_drc_mode(int mode);
extern int aac_vibra_update_parameter(const int32_t* data, int32_t length);
}

extern void aac_vibra_looper_start();
extern int32_t aac_vibra_looper_post(const int32_t* pattern, int32_t patternLen, int32_t intervalMs,
                                     int32_t loopNum, int32_t amplitude, int32_t freq);

extern bool aac_vibra_looper_performParam(int32_t intervalMs, int32_t amplitude, int32_t freq);

extern bool aac_vibra_looper_stopPerformHe(void);

// new api
extern int32_t aac_vibra_looper_on(uint32_t time_out);
extern int32_t aac_vibra_looper_prebaked_effect(uint32_t effect_id, int32_t strength);
extern int32_t aac_vibra_looper_envelope(const int32_t* envelope_data, uint32_t data_len,
                                         bool fastFlag);
extern int32_t aac_vibra_looper_rtp(int32_t fd);
