/* Copyright (c) 2013, Bastien Dejean
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* note:
 * the marjority of this code comes from github.com/baskerville/sxhkd parse.c
 * as such the original copyright notice is provided above.
 */

#include <stdlib.h>
#include "parse.h"
#include "dict.h"
#include "locales.h"

int16_t
modfield_from_keysym(xcb_keysym_t keysym)
{
	uint16_t modfield = 0;
	xcb_keycode_t *keycodes = NULL;
	if ((keycodes = keycodes_from_keysym(keysym)) != NULL) {
		for (xcb_keycode_t *k = keycodes; *k != XCB_NO_SYMBOL; k++)
			modfield |= modfield_from_keycode(*k);
	}
	free(keycodes);
	return modfield;
}

int16_t
modfield_from_keycode(xcb_keycode_t keycode)
{
	uint16_t modfield = 0;
	xcb_keycode_t *mod_keycodes = NULL;
	xcb_get_modifier_mapping_reply_t *reply = NULL;
	if ((reply = xcb_get_modifier_mapping_reply(dpy, xcb_get_modifier_mapping(dpy), NULL)) != NULL && reply->keycodes_per_modifier > 0) {
		if ((mod_keycodes = xcb_get_modifier_mapping_keycodes(reply)) != NULL) {
			unsigned int num_mod = xcb_get_modifier_mapping_keycodes_length(reply) / reply->keycodes_per_modifier;
			for (unsigned int i = 0; i < num_mod; i++) {
				for (unsigned int j = 0; j < reply->keycodes_per_modifier; j++) {
					xcb_keycode_t mkc = mod_keycodes[i * reply->keycodes_per_modifier + j];
					if (mkc == XCB_NO_SYMBOL)
						continue;
					if (keycode == mkc)
						modfield |= (1 << i);
				}
			}

		}
	}
	free(reply);
	return modfield;
}

xcb_keycode_t *
keycodes_from_keysym(xcb_keysym_t keysym)
{
	xcb_setup_t const *setup;
	unsigned int num = 0;
	xcb_keycode_t *result = NULL, *result_np = NULL;

	if ((setup = xcb_get_setup(dpy)) != NULL) {
		xcb_keycode_t min_kc = setup->min_keycode;
		xcb_keycode_t max_kc = setup->max_keycode;

		/* We must choose a type for kc other than xcb_keycode_t whose size
		 * is 1, otherwise, since max_kc will most likely be 255, if kc == 255,
		 * kc++ would be 0 and the outer loop would start over ad infinitum */
		for(unsigned int kc = min_kc; kc <= max_kc; kc++)
			for(unsigned int col = 0; col < KEYSYMS_PER_KEYCODE; col++) {
				xcb_keysym_t ks = xcb_key_symbols_get_keysym(symbols, kc, col);
				if (ks == keysym) {
					num++;
					result_np = realloc(result, sizeof(xcb_keycode_t) * (num + 1));
					if (result_np == NULL) {
						free(result);
						return NULL;
					}
					result = result_np;
					result[num - 1] = kc;
					result[num] = XCB_NO_SYMBOL;
					break;
				}
			}
	}
	return result;
}

