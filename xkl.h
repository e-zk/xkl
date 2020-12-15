#ifndef XKL_XKL_H
#define XKL_XKL_H

#include <xcb/xcb_keysyms.h>

#define MOD_STATE_FIELD 255
#define KEYSYMS_PER_KEYCODE 4

extern xcb_connection_t *dpy;
extern xcb_window_t root;
extern xcb_key_symbols_t *symbols;
extern xcb_window_t root;

extern uint16_t num_lock;
extern uint16_t caps_lock;
extern uint16_t scroll_lock;

void setup(void);
int16_t modfield_from_keysym(xcb_keysym_t keysym);
int16_t modfield_from_keycode(xcb_keycode_t keycode);
xcb_keycode_t *keycodes_from_keysym(xcb_keysym_t keysym);
const char* mod_to_str(uint16_t mod);

#endif
