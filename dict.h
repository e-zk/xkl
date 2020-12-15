#ifndef XKL_DICT_H
#define XKL_DICT_H

#include <xcb/xcb_keysyms.h>

#define MOD_SUPER 0x40
#define MOD_ALT   0x8
#define MOD_CTRL  0x4
#define MOD_SHIFT 0x1

typedef struct {
	char *name;
	xcb_keysym_t keysym;
} keysym_dict_t;

const char* find_keysym(xcb_keysym_t keysym);
const char* mod_to_str(uint16_t mod);

#endif
