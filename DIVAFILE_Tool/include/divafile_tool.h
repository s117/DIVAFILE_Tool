// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


#ifndef __H_DIVAFILE_TOOL
#define __H_DIVAFILE_TOOL

#include <stdint.h>
#define IN
#define OUT

#ifndef __C_DIVAFILE_TOOL
#define EXTERN extern
#else
#define EXTERN
#endif

struct data_descriptor {
	uint8_t *data;
	uint32_t size;
};

EXTERN uint32_t decrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output);
EXTERN uint32_t encrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output);
EXTERN uint32_t check_divafile(uint8_t* file, uint32_t size);

#undef EXTERN

#endif
