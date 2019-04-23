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

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "environment.h"

static int var[ITER];
static int current = 0;

static long long int prevTime = 0;
static long long int curTime = 0;
static bool flag = 0;

TimerHandle_t xTimers[2];


static void task1(void *arg)
{
    flag = 1;
    prevTime = esp_timer_get_time();
}

static void task2(void *arg)
{
    if (flag == 0)
    {
        return;
    }

    curTime = esp_timer_get_time();
    var[current] = curTime - prevTime;
    current++;
    flag = 0;

    if (current == ITER)
    {
        output("Timers test switching", var, true);

        xTimerStop(xTimers[0], 0);
        xTimerStop(xTimers[1], 0);
    }
}

void app_main()
{
    xTimers[0] = xTimerCreate("Timer 1", 1, pdTRUE, (void *) 0, task1);
    xTimers[1] = xTimerCreate("Timer 2", 1, pdTRUE, (void *) 0, task2);

    xTimerStart(xTimers[0], 0);
    xTimerStart(xTimers[1], 0);
}
