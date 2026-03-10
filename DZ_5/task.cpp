#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list2.h"
#include "command_type.h"

int main(int argc, char const *argv[])
{
	io_status err = io_status::success;
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

	if ((err  = list.read(fp))!= io_status::success)
	{
		switch(err)
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
	int kol = 0;
	t = clock();
	command_type COM = command_type::none;
	command test;
	bool err = true;
	
	while (fgets(buf, LEN, stdin) != nullptr)
	{
		char *s = buf, *end = nullptr;
		err = true;
		while ((s = strtok_r(s, ";", end)))
		{
			if ((err = test->read_command) != true)
				printf("Comand is not right\n");
			COM = test->get_type();
			list_node *head = nullptr; 
			switch(COM)
			{
				case command_type::quit:
				t = (clock()-t)/CLOCKS_PER_SEC;
				printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);
				return 0;
				
				case command_type::insert:
				list->add_value(test);
				break;
				case command_type::select:
				case command_type:: del:
				head = list->apply(test, kol);
				list->do_st(head, test);
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





