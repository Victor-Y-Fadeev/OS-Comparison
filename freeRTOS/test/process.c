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

#define ITER 100

static int var[ITER];
static int current = 0;

static long long int prevTime = 0;
static long long int curTime = 0;


void vTask1(void *pvParameters)
{
    while (current < ITER) 
    {
        prevTime = esp_timer_get_time();
        vTaskDelay(5);
    }

    vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
    int average = 0;
    
    while (current < ITER) 
    {
        if (prevTime != 0) 
        {
            curTime = esp_timer_get_time();
            var[current] = curTime - prevTime;
            average += var[current];
            prevTime = 0;
            current++;
        }
    }

    output("Processes switching test", var, ITER);
    
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(vTask1, "Task1", 10000, NULL, 2, NULL);
    xTaskCreate(vTask2, "Task2", 10000, NULL, 1, NULL);
}
