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
EXTERN uint8_t* get_file_content(const char *path, size_t *len);
EXTERN int write_to_file(const char* path, uint8_t* data, size_t len);

#undef EXTERN

#endif
