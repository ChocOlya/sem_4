enum class io_status
{
	success,
	fopen_1,
	fopen_2,
	fopen_3,
	mem,
};


io_status solve1(const char *name_in, const char *name_out, int &res);
io_status solve2(const char *name_in, const char *name_out, int &res);
io_status solve3(const char *name_in, const char *name_out, int &res);
io_status solve4(const char *name_in, const char *name_out, int &res);
io_status solve5(const char *name_a, const char *name_b, const char *name_out, const char * t, int &res);
io_status solve6(const char *name_a, const char *name_b, const char *name_out, const char * t, int &res);