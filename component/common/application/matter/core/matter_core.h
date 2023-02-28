#pragma once

#include <platform/CHIPDeviceLayer.h>

CHIP_ERROR matter_core_start(void);
uint16_t matter_core_get_next_endpoint_index(void);
int32_t matter_core_enable_all_endpoints(void);

namespace lock {

typedef enum status {
    LOCK_FAILED,
    LOCK_ALREADY_TAKEN,
    LOCK_SUCCESS,
} status_t;

status_t chip_stack_lock(uint32_t ticks_to_wait);
int32_t chip_stack_unlock(void);

} // namespace lock
