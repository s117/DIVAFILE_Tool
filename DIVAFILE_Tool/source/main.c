// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "divafile_tool.h"

int main(int argc, char* argv[]) {
	struct data_descriptor input;
	struct data_descriptor output;
	if ((argc == 2) && ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0))) {
		fprintf(stdout, "DIVAFILE tool 0.2 -- S117<admin@0x10c.pw>, Bitman Lab.\n");
		exit(0);
	}
	if (argc != 4) {
		fprintf(stderr, "useage: %s [e/c] [src] [dst]\n", argv[0]);
		fprintf(stderr, "\te - extract DIVAFILE\n");
		fprintf(stderr, "\tc - create DIVAFILE\n");
		exit(1);
	}
	if (strcmp(argv[1], "e") == 0 || strcmp(argv[1], "E") == 0) { // extract
		input.data = get_file_content(argv[2], &(input.size));
		if (input.data == NULL) {
			fprintf(stderr, "fail to open source file: %s\n", argv[2]);
			exit(1);
		}
		if (decrypt_divafile(&input, &output) == 0) {
			fprintf(stderr, "extract fail.\n");
			exit(1);
		}
		if (write_to_file(argv[3], output.data, output.size) == 0) {
			fprintf(stderr, "fail to write file: %s\n", argv[3]);
			exit(1);
		}
		free(input.data);
		free(output.data);
		fprintf(stdout, "extract complete, %s -> %s", argv[2], argv[3]);
	} else if (strcmp(argv[1], "c") == 0 || strcmp(argv[1], "C") == 0) { // extract
		input.data = get_file_content(argv[2], &(input.size));
		if (input.data == NULL) {
			fprintf(stderr, "fail to open source file: %s\n", argv[2]);
			exit(1);
		}
		if (encrypt_divafile(&input, &output) == 0) {
			fprintf(stderr, "create fail.\n");
			exit(1);
		}
		if (write_to_file(argv[3], output.data, output.size) == 0) {
			fprintf(stderr, "fail to write file: %s\n", argv[3]);
			exit(1);
		}
		free(input.data);
		free(output.data);
		fprintf(stdout, "create complete, %s -> %s", argv[2], argv[3]);
	} else {
		fprintf(stderr, "useage: %s [e/c] [src] [dst]\n", argv[0]);
		fprintf(stderr, "\te - extract DIVAFILE\n");
		fprintf(stderr, "\tc - create DIVAFILE\n");
		exit(1);
	}
}
