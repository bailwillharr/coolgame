#include <stdio.h>
#include <stdlib.h>

#include "spng/spng.h"

#include "texture.h"

void texture_loadFromPNG(struct texture * tex, const char *filename)
{

	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Failed to open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	// create context
	spng_ctx *context = spng_ctx_new(0);

	// set an input buffer
	spng_set_png_file(context, fp);

	// determine output image size
	size_t out_size;
	spng_decoded_image_size(context, SPNG_FMT_RGB8, &out_size);

	// Image header info
	struct spng_ihdr header;
	spng_get_ihdr(context, &header);

	uint8_t *buffer = malloc(out_size);

	// decode to 8 bit RGB
	spng_decode_image(context, buffer, out_size, SPNG_FMT_RGB8, 0);

	fclose(fp);
	spng_ctx_free(context);

	tex->width = header.width;
	tex->height = header.height;
	tex->data = buffer;

}

void texture_free(unsigned char *buffer)
{
	free(buffer);
}
