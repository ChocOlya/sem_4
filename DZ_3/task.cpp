#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list2.h"



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
	while (fgets(buf, LEN, stdin) != nullptr)
	{
		
		command test;
		char *s = buf;
		if (!test.parse(s))
		{
			printf("Command is not right\n");
			continue;
		}
		//test.print();
		
		
		list2_node *curr = list.get_head();
		if (test.like())//we should make razbor of s
		{
			razbor Raz;
			if (!Raz.read(test.get_name())) 
			{
				printf("Error in pattern\n");
				continue;
			}
			int flag = Raz.get_flag();
			if (flag == 2)
			{
				char *s1 = nullptr, *s2 = nullptr;
				int und = Raz.understand(&s1, &s2);
				while(curr != nullptr)
				{
					if (proverka(und, s1, s2, curr->get_name()))
					{
						kol++;
						curr->print();
					}
					curr = curr->get_next();
				}

			}
			else 
			{
				while(curr != nullptr)
				{
					if (Raz.check(curr->get_name()))
					{
						kol++;
						curr->print();
					}
					curr = curr->get_next();
				}
			}
			
		}
		else
		{
			while(curr != nullptr)
			{	
				if (test.apply(*curr))
				{
					kol++;
					curr->print();
				}
				curr = curr->get_next();
			}
		}
		
		printf("\n");
		

	}
	t = (clock()-t)/CLOCKS_PER_SEC;
	printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);


	return 0;
}

