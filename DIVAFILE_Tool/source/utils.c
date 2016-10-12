// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


#define __C_UTILS
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t* get_file_content(const char *path, uint32_t *len) {
	uint8_t *p_buffer = NULL;
	uint32_t file_len = 0;
	FILE *fp = fopen(path, "rb");

	if (fp == NULL)
		return NULL;

	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	rewind(fp);

	p_buffer = (uint8_t*)malloc(sizeof(uint8_t) * file_len);
	if (p_buffer == NULL) {
		fclose(fp);
		fprintf(stderr, "fail to allocate memory.\n");
		return 0;
	}

	file_len = fread(p_buffer, sizeof(uint8_t), file_len, fp);
	fclose(fp);

	*len = file_len;
	return p_buffer;
}

int write_to_file(const char* path, uint8_t* data, uint32_t len) {
	FILE* fp = fopen(path, "wb");
	if (fp == NULL)
		return 0;
	if (fwrite(data, 1, len, fp) != len) {
		return 0;
	}

	fflush(fp);
	fclose(fp);
	return 1;
}

int end_with(const char * str, const char * end) {
	int result = 0;

	if (str != NULL && end != NULL) {
		int64_t l1 = strlen(str);
		int64_t l2 = strlen(end);
		if (l1 >= l2) {
			if (strcmp(str + l1 - l2, end) == 0) {
				result = 1;
			}
		}
	}
	return result;
}
