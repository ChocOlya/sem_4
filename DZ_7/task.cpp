#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include "list2.h"

//#include "command_type.h"
//#include "definition_of_k.h"

int read_parametr(int& K1, int& k2, const char *s);

int main(int argc, char const *argv[])
{
	io_status er = io_status::success;
	const char *name_in = 0;
	char buf [LEN];
	double t;
	int K1 = 1, K2 = 1, err0 = 0;
	avl_tree *garden_name = nullptr;
	avl_tree *garden_phone = nullptr;
	FILE *fp;
	
	if (!(argc == 2))
	{
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}
	name_in = argv[1];
	err0 = read_parametr(K1, K2, argv[0]);
	if (K1 <= 0 || K2 <= 0 || err0 != 0)
	{
		printf("not right parametrs\n");
		return 1;
	}
	if (!(fp = fopen(name_in, "r")))
	{
		printf("Cannot open %s\n", name_in);
		return -1;
	}

	list2 list;

	garden_name = new avl_tree [K1];
	if (!garden_name)
	{
		fclose(fp);
		printf("Memory error\n");
		return -1;
	} 
	for (int i = 0; i < K1; i++) garden_name[i].set_con(by_who::name);

	garden_phone = new avl_tree [K2];
	if (!garden_phone)
	{
		fclose(fp);
		delete[] garden_name;
		printf("Memory error\n");
		return -1;
	}
	for (int i = 0; i < K2; i++) garden_phone[i].set_con(by_who::phone);
	

	if ((er  = list.read(K1, K2, garden_name, garden_phone, fp))!= io_status::success)
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
		delete[] garden_name;
		delete[] garden_phone;
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
			//test.print();
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
					delete[] garden_name;
					delete[] garden_phone;
					printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);
					return 0;
				
				case command_type::insert:
					list.insert(K1, K2, &test, garden_name, garden_phone);
					break;
				case command_type::select:
				case command_type:: del:
					//printf("miu1\n");
					head = list.apply(K1, K2, &test, garden_name, garden_phone, &HELP);
					//printf("miu2\n");
					list.do_st(K1, K2, head, &test, garden_name, garden_phone, kol);
					//printf("miu3\n");
					break;
				case command_type::none:
					break;

			}
			s = end;

		}
		if (err != true) printf("Comand is not right\n");
	}
	printf("There was not the command quit\n");
	t = (clock()-t)/CLOCKS_PER_SEC;
	printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);
	delete[] garden_name;
	delete[] garden_phone;
	return 0;
}





int read_parametr(int& K1, int& K2, const char *s)
{
	const char * config_name = "config.txt";
	unique_ptr<char []> exe_path = make_unique<char []> (strlen (s) + 1);
	strcpy (exe_path.get (), s); // make a copy, "dirname" modifies argument
	char *dir = dirname (exe_path.get ()); // get directory with executable
	//printf ("Executable dir = %s\n", dir);
	size_t path_len = strlen (dir) + 1 + strlen (config_name) + 1;
	unique_ptr<char []> config_path = make_unique<char []> (path_len);
	snprintf (config_path.get (), path_len, "%s/%s", dir, config_name);
	//printf ("Config path = %s\n", config_path.get ());
	FILE *fp = fopen(config_path.get (), "r");
	if (!fp) 
	{
		printf("Cannot open file %s\n", config_path.get ());
		return -1;
	}
	char buf[LEN];
	int flag = 0;
	char *sav = nullptr, *el = nullptr;
	while (fgets(buf, LEN, fp) != nullptr)
	{
		el = strtok_r(buf, " \t\n", &sav);
		if (buf[0] == '#' || el == nullptr) continue;
		else
		{
			//printf("%s\n", el);
			if (flag == 0)
			{
				if (sscanf(el, "%d", &K1) != 1) return -1;
				flag = 1;
			}
			else
			{
				fclose(fp);
				if (sscanf(el, "%d", &K2) != 1) return -1;
				return 0;
			}			
		}
	}
	fclose(fp);
	return -1;
}


