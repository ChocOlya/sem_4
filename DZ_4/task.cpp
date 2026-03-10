#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list2.h"
#include "ordering.h"


bool where (char * s);
bool read_or(ordering *place, char * s, int i);
bool read_nach(char *s, ordering *place, char ** end);

bool where (char * s)
{
	if (strcmp(s, "where") == 0) return true;
	return false;
}


bool read_or(ordering *place, char * s, int i)
{
	if (strcmp(s, "*") == 0)
	{
		if (i == 0) 
		{
			place[0] = ordering::name;
			place[1] = ordering::phone;
			place[2] = ordering::group;
			return true;
		}
		return false;
	}
	if (strcmp(s, "name") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::name;
		return true;
	}
	if (strcmp(s, "phone") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::phone;
		return true;
	}
	if (strcmp(s, "group") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::group;
		return true;
	}
	return false;

}

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
	ordering place[3];
	
	while (fgets(buf, LEN, stdin) != nullptr)
	{
		for (int i = 0; i < 3; i++) place[i] = ordering::none;
		char *s = buf, *end = nullptr;
		if (read_nach(s, place, &end) == false)
		{
			printf("Command is not right\n");
			continue;
		}
		//printf("READ THE FIRST PART\n");
		command test;
		
		if (!test.parse(end))
		{
			printf("Command is not right\n");
			continue;
		}
		//test.print();
		//printf("READ THE SECOND PART\n");
		
		
		list2_node *curr = list.get_head();
		if (test.like())//we should make razbor of s
		{
			razbor Raz;
			if (!Raz.read(test.get_name())) 
			{
				printf("Error in pattern\n");
				continue;
			}
			while(curr != nullptr)
			{	
				if (test.apply(*curr, &Raz) == true)
				{
					kol++;
					curr->print(place);
				}
				curr = curr->get_next();
			}
			
		}
		
		while(curr != nullptr)
		{	
			if (test.apply(*curr) == true)
			{
				kol++;
				curr->print(place);
			}
			curr = curr->get_next();
		}
	
		
		printf("\n");
		

	}
	t = (clock()-t)/CLOCKS_PER_SEC;
	printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);


	return 0;
}



bool read_nach(char *s, ordering *place, char ** end)
{
	s = strtok_r(s, " ,\n", end);
	if (s == nullptr) return false;
	//printf("%s\n", s);	
	if (read_or(place, s, 0) == false) return false;

	s = strtok_r(*end, " ,\n", end);
	if (s == nullptr) return false;	
	//printf("%s\n", s);
	if (where(s) == false)
	{
		if (read_or(place, s, 1) == false) return false;


		s = strtok_r(*end, " ,\n", end);
		if (s == nullptr) return false;
		//printf("%s\n", s);

		if (where(s) == false)
		{
			//printf("Case word word word where\n");
			if (read_or(place, s, 2) == false) return false;
			s = strtok_r(*end, " ,\n", end);
			if (s == nullptr) return false;
			//printf("%s\n", s);
		}
		//else printf("Case word word where\n");
	}
	//else printf("Case word where\n");
	if (where(s) == false) return false;
	return true;
}

