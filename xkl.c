#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_event.h>
#include <xcb/xcb_keysyms.h>

#include "xkl.h"
#include "parse.h"
#include "dict.h"
#include "config.h"

xcb_connection_t *dpy;
xcb_screen_t *screen;
xcb_window_t root;
xcb_key_symbols_t *symbols;

uint16_t num_lock;
uint16_t caps_lock;
uint16_t scroll_lock;

int
main(void)
{
	setup();
	grab_all_keys();
	xcb_flush(dpy);

	bool running = true;
	while (running) {

		xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
		xcb_allow_events(dpy, XCB_ALLOW_REPLAY_KEYBOARD, XCB_CURRENT_TIME);
		xcb_flush(dpy);

		if (ev && ((ev->response_type & ~0x80) == XCB_KEY_PRESS)) {
			xcb_keysym_t keysym = XCB_NO_SYMBOL;
			uint16_t modfield = 0;
 			uint16_t lockfield = num_lock | caps_lock | scroll_lock;

         		xcb_key_press_event_t *e = (xcb_key_press_event_t *)ev;
			xcb_keycode_t keycode = e->detail;
			modfield = e->state;
			modfield &= ~lockfield & MOD_STATE_FIELD;
			keysym = xcb_key_symbols_get_keysym(symbols, keycode, 0);

			fprintf(stdout, "%02X%c", modfield, modsep_char);
			fprintf(stdout, "%06X", keysym);
			fprintf(stdout, "\n");
			fflush(stdout);
      		}

		if (ev != NULL) {
			free(ev);
		}

 		if (xcb_connection_has_error(dpy)) {
			warn("the server closed the connection.\n");
			running = false;
		}

	}
	
	ungrab_all_keys();
	xcb_key_symbols_free(symbols);
	xcb_disconnect(dpy);
	return 0;
}

void
setup(void)
{
	// open connection to X display
	dpy = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(dpy))
		err(1, "cannot open display.\n");

	// alloc symbols
	symbols = xcb_key_symbols_alloc(dpy);

	// get screen
	screen = NULL;
	screen = xcb_setup_roots_iterator(xcb_get_setup (dpy)).data;
	if (screen == NULL)
		err(1, "cannot acquire screen.\n");

	// get root win
	root = screen->root;

	num_lock = modfield_from_keysym(0xff7f);
	caps_lock = XCB_MOD_MASK_LOCK;
	scroll_lock = modfield_from_keysym(0xff14);
}

void
grab_keys(uint16_t modfield)
{
	xcb_generic_error_t *err = xcb_request_check(dpy,
	    xcb_grab_key_checked(dpy,
                0,
	        root,
	        modfield,
	        XCB_GRAB_ANY,
	        XCB_GRAB_MODE_ASYNC,
	        XCB_GRAB_MODE_SYNC
	    )
	);
	if (err != NULL) {
		warn("error %u encountered.\n", err->error_code);
	}
	free(err);
}

void
grab_all_keys(void)
{

	// TODO
	
	grab_keys(XCB_NONE);
	grab_keys(XCB_MOD_MASK_SHIFT);

	//if (num_lock != 0)
	//	grab_keys(XCB_NONE | num_lock);
	//if (caps_lock != 0)
	//	grab_keys(XCB_NONE | caps_lock);
	//if (scroll_lock != 0)
	//	grab_keys(XCB_NONE | scroll_lock);
	//if (num_lock != 0 && caps_lock != 0)
	//	grab_keys(XCB_NONE | num_lock | caps_lock);
	//if (caps_lock != 0 && scroll_lock != 0)
	//	grab_keys(XCB_NONE | scroll_lock | caps_lock);
	//if (num_lock != 0 && scroll_lock != 0)
	//	grab_keys(XCB_NONE | scroll_lock | num_lock);
	//if (num_lock != 0 && caps_lock != 0 && scroll_lock != 0)
	//	grab_keys(XCB_NONE | scroll_lock | num_lock | caps_lock );
}

void
ungrab_all_keys(void)
{
	xcb_ungrab_key(dpy, XCB_GRAB_ANY, root, XCB_BUTTON_MASK_ANY);
	xcb_flush(dpy);
}

