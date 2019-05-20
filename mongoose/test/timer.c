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
#include "mgos_timers.h"
#include "mgos_time.h"
#include "environment.h"

#define ITER 100

static int var[ITER];
static int current = 0;

static long long int prev_time = 0;
static long long int cur_time = 0;
static bool flag = 0;

static mgos_timer_id timer_1;
static mgos_timer_id timer_2;


static void task_1(void *arg) {
    flag = 1;
    prev_time = mgos_uptime_micros();
}

static void task_2(void *arg) {
    if (flag == 0) {
        return;
    }

    cur_time = mgos_uptime_micros();
    var[current] = cur_time - prev_time;
    current++;
    flag = 0;

    if (current == ITER) {
        output("Timers test switching", var, ITER);

        mgos_clear_timer(timer_1);
        mgos_clear_timer(timer_2);
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    timer_1 = mgos_set_timer(portTICK_RATE_MS, MGOS_TIMER_REPEAT, task_1, NULL);
    timer_2 = mgos_set_timer(portTICK_RATE_MS, MGOS_TIMER_REPEAT, task_2, NULL);

    return MGOS_APP_INIT_SUCCESS;
}
