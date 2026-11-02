target_sources(app PRIVATE ${PLATFORM_DIR}/src/wba55_standby.c ${PLATFORM_DIR}/src/wba55_wakeup.c)
target_compile_definitions(app PRIVATE BLUEJOULE_WBA55_CONNECTED_STANDBY=1 BLUEJOULE_WBA55_WAKEUP_US=500)
zephyr_ld_options(-Wl,--wrap=bt_enable -Wl,--wrap=pm_policy_next_custom_ticks -Wl,--wrap=os_timer_get_earliest_time -Wl,--wrap=bt_le_adv_start -Wl,--wrap=ll_sys_dp_slp_enter)
