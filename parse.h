#ifndef XKL_PARSE_H
#define XKL_PARSE_H

#include <xcb/xcb_keysyms.h>
#include "xkl.h"

int16_t modfield_from_keycode(xcb_keycode_t keycode);
int16_t modfield_from_keysym(xcb_keysym_t keysym);
xcb_keycode_t *keycodes_from_keysym(xcb_keysym_t keysym);

#endif
