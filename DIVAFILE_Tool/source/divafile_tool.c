// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


#define __C_DIVAFILE_TOOL
#include "divafile_tool.h"
#include "aes.h"
#include "endian_utils.h"

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <malloc.h>

#define is_16_aligned(addr) (((addr + 0xf) & (~0xf)) == addr)
#define BLOCK_SIZE						16
#define DIVAFILE_HDR_SIZE				16
#define DIVAFILE_OFFSET_LEN_PAYLOAD		0x8
#define DIVAFILE_OFFSET_LEN_PLAINTEXT	0xc

static const uint8_t DIVAFILE_KEY[BLOCK_SIZE] = { 'f','i','l','e',' ','a','c','c','e','s','s',' ','d','e','n','y' };
static const uint8_t DIVAFILE_MAGIC_NUMBER[8] = { 'D', 'I', 'V', 'A', 'F', 'I', 'L', 'E' };

uint32_t check_divafile(uint8_t* p_file, uint32_t size) { //sub_560738
	if (size < DIVAFILE_HDR_SIZE) {
		return 0;
	}

	const uint64_t*pmagic_num = (const uint64_t*)DIVAFILE_MAGIC_NUMBER;						// magic_number pointer
	uint32_t len_file_payload = LE32((uint32_t*)(p_file + DIVAFILE_OFFSET_LEN_PAYLOAD));		// offset 0x8, file_size_1, store in little-endian
	uint32_t len_file_plaintext = LE32((uint32_t*)(p_file + DIVAFILE_OFFSET_LEN_PLAINTEXT));	// offset 0xc, file_size_2, store in little-endian

	if (*((int64_t*)p_file) != *(pmagic_num))	// compare 'DIVAFILE'
		return 0;
	if (!is_16_aligned(len_file_payload))
		return 0;
	if (len_file_payload < len_file_plaintext)
		return 0;
	if ((len_file_payload + DIVAFILE_HDR_SIZE) > size)		// payload + sizeof(header) == size?
		return 0;
	return 1;
}

uint32_t decrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output) {
	uint32_t len_divafile = input->size;
	uint8_t* p_divafile = input->data;

	if (check_divafile(p_divafile, len_divafile) == 0) {
		fprintf_s(stderr, "invalid divafile.\n");
		return 0;
	}

	uint32_t len_divafile_payload = LE32((uint32_t*)(p_divafile + DIVAFILE_OFFSET_LEN_PAYLOAD));	// offset 0x8, store in little-endian
	uint8_t* p_divafile_payload = p_divafile + BLOCK_SIZE;

	uint32_t len_plaintext = LE32((uint32_t*)(p_divafile + DIVAFILE_OFFSET_LEN_PLAINTEXT));		// offset 0xc, store in little-endian
	uint8_t* p_plaintext = (uint8_t *)malloc(len_divafile_payload);

	int n = len_divafile_payload / BLOCK_SIZE;
	int cur_offset;
	int i;

	for (i = 0; i < n; ++i) {
		cur_offset = i * BLOCK_SIZE;
		AES128_ECB_decrypt(p_divafile_payload + cur_offset, DIVAFILE_KEY, p_plaintext + cur_offset);
	}

	output->data = p_plaintext;
	output->size = len_plaintext;

	return 1;
}

uint32_t encrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output) {
	uint8_t buffer[16];
	uint32_t len_plaintext = input->size;
	uint8_t* p_plaintext = input->data;


	uint32_t n_padding = (is_16_aligned(len_plaintext) ? 0 : 16 - (len_plaintext & 0xf));
	uint32_t len_divafile_payload = len_plaintext + n_padding;
	uint32_t len_divafile = len_divafile_payload + DIVAFILE_HDR_SIZE;
	uint8_t* p_divafile = (uint8_t*)malloc(len_divafile);
	uint8_t* p_divafile_payload = p_divafile + DIVAFILE_HDR_SIZE;

	if (p_divafile == NULL) {
		fprintf_s(stderr, "fail to allocate memory.\n");
		return 0;
	}

	if (!is_16_aligned(len_plaintext)) {
		memcpy(buffer, p_plaintext + (len_plaintext & ~0xf), len_plaintext & 0xf);
		memset(buffer + (len_plaintext & 0xf), 0, 16 - (len_plaintext & 0xf));
	}

	int cur_offset;
	int n = (len_plaintext & ~0xf) / BLOCK_SIZE;
	int i;
	for (i = 0; i < n; ++i) {
		cur_offset = i * BLOCK_SIZE;
		AES128_ECB_encrypt(p_plaintext + cur_offset, DIVAFILE_KEY, p_divafile_payload + cur_offset);
	}
	if (!is_16_aligned(len_plaintext)) {
		AES128_ECB_encrypt(buffer, DIVAFILE_KEY, p_divafile_payload + (i * BLOCK_SIZE));
	}

	memcpy(p_divafile, DIVAFILE_MAGIC_NUMBER, 8);
	WRITE_LE32(((uint32_t*)(p_divafile + DIVAFILE_OFFSET_LEN_PAYLOAD)), len_divafile_payload);
	WRITE_LE32(((uint32_t*)(p_divafile + DIVAFILE_OFFSET_LEN_PLAINTEXT)), len_plaintext);
	assert(check_divafile(p_divafile, len_divafile));

	output->size = len_divafile;
	output->data = p_divafile;

	return 1;
}
