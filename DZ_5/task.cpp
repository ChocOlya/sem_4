#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list2.h"
#include "command_type.h"

int main(int argc, char const *argv[])
{
	io_status er = io_status::success;
	const char *name_in = 0;
	char buf [LEN];
	double t;
	FILE *fp;

	
	list2 list;
	if (!(argc == 2))
	{
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}
	name_in = argv[1];
	if (!(fp = fopen(name_in, "r")))
	{
		printf("Cannot open %s\n", name_in);
		return -1;
	}

	if ((er  = list.read(fp))!= io_status::success)
	{
		switch(er)
		{
			case io_status::success:
			case io_status::eof:
			printf("End of file %s\n", name_in);
			break;
			case io_status::format:
			printf("Incorrect file format %s\n", name_in);
			break;
			case io_status::memory:
			printf("Memory error\n");
			break;
			
		}
		fclose(fp);
		return -1;
	}
	fclose(fp);
	//list.print();
	int kol = 0;
	t = clock();
	command_type COM = command_type::none;
	command test;
	razbor HELP;
	bool err = true;
	
	while (fgets(buf, LEN, stdin) != nullptr)
	{
		char *s = buf, *end = nullptr;
		err = true;
		while ((s = strtok_r(s, ";", &end)))
		{
			//printf("s = %s\n", s);
			if ((err = test.read_command(s)) != true)
			{
				printf("Comand is not right\n");
				s = end;
				continue;
			}
			test.print();
			COM = test.get_type();
			list2_node *head = nullptr; 
			if (test.like())
			{
				HELP.read(test.get_name());
				//printf("LIKE\n");
			}
			switch(COM)
			{
				case command_type::quit:
				t = (clock()-t)/CLOCKS_PER_SEC;
				printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);
				return 0;
				
				case command_type::insert:
				list.add_value(&test);
				break;
				case command_type::select:
				case command_type:: del:
				//printf("miu1\n");
				head = list.apply(&test, kol, &HELP);
				//printf("miu2\n");
				list.do_st(head, &test);
				//printf("miu3\n");
				case command_type::none:
				break;

			}
			s = end;

		}
		if (err != true) printf("Comand is not right\n");
	}
	printf("There was not the command quit\n");
	return 0;
}





