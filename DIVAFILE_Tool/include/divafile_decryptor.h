#ifndef __H_DIVAFILE_DECRYPTOR
#define __H_DIVAFILE_DECRYPTOR

#include <stdint.h>
#define IN
#define OUT

struct data_descriptor {
	uint8_t *data;
	uint32_t size;
};

uint32_t decrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output);
uint32_t encrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output);
int check_divafile(uint8_t* file, uint32_t size);

#endif
