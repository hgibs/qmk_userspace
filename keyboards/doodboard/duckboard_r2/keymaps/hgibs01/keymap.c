
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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 TG(1),   KC_PSLS, KC_PAST, KC_PMNS,
                 KC_7,    KC_8,    KC_9,    KC_PPLS,
                 KC_4,    KC_5,    KC_6,    KC_PPLS,
        KC_MUTE, KC_1,    KC_2,    KC_3,    KC_ENT,
        KC_BSPC, KC_0,    KC_0,    KC_DOT,  KC_ENT),

    [1] = LAYOUT(
                 TG(1),   KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_HOME, KC_UP,   KC_PGUP, KC_TRNS,
                 KC_LEFT, KC_TRNS, KC_RGHT, KC_TRNS,
        KC_TRNS, KC_END,  KC_DOWN, KC_PGDN, KC_TRNS,
        TG(2),   KC_TRNS, KC_INS,  KC_DEL,  KC_TRNS),

    [2] = LAYOUT(
                 KC_TRNS, UG_TOGG, UG_NEXT, KC_TRNS,
                 UG_HUEU, UG_SATU, UG_VALU, KC_TRNS,
                 UG_HUED, UG_SATD, UG_VALD, KC_TRNS,
        QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        TG(2),   QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS),
};

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
#include "animation.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

static int c_frame = 0;
bool first_render = true;

static void render_anim(void) {
    if (first_render) {
        oled_write_raw_P( frame, ANIM_SIZE);
        first_render = 0;
    } else {
        change_frame_bytewise(c_frame);
    }
    c_frame = c_frame+1 > IDLE_FRAMES ? 0 : c_frame+1;
}

bool oled_task_user(void) {
    render_anim();
    oled_set_cursor(0,6);
    oled_write_P(PSTR("DUCK\nBOARD\n"), false);
    oled_write_P(PSTR("-----\n"), false);
    // Host Keyboard Layer Status
    oled_write_P(PSTR("MODE\n"), false);
    oled_write_P(PSTR("\n"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("BASE\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("FUNC\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("RGB\n"), false);
            break;
    }
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
