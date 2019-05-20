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

static xQueueHandle xQueue = NULL;


void vTask(void *pvParameters)
{
    int massageSend = 1;
    int massageReceive = 0;
    portBASE_TYPE xStatus;

    while (current <= ITER) 
    {
        prevTime = esp_timer_get_time();
        xQueueSendToBack(xQueue, &massageSend, 0);
        curTime = esp_timer_get_time();

        xStatus = xQueueReceive(xQueue, &massageReceive, 0);

        if (xStatus == pdPASS)
        {
            if (massageReceive == 0)
            {
                error("Massage failed!");
            }
            massageReceive = 0;

            var[current] = curTime - prevTime;
            current++;
            if (current == ITER)
            {
                output("Send message test", var, ITER);
                vTaskDelete(NULL);
            }
            
        }
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    xQueue = xQueueCreate(1, sizeof(int));

    if (xQueue != NULL) 
    {
        xTaskCreate(vTask, "Task", 10000, NULL, 1, NULL);
    }
}
