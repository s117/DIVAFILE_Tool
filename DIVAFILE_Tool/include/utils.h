// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


#ifndef __H_UTILS
#define __H_UTILS

#include <stdint.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __C_UTILS
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN int end_with(const char * str, const char * end);
EXTERN uint8_t* get_file_content(const char *path, uint32_t *len);
EXTERN int write_to_file(const char* path, uint8_t* data, uint32_t len);

#undef EXTERN

#endif
