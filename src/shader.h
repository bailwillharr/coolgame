enum shader {
	SHADER_BASIC
};

unsigned int shader_load(enum shader s);
void shader_delete(unsigned int id);
