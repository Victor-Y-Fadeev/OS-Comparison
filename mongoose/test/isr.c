/*
 * Copyright (c) 2019 OS Research Group
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_event.h"
#include "mgos_timers.h"
#include "mgos_time.h"
#include "environment.h"

#define ITER 1000
#define EVENT_ISR MGOS_EVENT_BASE('I', 'S', 'R')

static long long int prev_time = 0;
static long long int cur_time = 0;

static mgos_timer_id timer;


static void isr_cb(int ev, void *ev_data, void *userdata) {
    (void) ev;
    (void) ev_data;
    (void) userdata;
}

static void task(void *arg) {
    double average;

    prev_time = mgos_uptime_micros();
    for (int i = 0; i < ITER; i++) {
        mgos_event_trigger(EVENT_ISR, NULL);
    }
    cur_time = mgos_uptime_micros();

    average = cur_time - prev_time;

    prev_time = mgos_uptime_micros();
    for (int i = 0; i < ITER; i++);
    cur_time = mgos_uptime_micros();

    average = (average - cur_time + prev_time) / ITER;

    single("ISR test", average);
    mgos_clear_timer(timer);
}

enum mgos_app_init_result mgos_app_init(void) {
    mgos_event_register_base(EVENT_ISR, "Test ISR");
    mgos_event_add_handler(EVENT_ISR, isr_cb, NULL);

    timer = mgos_set_timer(0, MGOS_TIMER_REPEAT, task, NULL);

    return MGOS_APP_INIT_SUCCESS;
}
