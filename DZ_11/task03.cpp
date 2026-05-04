# include <stdio.h>
# include <string.h>
# include <memory>
# include <stdlib.h>
# include <time.h>
# include "solve.h"




int main(int argc, const char ** argv)
{
	io_status err;
	int res = 0, task = 3;
	const char * name_in = 0;
	const char * name_out = 0;
	double t;
	
	if (!(argc == 3))
	{
		printf("Usage: %s file_in file_out\n", argv[0]);
		return -1;
	}

	name_in = argv[1];
	name_out = argv[2];
	t = clock();
	err = solve3(name_in, name_out, res);
	t = (clock() - t) / CLOCKS_PER_SEC;
	switch (err)
	{
		case io_status::success:
			printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, t);
			return 0;
		case io_status::fopen_1:
			printf ("Cannot open file %s\n", name_in);
			break;
		case io_status::fopen_2:
			printf ("Cannot open file %s\n", name_out);
			break;
		case io_status::fopen_3:
			printf ("Unknown error\n");
			break;
		case io_status::mem:
			printf ("Memory error\n");
			break;
	}

	return -1;
}


