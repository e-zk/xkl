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
void grab_keys(uint16_t modfield);
void grab_all_keys(void);
void ungrab_all_keys(void);

#endif
