#include <stdlib.h>
#include <stdio.h>

#include "dict.h"
#include "config.h"

int
main(void) {
	unsigned int key;
	uint16_t mod;

	// format the format string using the separator defined in config.h
	int sz = 11;
	char *format_str = (char*) malloc(sz*sizeof(char));
	snprintf(format_str, sz, "%%02hX%c%%06X", modsep_char);

	while (fscanf(stdin, format_str, &mod, &key) == 2) {
		//printf("%02hX-%s\n", mod, find_keysym(key));
		printf("%s-%s\n", mod_to_str(mod), find_keysym(key));
	}

	return 0;
}
