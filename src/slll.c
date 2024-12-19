#include "c.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.c"
#include "language/parser/parser.c"

static void help(void);

int main(int argc, char **argv) {
	bool show_help = 0;

	if (argc <= 1) {
		show_help = true;
	} else {
		FILE *f = fopen(argv[1], "rb");
		if (f == NULL) {
			fprintf(stderr, "Failed to open file or file does not exist. File: \"%s\"\n", argv[1]);
			show_help = true;
		} else {
			String source;

			fseek(f, 0, SEEK_END);
			source.length = ftell(f);
			fseek(f, 0, SEEK_SET);
			source.buffer = malloc(source.length);
			fread(source.buffer, source.length, 1, f);
			LanguageParse(source);
		}
	}

	if (show_help) {
		help();
	}
	return 0;
}



static void help() {
	puts("lole");
}
