#define __C_DIVAFILE_DECRYPTOR
#include "divafile_decryptor.h"
#include "aes.h"
#include "endian_utils.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <malloc.h>

#define is_16_aligned(addr) (((addr + 0xf) & (~0xf)) == addr)

static const unsigned char DIVAFILE_MAGIC_NUMBER[8] = { 'D', 'I', 'V', 'A', 'F', 'I', 'L', 'E' };

int check_divafile(uint8_t* file, uint32_t size) { //sub_560738

	if (size < 16) {	// 0x10 == 16
		return 0;
	}

	const uint64_t*pmagic_num = (const uint64_t*)DIVAFILE_MAGIC_NUMBER;	// magic_number pointer
	uint32_t file_payload_len = LE32((uint32_t*)(file + 8));	// offset 0x8, file_size_1, store in little-endian
	uint32_t file_plaintext_len = LE32((uint32_t*)(file + 12));	// offset 0xc, file_size_2, store in little-endian


	if (*((int64_t*)file) != *(pmagic_num)) // compare 'DIVAFILE'
		return 0;
	//if (((file_payload_len1_bit_reorder + 0xf) & (~0xf)) != file_payload_len1_bit_reorder)	// check file size 16-byte align
	if (!is_16_aligned(file_payload_len))
		return 0;
	if (file_payload_len < file_plaintext_len)
		return 0;
	if ((file_payload_len + 16) > size)	// payload + sizeof(header) == size?
		return 0;
	return 1;
}

#define BLOCK_SIZE 16
static const uint8_t key_divafile[BLOCK_SIZE] = { 'f','i','l','e',' ','a','c','c','e','s','s',' ','d','e','n','y' };

uint32_t decrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output) {
	uint32_t len_divafile = input->size;
	uint8_t* p_divafile = input->data;

	if (check_divafile(p_divafile, len_divafile) == 0) {
		fprintf_s(stderr, "invalid divafile.\n");
		return 0;
	}

	uint32_t len_divafile_payload = LE32((uint32_t*)(p_divafile + 8));	// offset 0x8, store in little-endian
	uint8_t *p_divafile_payload = p_divafile + BLOCK_SIZE;

	uint32_t len_plaintext = LE32((uint32_t*)(p_divafile + 0xc));	// offset 0xc, store in little-endian
	uint8_t* p_plaintext = (uint8_t *)malloc(len_plaintext + (is_16_aligned(len_plaintext) ? 0 : 16 - (len_plaintext & 15)));

	int n = len_divafile_payload / BLOCK_SIZE;
	int cur_offset;
	int i;

	for (i = 0; i < n; ++i) {
		cur_offset = i * BLOCK_SIZE;
		AES128_ECB_decrypt(p_divafile_payload + cur_offset, key_divafile, p_plaintext + cur_offset);
	}

	output->data = p_plaintext;
	output->size = len_plaintext;

	return 1;
}

#define DIVAFILE_HEADER_LEN 16

uint32_t encrypt_divafile(IN struct data_descriptor *input, OUT struct data_descriptor *output) {
	uint8_t buff[16];
	uint32_t len_plaintext = input->size;
	uint8_t* p_plaintext = input->data;


	uint32_t n_padding = (is_16_aligned(len_plaintext) ? 0 : 16 - (len_plaintext & 15));
	uint32_t len_divafile_payload = len_plaintext + n_padding;
	uint32_t len_divafile = len_divafile_payload + DIVAFILE_HEADER_LEN;
	uint8_t* p_divafile = (uint8_t*)malloc(len_divafile);
	uint8_t* p_divafile_payload = p_divafile + DIVAFILE_HEADER_LEN;

	if (p_divafile == NULL) {
		fprintf_s(stderr, "fail to allocate memory.\n");
		return 0;
	}

	memset(buff, 0, sizeof(buff));
	if (!is_16_aligned(len_plaintext)) {
		memcpy(buff, p_plaintext + (len_plaintext & ~15), len_plaintext & 15);
	}

	int cur_offset;
	int n = (len_plaintext & ~15) / BLOCK_SIZE;
	int i;
	for (i = 0; i < n; ++i) {
		cur_offset = i * BLOCK_SIZE;
		AES128_ECB_encrypt(p_plaintext + cur_offset, key_divafile, p_divafile_payload + cur_offset);
	}
	if (!is_16_aligned(len_plaintext)) {
		AES128_ECB_encrypt(buff, key_divafile, p_divafile_payload + i*BLOCK_SIZE);
	}

	memcpy(p_divafile, DIVAFILE_MAGIC_NUMBER, 8);
	WRITE_LE32(((uint32_t*)(p_divafile + 8)), len_divafile_payload);
	WRITE_LE32(((uint32_t*)(p_divafile + 0xc)), len_plaintext);
	assert(check_divafile(p_divafile, len_divafile));

	output->size = len_divafile;
	output->data = p_divafile;

	return 1;
}
