/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/sleep.h"

static bool awake;

static void sleep_callback(void) {
    printf("RTC woke us up\n\n");
    awake = true;
}

static void rtc_sleep(void) {
    // Start on Wednesday 5th of July 2023 12:00:00
    datetime_t t = {
            .year  = 2023,
            .month = 07,
            .day   = 05,
            .dotw  = 3, // 0 is Sunday, so 3 is Wednesday
            .hour  = 12,
            .min   = 00,
            .sec   = 00
    };

    // Alarm 10 seconds later
    datetime_t t_alarm = {
            .year  = 2023,
            .month = 07,
            .day   = 05,
            .dotw  = 3, // 0 is Sunday, so 3 is Wednesday
            .hour  = 12,
            .min   = 00,
            .sec   = 2
    };

    // Start the RTC
    rtc_init();
    rtc_set_datetime(&t);

    printf("Sleeping for 10 seconds\n");
    uart_default_tx_wait_blocking();

    sleep_goto_sleep_until(&t_alarm, &sleep_callback);
}

int main() {

    stdio_init_all();

    printf("Hello Sleep!\n");

    printf("Switching to XOSC\n");

    // Wait for the fifo to be drained so we get reliable output
    uart_default_tx_wait_blocking();

    // UART will be reconfigured by sleep_run_from_xosc
    sleep_run_from_xosc();

    printf("Switched to XOSC\n");

    awake = false;

    rtc_sleep();

    // Make sure we don't wake
    while (!awake) {
        printf("Should be sleeping\n");
    }

    // //Restart the ROSC - this is crucial in preventing lock-up of the cores, so execution may continue
    rosc_restart();

    uart_default_tx_wait_blocking();

    printf("ROSC restarted!\n");

    return 0;
}
