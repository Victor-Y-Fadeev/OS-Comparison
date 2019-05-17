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

static long long int prevTime = 0;
static long long int curTime = 0;


static void vTask(void *pvParameters)
{
    double average = 0;

    prevTime = esp_timer_get_time();
    for(int i = 0; i < ITER; i++) 
    {
        portYIELD_FROM_ISR();
    }
    curTime = esp_timer_get_time();

    average = curTime - prevTime;

    prevTime = esp_timer_get_time();
    for(int i = 0; i < ITER; i++) 
    {
    }
    curTime = esp_timer_get_time();

    average = (average - curTime + prevTime) / ITER;
  
    single("ISR test", average);

    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(vTask, "Task", 10000, NULL, 1, NULL);
}
