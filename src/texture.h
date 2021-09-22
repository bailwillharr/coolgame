struct texture {
	int width;
	int height;
	const unsigned char *data;
};

void texture_loadFromPNG(struct texture * tex, const char *filename);
void texture_free(unsigned char *buffer);
