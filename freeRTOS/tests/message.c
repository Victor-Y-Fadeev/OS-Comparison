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

static xQueueHandle xQueue = NULL;


void vTask1(void *pvParameters)
{
    int massage = 0;
    portBASE_TYPE xStatus;

    while (current <= ITER) 
    {
        xStatus = xQueueReceive(xQueue, &massage, 0);
        curTime = esp_timer_get_time();

        if (xStatus == pdPASS)
        {
            var[current] = curTime - prevTime;
            current++;

            if (current == ITER)
            {
                output("Full message test", var, true);
                vTaskDelete(NULL);
            }
            
        }
    }
    vTaskDelete(NULL);
}

void vTask2 (void *pvParameters)
{
    int massage = 255;
  
    while (current <= ITER) 
    {
        prevTime = esp_timer_get_time();
        xQueueSendToBack(xQueue, &massage,0);
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
     xQueue = xQueueCreate(1, sizeof(int));

    if (xQueue != NULL) 
    {
        xTaskCreate(vTask1, "Task1", 10000, NULL, 1, NULL);
        xTaskCreate(vTask2, "Task2", 10000, NULL, 1, NULL);
    }
}