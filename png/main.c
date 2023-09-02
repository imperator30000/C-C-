#include "return_codes.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define UNEXPECTED_CHUNK 16
#define SKIPPED_CHUNK 17
#if defined(ZLIB)

#include <zlib.h>

#elif defined(LIBDEFLATE)
#include <libdeflate.h>
#elif defined(ISAL)
#include <include/igzip_lib.h>
#else
#error "Defined library is not supported"
#endif

int check_name(unsigned char const *name, unsigned char const *check_name, int start, int length)
{
	for (int i = start; i < length + start; i++)
	{
		if (name[i - start] != check_name[i])
		{
			return ERROR_UNSUPPORTED;
		}
	}

	return SUCCESS;
}

int input_in_buffer(FILE *input, unsigned char *buffer, unsigned int length)
{
	size_t check = 0;
	size_t read;
	if (length > BUFFER_SIZE)
	{
		while (length - check > BUFFER_SIZE)
		{
			size_t n = fread(buffer, 1, BUFFER_SIZE, input);
			if (n != BUFFER_SIZE)
			{
				return ERROR_DATA_INVALID;
			}
			check += n;
		}
		read = fread(buffer, 1, length - check, input);
		if (read != length - check)
		{
			return ERROR_DATA_INVALID;
		}
	}
	else
	{
		read = fread(buffer, 1, length, input);
		if (read != length)
		{
			return ERROR_DATA_INVALID;
		}
	}
	return SUCCESS;
}

unsigned int bytes_to_int(unsigned char const *bytes, int start, int n)
{
	int result = bytes[start] << (n - 1) * 8;
	for (int i = n - 2; i >= 0; i--)
	{
		result |= (bytes[start + n - i - 1] << i * 8);
	}
	return result;
}

int get_name_and_len_chunk(FILE *png, unsigned char *buffer, size_t *len)
{
	int ret = input_in_buffer(png, buffer, 4);
	if (ret != SUCCESS)
	{
		return ret;
	}
	*len = bytes_to_int(buffer, 0, 4);
	ret = input_in_buffer(png, buffer, 4);
	if (ret != SUCCESS)
	{
		return ret;
	}
	return ret;
}

int check_png_signature(FILE *png, unsigned char *buffer)
{
	unsigned char ok_signature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	fread(buffer, 1, 8, png);
	return check_name(buffer, ok_signature, 0, 8);
}

int read_IHDR(FILE *png, size_t *w, size_t *h, unsigned int *bit_depth, unsigned int *color_type, unsigned int *deflate, unsigned int *filter, unsigned int *interlace, unsigned char *buffer)
{
	int ret = input_in_buffer(png, buffer, 13);
	if (ret != SUCCESS)
	{
		return ret;
	}
	*w = bytes_to_int(buffer, 0, 4);
	*h = bytes_to_int(buffer, 4, 4);
	*bit_depth = bytes_to_int(buffer, 8, 1);
	*color_type = bytes_to_int(buffer, 9, 1);
	*deflate = bytes_to_int(buffer, 10, 1);
	*filter = bytes_to_int(buffer, 11, 1);
	*interlace = bytes_to_int(buffer, 12, 1);
	input_in_buffer(png, buffer, 4);
	return SUCCESS;
}

int read_PLTE(FILE *png, size_t len, unsigned char pl[len], unsigned char *buffer, int *it_is_p5)
{
	int ret;
	if (len % 3 != 0 || len > 3 * 256)
	{
		return ERROR_DATA_INVALID;
	}
	for (size_t i = 0; i < len; i++)
	{
		ret = input_in_buffer(png, buffer, 1);
		if (ret != SUCCESS)
		{
			return ret;
		}
		pl[i] = buffer[0];
		if (i % 3 == 2)
		{
			*it_is_p5 &= pl[i - 2] == pl[i - 1] && pl[i] == pl[i - 1];
		}
	}
	ret = input_in_buffer(png, buffer, 4);
	if (ret != SUCCESS)
	{
		return ret;
	}
	return SUCCESS;
}

int read_IDAT(FILE *png, unsigned char *buffer, unsigned char *data, size_t start, size_t n)
{
	int ret;
	for (size_t i = 0; i < n; i++)
	{
		ret = input_in_buffer(png, buffer, 1);
		if (ret != SUCCESS)
		{
			return ret;
		}
		data[start + i] = buffer[0];
	}
	if (fseek(png, 4, SEEK_CUR) == 0)
	{
		return SUCCESS;
	}
	return ERROR_DATA_INVALID;
}

void filter_sub(unsigned char *data, int koef, size_t w, size_t i)
{
	for (size_t j = 1; j < w; j++)
	{
		for (size_t g = 0; g < koef; g++)
		{
			data[i * (w * koef + 1) + j * koef + 1 + g] += data[i * (w * koef + 1) + (j - 1) * koef + 1 + g];
		}
	}
}

void filter_up(unsigned char *data, int koef, size_t w, size_t i)
{
	if (i == 0)
	{
		return;
	}
	for (size_t j = 0; j < w; j++)
	{
		for (size_t g = 0; g < koef; g++)
		{
			data[i * (w * koef + 1) + j * koef + 1 + g] += data[(i - 1) * (w * koef + 1) + j * koef + 1 + g];
		}
	}
}

void filter_average(unsigned char *data, int koef, size_t w, size_t i)
{
	for (int j = 0; j < w; j++)
	{
		for (int g = 0; g < koef; g++)
		{
			int avg_left = (j != 0) ? data[i * (w * koef + 1) + (j - 1) * koef + 1 + g] : 0;
			int avg_upper = (i != 0) ? data[(i - 1) * (w * koef + 1) + j * koef + 1 + g] : 0;
			data[i * (w * koef + 1) + j * koef + 1 + g] += (avg_left + avg_upper) / 2;
		}
	}
}

unsigned char paeth_predictor(unsigned char a, unsigned char b, unsigned char c)
{
	int p = (a + b - c);
	int pa = abs(p - a);
	int pb = abs(p - b);
	int pc = abs(p - c);
	if (pa <= pb && pa <= pc)
		return a;
	if (pb <= pc)
		return b;
	return c;
}

void filter_paeth(unsigned char *data, int koef, size_t w, size_t i)
{
	for (int j = 0; j < w; j++)
	{
		for (int g = 0; g < koef; g++)
		{
			size_t left = 0;
			size_t up = 0;
			size_t up_left = 0;
			if (j > 0)
				left = data[i * (w * koef + 1) + (j - 1) * koef + 1 + g];
			if (i > 0)
				up = data[(i - 1) * (w * koef + 1) + j * koef + 1 + g];
			if (i > 0 && j > 0)
				up_left = data[(i - 1) * (w * koef + 1) + (j - 1) * koef + 1 + g];
			data[i * (w * koef + 1) + j * koef + 1 + g] += paeth_predictor(left, up, up_left);
		}
	}
}

void filters(unsigned char *data, int koef, size_t w, size_t h)
{
	for (size_t i = 0; i < h; i++)
	{
		if (data[i * (w * koef + 1)] == 1)
		{
			filter_sub(data, koef, w, i);
		}
		if (data[i * (w * koef + 1)] == 2)
		{
			filter_up(data, koef, w, i);
		}
		if (data[i * (w * koef + 1)] == 3)
		{
			filter_average(data, koef, w, i);
		}
		if (data[i * (w * koef + 1)] == 4)
		{
			filter_paeth(data, koef, w, i);
		}
	}
}

int to_PNM(unsigned char *data, unsigned char *pl, int it_is_p5, unsigned int color_type, FILE *pnm, const size_t w, const size_t h)
{
	if (it_is_p5)
	{
		fprintf(pnm, "P5\n%ld %ld\n%d\n", w, h, 255);
		for (size_t i = 0; i < h; i++)
		{
			for (size_t j = 0; j < w; j++)
			{
				if (color_type == 3)
				{
					fwrite(&pl[data[i * (w + 1) + j + 1] * 3], 1, 1, pnm);
				}
				else
				{
					fwrite(&data[i * (w + 1) + j + 1], 1, 1, pnm);
				}
			}
		}
	}
	else
	{
		fprintf(pnm, "P6\n%ld %ld\n%d\n", w, h, 255);
		for (size_t i = 0; i < h; i++)
		{
			for (size_t j = 0; j < w; j++)
			{
				if (color_type == 2)
					fwrite(&data[i * (w * 3 + 1) + j * 3 + 1], 1, 3, pnm);
				if (color_type == 3)
				{
					//                    printf("qwerty   %d\n", data[i * (w * 1 + 1) + j * 1 + 1]);
					fwrite(&pl[data[i * (w + 1) + j + 1] * 3], 1, 3, pnm);
				}
			}
		}
	}
	return 0;
}

int skip_chunk(FILE *png, size_t len)
{
	if (fseek(png, (long)len + 4, SEEK_CUR) != 0)
	{
		return ERROR_DATA_INVALID;
	}
	return SKIPPED_CHUNK;
}
int get_next_chunk(FILE *png, unsigned char *buffer, size_t *len, unsigned char *dream_name)
{
	int ret = get_name_and_len_chunk(png, buffer, len);
	if (ret != SUCCESS)
	{
		return ret;
	}
	ret = check_name(buffer, dream_name, 0, 4);
	if (ret != SUCCESS)
	{
		if (buffer[0] == 0x49 || buffer[0] == 0x50)
		{
			return UNEXPECTED_CHUNK;
		}
		return skip_chunk(png, *len);
	}
	return SUCCESS;
}
int convertor(FILE *png, unsigned char *buffer, char *pnm)
{
	unsigned char ok_IHDR[4] = { 0x49, 0x48, 0x44, 0x52 };
	unsigned char ok_PLTE[4] = { 0x50, 0x4c, 0x54, 0x45 };
	unsigned char ok_IDAT[4] = { 0x49, 0x44, 0x41, 0x54 };
	unsigned char ok_IEND[4] = { 0x49, 0x45, 0x4e, 0x44 };
	int it_is_p5 = 0;
	size_t out_len = 0;
	size_t len, w, h;
	unsigned int bit_depth, color_type, deflate, filter, interlace;
	int ret;
	ret = check_png_signature(png, buffer);
	if (ret != SUCCESS)
	{
		return ret;
	}
	ret = get_name_and_len_chunk(png, buffer, &len);
	if (ret != SUCCESS)
	{
		return ret;
	}
	if (len != 13)
	{
		return ERROR_DATA_INVALID;
	}
	if (check_name(buffer, ok_IHDR, 0, 4) != SUCCESS)
	{
		return ERROR_UNSUPPORTED;
	}
	ret = read_IHDR(png, &w, &h, &bit_depth, &color_type, &deflate, &filter, &interlace, buffer);
	if (ret != SUCCESS)
	{
		return ret;
	}
	it_is_p5 = color_type == 3 || color_type == 0;
	unsigned char pl[256 * 3];
	if (color_type != 3 && color_type != 0 && color_type != 2)
	{
		fprintf(stderr, "color type %d ", color_type);
		return ERROR_UNSUPPORTED;
	}
	if (bit_depth != 8)
	{
		fprintf(stderr, "bit depth %d ", bit_depth);
		return ERROR_UNSUPPORTED;
	}
	if (filter != 0)
	{
		fprintf(stderr, "filter %d ", filter);
		return ERROR_DATA_INVALID;
	}
	if (deflate != 0)
	{
		fprintf(stderr, "deflate %d ", deflate);
		return ERROR_DATA_INVALID;
	}
	if (interlace != 0 && interlace != 1)
	{
		fprintf(stderr, "interlace %d ", interlace);
		return ERROR_DATA_INVALID;
	}
	if (color_type == 3)
	{
		while (1)
		{
			ret = get_next_chunk(png, buffer, &len, ok_PLTE);
			if (ret == SUCCESS)
			{
				ret = read_PLTE(png, len, pl, buffer, &it_is_p5);
				if (ret != SUCCESS)
				{
					fprintf(stderr, "in PLTE error ");
					return ret;
				}
				break;
			}
			else if (ret == UNEXPECTED_CHUNK)
			{
				fprintf(stderr, "unexpected chunk ");
				return ERROR_DATA_INVALID;
			}
			else if (ret != SKIPPED_CHUNK)
			{
				return ret;
			}
		}
	}
	unsigned char *data = (unsigned char *)malloc(0);
	int final_data = 0;
	int start_data = 0;
	while (1)
	{
		ret = get_next_chunk(png, buffer, &len, ok_IDAT);
		if (ret == SUCCESS && !final_data)
		{
			start_data = 1;
			unsigned char *tmp = realloc(data, (out_len + len) * sizeof(unsigned char));
			if (tmp == NULL)
			{
				free(data);
				return ERROR_OUT_OF_MEMORY;
			}
			data = tmp;
			ret = read_IDAT(png, buffer, data, out_len, len);
			if (ret != SUCCESS)
			{
				free(data);
				return ret;
			}
			out_len += len;
		}
		else if (ret == UNEXPECTED_CHUNK)
		{
			if (check_name(buffer, ok_IEND, 0, 4) == SUCCESS)
			{
				if (start_data)
				{
					ret = skip_chunk(png, 4);
					if (ret != SKIPPED_CHUNK)
					{
						free(data);
						return ret;
					}

					break;
				}
			}
			fprintf(stderr, "unexpected chunk ");
			free(data);
			return ERROR_DATA_INVALID;
		}
		else if (ret == SKIPPED_CHUNK)
		{
			if (start_data)
			{
				final_data = 1;
			}
		}
		else
		{
			free(data);
			return ret;
		}
	}
	ret = fseek(png, 1, SEEK_CUR);
	if (ret)
	{
		return ERROR_DATA_INVALID;
	}
	int koef = 1;
	if (color_type == 2)
	{
		koef = 3;
	}
	unsigned char *data_dec = (unsigned char *)malloc(h * (w * koef + 1) * sizeof(unsigned char));
	if (data_dec == NULL)
	{
		free(data_dec);
		free(data);
		return ERROR_OUT_OF_MEMORY;
	}
	size_t ll = h * (w * koef + 1);
#if defined(ZLIB)
	ret = uncompress(data_dec, &ll, data, out_len);
	if (ret != Z_OK)
	{
		free(data_dec);
		free(data);
		fprintf(stderr, "decompression ");
		return ERROR_DATA_INVALID;
	}
#elif defined(LIBDEFLATE)
	struct libdeflate_decompressor *decompressor;
	decompressor = libdeflate_alloc_decompressor();
	ret = libdeflate_zlib_decompress(decompressor, data, out_len, data_dec, ll, NULL);
	if (ret != LIBDEFLATE_SUCCESS)
	{
		free(data_dec);
		free(data);
		libdeflate_free_decompressor(decompressor);
		fprintf(stderr, "decompression ");
		return ERROR_DATA_INVALID;
	}
	libdeflate_free_decompressor(decompressor);
#elif defined(ISAL)
	struct inflate_state state;
	isal_inflate_init(&state);
	state.avail_in = out_len;
	state.next_in = (uint8_t *)data;
	state.avail_out = ll;
	state.next_out = data_dec;
	state.crc_flag = IGZIP_ZLIB;
	ret = isal_inflate(&state);
	if (ret != ISAL_DECOMP_OK)
	{
		fprintf(stderr, "decompression ");
		return ERROR_DATA_INVALID;
	}

#endif
	free(data);
	filters(data_dec, koef, w, h);
	FILE *out = fopen(pnm, "wb");
	if (out == NULL)
	{
		fprintf(stderr, "cannot open file %s", pnm);
		return ERROR_CANNOT_OPEN_FILE;
	}
	to_PNM(data_dec, pl, it_is_p5, color_type, out, w, h);
	fclose(out);
	free(data_dec);
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Error: cnt args != 2 \n");
		return ERROR_PARAMETER_INVALID;
	}
	FILE *fp = fopen(argv[1], "rb");

	if (fp == NULL)
	{
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		return ERROR_CANNOT_OPEN_FILE;
	}

	unsigned char buffer[BUFFER_SIZE];


	int ret = convertor(fp, buffer, argv[2]);
	fclose(fp);
	if (ret != SUCCESS)
	{
		if (ret == ERROR_OUT_OF_MEMORY)
		{
			fprintf(stderr, "out of memory ");
		}
		else if (ret == ERROR_DATA_INVALID)
		{
			fprintf(stderr, "data invalid ");
		}
		else if (ret == ERROR_UNSUPPORTED)
		{
			fprintf(stderr, "unsupported ");
		}
		return ret;
	}
	return ret;
}
