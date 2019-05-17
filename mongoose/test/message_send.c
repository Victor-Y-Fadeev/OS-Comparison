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

static int var[ITER];
static int current = 0;

static long long int prev_time = 0;
static long long int cur_time = 0;

static mgos_timer_id timer;

struct mbuf msg_mbuf;


static void task(void *arg) {
    int msg = 255;

    prev_time = mgos_uptime_micros();
    mbuf_append(&msg_mbuf, &msg, sizeof(int));
    cur_time = mgos_uptime_micros();

    msg = *msg_mbuf.buf;
    mbuf_remove(&msg_mbuf, sizeof(int));

    var[current] = cur_time - prev_time;
    current++;

    if (current == ITER) {
        output("Send message test", var, true);

        mbuf_free(&msg_mbuf);
        mgos_clear_timer(timer);
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    mbuf_init(&msg_mbuf, 0);

    timer = mgos_set_timer(0, MGOS_TIMER_REPEAT, task, NULL);

    return MGOS_APP_INIT_SUCCESS;
}
