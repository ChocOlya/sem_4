# include <stdio.h>
# include <string.h>
# include <memory>
# include <stdlib.h>
# include <time.h>
# include "solve.h"




int main(int argc, const char ** argv)
{
	io_status err;
	int res = 0, task = 5;
	const char * name_a = 0;
	const char * name_b = 0;
	const char * name_out = 0;
	const char * str_t = 0;
	double t;
	
	if (!(argc == 5))
	{
		printf("Usage: %s file_a file_b file_out t\n", argv[0]);
		return -1;
	}

	name_a = argv[1];
	name_b = argv[2];
	name_out = argv[3];
	str_t = argv[4];

	t = clock();
	err = solve5(name_a, name_b, name_out, str_t, res);
	t = (clock() - t) / CLOCKS_PER_SEC;
	switch (err)
	{
		case io_status::success:
			printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, t);
			return 0;
		case io_status::fopen_1:
			printf ("Cannot open file %s\n", name_a);
			break;
		case io_status::fopen_2:
			printf ("Cannot open file %s\n", name_b);
			break;
		case io_status::fopen_3:
			printf ("Cannot open file %s\n", name_out);
			break;
		case io_status::mem:
			printf ("Memory error\n");
			break;
	}

	return -1;
}


