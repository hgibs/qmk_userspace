/* Copyright 2020-2021 doodboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum custom_keycodes {
    KVM_C1 = SAFE_RANGE,
    KVM_C2,
    KVM_C3,
    KVM_C4,
};

#define MY_MACRO_DELAY_TIME 50

// KVM macros have to be on keyboard port it looks like

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KVM_C1:
        if (record->event.pressed) {
            // SEND_STRING_DELAY(X_TAP_KVM_MAGIC_KEY X_TAP_KVM_MAGIC_KEY "1" SS_TAP(X_ENTER), MY_MACRO_DELAY_TIME);
            SEND_STRING_DELAY(SS_TAP(X_F14) SS_TAP(X_F14) "1" SS_TAP(X_ENTER), MY_MACRO_DELAY_TIME);
        } else {
            // when keycode is released
        }
        break;
    case KVM_C2:
        if (record->event.pressed) {
            SEND_STRING_DELAY(SS_TAP(X_F14) SS_TAP(X_F14) "2" SS_TAP(X_ENTER), MY_MACRO_DELAY_TIME);
        } else {
            // when keycode is released
        }
        break;
    case KVM_C3:
        if (record->event.pressed) {
            SEND_STRING_DELAY(SS_TAP(X_F14) SS_TAP(X_F14) "3" SS_TAP(X_ENTER), MY_MACRO_DELAY_TIME);
        } else {
            // when keycode is released
        }
        break;
    case KVM_C4:
        if (record->event.pressed) {
            SEND_STRING_DELAY(SS_TAP(X_F14) SS_TAP(X_F14) "4" SS_TAP(X_ENTER), MY_MACRO_DELAY_TIME);
        } else {
            // when keycode is released
        }
        break;
  }
  return true;
};

#include "keymap.h"

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

// static void render_logo(void) {
//     static const char PROGMEM raw_logo[] = {
//         0,  0,  0,  0,  0,128, 64, 32, 32, 16,  8,  8,  8,  4,  4,  4,  4,  4,  4,  8,  8,  8, 16, 32, 32, 64,128,  0,  0,  0,  0,  0,  0,  0,224, 28,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2, 28,224,  0,  0,  0,  0,  7, 56, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 56,  7,  0,  0,  0,  0,  0,  0,  0,  1,  2,  4,  4,  8, 16, 16, 16, 32, 32, 32, 32, 32, 32, 16, 16, 16,  8,  4,  4,  2,  1,  0,  0,  0,  0,  0,
//     };
//     // oled_write_raw_P(raw_logo, sizeof(raw_logo));
//     oled_write_P(raw_logo, false);
// }
//

// static void render_logo(void) {
//     static const char PROGMEM qmk_logo[] = {
//             0,  0,192,192,192,192,192,192,192,248,248, 30, 30,254,254,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  3,  3,255,255,255,255,255,255,255,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,127,255,255,255,255,255,159,159,135,135,129,129,129, 97, 97, 25, 25,  7,  7,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1, 97, 97,127,  1,  1, 97, 97,127,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0
//         };

//     oled_write_P(qmk_logo, false);
// }
//
// WPM-responsive animation stuff here
#define IDLE_FRAMES 2
#define IDLE_SPEED 40 // below this wpm value your animation will idle

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 636 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;

// Credit to u/Pop-X- for the initial code. You can find his commit here https://github.com/qmk/qmk_firmware/pull/9264/files#diff-303f6e3a7a5ee54be0a9a13630842956R196-R333.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,192,192,192,192,192,192,192,248,248, 30, 30,254,254,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  3,  3,255,255,255,255,255,255,255,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,127,255,255,255,255,255,159,159,135,135,129,129,129, 97, 97, 25, 25,  7,  7,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1, 97, 97,127,  1,  1, 97, 97,127,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        {
        0,  0,128,128,128,128,128,128,128,240,240, 60, 60,252,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  7,  7,  7,  7,  7,255,255,254,254,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,255, 63, 63, 15, 15,  3,  3,  3,195,195, 51, 51, 15, 15,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3, 99, 99,127,  3,  3, 99, 99,127,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0
        }
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

    if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animation_phase();
    }
}

bool oled_task_user(void) {
    // render_logo();
    render_anim();
    oled_set_cursor(0,6);
    oled_write_P(PSTR("DUCK\nBOARD\n"), false);
    oled_write_P(PSTR("-----\n"), false);
    // Host Keyboard Layer Status
    oled_write_P(PSTR("MODE\n"), false);
    oled_write_P(PSTR("\nXXXXX\n"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("NUMPD\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("TEAMS\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("KVM\n"), false);
            break;
        case 11:
            oled_write_P(PSTR(">SEL<\n"), false);
            break;
        default:
            oled_write_P(PSTR("???\n"), false);
            break;
    }
    oled_set_brightness(100);
    return false;
}

void oled_render_boot(bool bootloader) {
    oled_clear();
    for (int i = 0; i < 16; i++) {
        oled_set_cursor(0, i);
        if (bootloader) {
            oled_write_P(PSTR("Await\ning \nnew \nfirmw\nare"), false);
        } else {
            oled_write_P(PSTR("Rebooting"), false);
        }
    }

    oled_render_dirty(true);
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);
    return false;
}

#endif

void keyboard_post_init_user(void) {
  //Customise these values to debug
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
