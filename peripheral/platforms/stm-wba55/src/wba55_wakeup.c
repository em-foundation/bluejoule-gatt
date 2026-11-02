/* App-local experiment: compensate the driver's fixed wakeup subtraction
 * without changing the installed Zephyr driver or ST SDK. */
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <app_conf.h>
#include <ll_sys.h>

BUILD_ASSERT(BLUEJOULE_WBA55_WAKEUP_US > 0);
BUILD_ASSERT(BLUEJOULE_WBA55_WAKEUP_US < CFG_LPM_STDBY_WAKEUP_TIME);
ll_sys_status_t __real_ll_sys_dp_slp_enter(uint32_t duration);
ll_sys_status_t __wrap_ll_sys_dp_slp_enter(uint32_t duration)
{
    const uint32_t extension = CFG_LPM_STDBY_WAKEUP_TIME - BLUEJOULE_WBA55_WAKEUP_US;
    /* Startup's no-wakeup sentinel must remain unchanged. */
    if (duration != LL_DP_SLP_NO_WAKEUP && duration < UINT32_MAX - extension) {
        duration += extension;
    }
    return __real_ll_sys_dp_slp_enter(duration);
}