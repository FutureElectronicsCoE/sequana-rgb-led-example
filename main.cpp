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

// Return new color value by dimming the current color to x%
BGR24_color_t dim(BGR24_color_t color, uint8_t percent)
{
    BGR24_color_t new_color;

    new_color.r = color.r * percent / 100;
    new_color.g = color.g * percent / 100;
    new_color.b = color.b * percent / 100;

    return new_color;
}

#define sat(x)  (uint8_t)((x) > 255? 255 : (x) < 0? 0 : (x))

// This function generates color sequence. Each call to this function
// returns the next color in a sequence.
// Color sequence generation algorithm is such, that it provides
// smooth color swipe, keeping intensity roughly the same.
BGR24_color_t gen_color()
{
    const uint8_t step = 40;
    const uint8_t max = 200;
    static uint32_t phase = 0;
    static int32_t r = max;
    static int32_t g = max;
    static int32_t b = 0;
    static int32_t base_level = 6;

#define step_up(x)      do { if ((x += step) > max) x = max; } while(0)
#define step_down(x, v) do { x -= step; if (x < v) x = v;} while (0)

    switch (phase) {
        case 0:
            // swiping from reddish into blueish
            step_down(r, 0);
            g = base_level;
            step_up(b);
            if (r == 0) phase++;
            break;
        case 1:
            // swiping from blueish into greenish to level off blue
            r = 0;
            step_up(g);
            step_down(b, base_level);
            if (b == base_level) phase++;
            break;
        case 2:
            // swiping from greenish into reddish
            step_up(r);
            step_down(g, 0);
            b = base_level;
            if (g == 0) phase++;
            break;
        case 3:
            // swiping from reddish into blueish to level off red
            step_down(r, base_level);
            g = 0;
            step_up(b);
            if (r == base_level) phase++;
            break;
        case 4:
            // swiping from blueish into greenish
            r = base_level;
            step_up(g);
            step_down(b, 0);
            if (b == 0) phase++;
            break;
        case 5:
            // swiping from greenish into reddish to level off green
            step_up(r);
            step_down(g, base_level);
            b = 0;
            if (g == base_level) {
                phase = 0;
                base_level *= 2;
                if (base_level > 200) {
                    base_level = 6;
                }
                g = base_level;
            }
            break;
    }

    return BGR24_color_t(sat(b), sat(g), sat(r));
}


#define NUM_LEDS_LIGHTING   3

// Keeps indexes of LEDs that should be on in the next step.
uint32_t led_index[NUM_LEDS_LIGHTING] = {0, 1, 2};
// Current color.
BGR24_color_t current_col[NUM_LEDS_LIGHTING];

// main() runs in its own thread in the OS
int main() {
    int color_step = 0;

    leds.set_dma_usage(DMA_USAGE_ALWAYS);

    while (true) {
        // This defines phase relationship between rotating pattern
        // and color change.
        // As there are 8 steps for the whole circle (8 LEDs)
        // value of 6 means the color changes every 3/4 of rotation.
        if (++color_step == 6) {
            color_step = 0;
            current_col[2] = gen_color();
            current_col[1] = dim(current_col[2], 50);
            current_col[0] = dim(current_col[2], 10);
        }

        // Set up lighting pattern for current step.
        for (int i = 0; i < NUM_LEDS_LIGHTING; ++i) {
            leds.set_color(led_index[i], current_col[i]);
        }
        leds.refresh();

        wait_ms(50);
        // Turn off set LEDs, so we have a dark frame buffer again.
        // Must be done after delay, as with DMA refresh() operation
        // only starts LEDs updating from the frame buffer.
        leds.set_color(led_index[0], BGR24_color_t(0,0,0));

        // Update lighting LED indexes for the next step.
        for (int i = 0; i < NUM_LEDS_LIGHTING; ++i) {
            if (++led_index[i] == 8) led_index[i] = 0;
        }
    }
}

