/*
 * Mbed OS example program for Future Electronics Sequana board.
 *
 * Copyright (c) 2018 Future Electronics
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This example demonstrates used of ROHM BD2808 24-channel LED Controller
// and its driver library on Sequana board.

#include "mbed.h"
#include "BD2808.h"

BD2808 leds;

BGR24_color_t dim(BGR24_color_t color, uint8_t percent)
{
    BGR24_color_t new_color;

    new_color.r = color.r * percent / 100;
    new_color.g = color.g * percent / 100;
    new_color.b = color.b * percent / 100;

    return new_color;
}

#define sat(x)  (uint8_t)((x) > 255? 255 : (x) < 0? 0 : (x))

BGR24_color_t gen_color()
{
    const uint8_t step = 20;
    const uint8_t max = 200;
    static uint32_t i = 0;
    static uint32_t j = 0;
    static int32_t r = max;
    static int32_t g = max;
    static int32_t b = 0;
    static uint8_t pull = 0;

    if (++j == 6)
    {
        j = 0;
        if ( i < 10)
        {
            b += step;
            r -= step;
        }
        else if (i < 20)
        {
            r += step;
            g -= step;
        }
        else if (i < 30)
        {
            g += step;
            b -= step;
        }

        if (++i >= 30) {
            i = 0;
        }

        if (i == 0) {
            if ((pull += 20) > 180) {
                pull = 0;
            }
            g = max - pull;
            r = max ;//+ pull/2;
            b = pull / 2;
        } else if (i == 10) {
            b = max - pull;
            r = pull/2;
            g = max ;//+ pull/2;
        } else if (i == 20) {
            r = max - pull;
            g = pull / 2;
            b = max ;//+ pull/2;
        }
    }

    return BGR24_color_t(sat(b), sat(g), sat(r));
}


#define NUM_LEDS_LIGHTING   3

uint32_t led_index[NUM_LEDS_LIGHTING] = {0, 1, 2};
BGR24_color_t current_col[NUM_LEDS_LIGHTING];

// main() runs in its own thread in the OS
int main() {

    leds.set_dma_usage(DMA_USAGE_ALWAYS);

    while (true) {
        for (int i = 0; i < NUM_LEDS_LIGHTING; ++i) {
            leds.set_color(led_index[i], current_col[i]);
        }
        leds.refresh();

        wait_ms(50);
        leds.set_color(led_index[0], BGR24_color_t(0,0,0));

        for (int i = 0; i < NUM_LEDS_LIGHTING; ++i) {
            if (++led_index[i] == 8) led_index[i] = 0;
        }
        current_col[2] = gen_color();
        current_col[1] = dim(current_col[2], 50);
        current_col[0] = dim(current_col[2], 15);
    }
}

