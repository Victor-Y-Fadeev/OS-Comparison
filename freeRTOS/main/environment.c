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

#include "environment.h"


void output(const char *str, const int *var, const bool log)
{
    printf("\r\n---%s---\r\n", str);
    printf("CPU frequency: %d MHz\r\n", XT_CLOCK_FREQ / 1000000);
    printf("Tick rate: %d Hz\r\n", configTICK_RATE_HZ);
    printf("Iterations: %d\r\n", ITER);

    int average = 0;
    for (int i = 1; i < ITER; i++)
    {
        average += var[i];
    }
    printf("Average: %.2f us\r\n\r\n", ((double) average) / (ITER - 1));

    if (log)
    {
        for (int i = 0; i < ITER; i++)
        {
            printf("#%d switch - %d microsecond\r\n", i, var[i]);
        }
    }
}

void single(const char *str, const double average)
{
    printf("\r\n---%s---\r\n", str);
    printf("CPU frequency: %d MHz\r\n", XT_CLOCK_FREQ / 1000000);
    printf("Tick rate: %d Hz\r\n", configTICK_RATE_HZ);
    printf("Average: %.2f us\r\n\r\n", average);
}

void error(const char *str)
{
    printf("Error: %s\r\n", str);
}
