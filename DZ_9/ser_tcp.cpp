//TIPO SERVER
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libgen.h>
#include <memory>
#include <time.h>
#include "list2.h"

int pair_of_gardens::K1;
int pair_of_gardens::K2;
// Определимся с номером порта и другими константами.
//#define PORT 5555
#define BUFLEN 1234
// Две вспомогательные функции для чтения/записи (см. ниже)
int readFromClient (int fd, char *buf);
int writeToClient (int fd, int kol, list2_node *head, ordering *order_by, command_type COM);
int read_parametr(int& K1, int& k2, const char *s);
int send_a_namber(int fd, int num);
int send_a_str(int fd, char *s, int len);
int read_a_number(int fd, int& kol);
int read_a_str(int fd, char* buf, int len);


int main (int argc, char const *argv[])
{
	io_status er = io_status::success;
	const char *name_in = 0;
	int K1 = 1, K2 = 1, err0 = 0;
	pair_of_gardens ALL;
	pair_of_gardens GROUP[1000];
	int PORT;
	FILE *fp;

	command_type COM = command_type::none;
	command test;
	razbor HELP;


	if (!(argc == 3 && sscanf(argv[2], "%d", &PORT) == 1))
	{
		printf("Usage: %s filename port\n", argv[0]);
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

	ALL.set_K1(K1);
	ALL.set_K2(K2);

	list2 list;

	if ((er  = list.read(ALL, GROUP, fp))!= io_status::success)
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
	int kol = 0;
	//создала список и структуры



	int i, err, flag = 0, opt = 1;
	int sock, new_sock;
	fd_set active_set, read_set;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	char buf[BUFLEN];
	socklen_t size;
	// Создаем TCP сокет для приема запросов на соединение
	sock = socket (PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror ("Server: cannot create socket");
		return -1;
	}
	setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));
	// Заполняем адресную структуру и
	// связываем сокет с любым адресом
	addr.sin_family = AF_INET;
	addr.sin_port = htons (PORT);
	addr.sin_addr.s_addr = htonl (INADDR_ANY);
	err = bind (sock, (struct sockaddr *) &addr, sizeof (addr));
	if (err < 0)
	{
		perror ("Server: cannot bind socket");
		return -1;
	}
	// Создаем очередь на 3 входящих запроса соединения
	err = listen (sock, 3);
	if (err < 0)
	{
		perror ("Server: listen queue failure");
		return -1;
	}
	// Подготавливаем множества дескрипторов каналов ввода-вывода.
	// Для простоты не вычисляем максимальное значение дескриптора,
	// а далее будем проверять все дескрипторы вплоть до максимально
	// возможного значения FD_SETSIZE.
	FD_ZERO (&active_set);
	FD_SET (sock, &active_set);
	// Основной бесконечный цикл проверки состояния сокетов
	while (1)
	{
		//printf("hello\n");
		// Проверим, не появились ли данные в каком-либо сокете.
		// В нашем варианте ждем до фактического появления данных.
		read_set = active_set;
		if (select (FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
		{
			perror ("Server: select failure");
			return -1;
		}
		// Данные появились. Проверим в каком сокете.
		for (i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET (i, &read_set))
			{
				if (i == sock)
				{
					// пришел запрос на новое соединение
					size = sizeof (client);
					new_sock = accept (sock, (struct sockaddr *) &client, &size);
					if (new_sock < 0)
					{
						perror ("accept");
						return -1;
					}
					fprintf (stdout, "Server: connect from host %s, port %d.\n", inet_ntoa (client.sin_addr),(unsigned int) ntohs (client.sin_port));
					FD_SET (new_sock, &active_set);
				}
				else
				{
					// пришли данные в уже существующем соединени
					if ((readFromClient (i, buf)) >= 0)//READ
					{
						// а если это команда закончить работу?
						if (!strcmp (buf, "stop;"))
						{
							close (sock);
							FD_SET (new_sock, &active_set);
							return 0;
						}
						char *s = buf, *end = nullptr;
						bool err1 = true; flag = 0;
						if ((s = strtok_r(s, ";", &end)) != nullptr)
						{
							COM = command_type::none;
							if ((err1 = test.read_command(s)) != true)
							{
								printf("Comand is not right\n");
								writeToClient (i, 0, nullptr, nullptr, COM);
								break;
								
							}
							//test.print();
							COM = test.get_type();
							list2_node *head = nullptr; 
							if (test.like())
							{
								HELP.read(test.get_name());
							}
							kol = 0;
							switch(COM)
							{
								case command_type::quit:
									flag = 1;
									break;
								case command_type::insert:
									list.insert(test, ALL, GROUP);
									break;
								case command_type::select:
									head = list.apply(&test, ALL, GROUP, &HELP);
									//head->print_select();
									head = list.do_st(head, &test, ALL, GROUP, kol);
									break;
								case command_type:: del:
									head = list.apply(&test, ALL, GROUP, &HELP);
									//head->print_select();
									list.do_st(head, &test, ALL, GROUP, kol);
									break;
								case command_type::none:
									break;

							}
							writeToClient (i, kol, head, test.get_order(), COM);
							if (flag == 1) 
							{
								close (i);
								FD_CLR (i, &active_set);
							}
							
							break;

						}
					}
					// ошибка или конец данных
					printf("WF!\n");
					close (i);
					FD_CLR (i, &active_set);

				}
			}
		}
	}
}



int readFromClient (int fd, char *buf)
{
	int len = 0;
	if (read_a_number(fd, len) < 0) return -1;
	if (read_a_str(fd, buf, len) < 0) return -1;
	return 0;
}



int writeToClient (int fd, int kol, list2_node *head, ordering *order_by, command_type COM)
{
	int len = 0;
	char where[BUFLEN];
	list2_node *prev = nullptr;
	if (COM == command_type::select)
	{
		// Пересылаем длину сообщения
		if (send_a_namber(fd, kol) < 0)
			return -1;
		// Пересылаем len байт
		for (int j = 0; j < kol; j++)
		{
			where[0] = '\0';
			head->prepare_str(where, order_by);
			len = strlen(where);
			if (send_a_namber(fd, len) < 0) return -1;
			if (send_a_str(fd, where, len) < 0) return -1; 
			prev = head;
			head = head->get_next_select();
			prev->set_next_select(nullptr);
		}
		return 0;
	}
	if (COM == command_type::quit)
	{
		//printf("QUIT\n");
		if (send_a_namber(fd, -7) < 0) return -1;
		return 0;
	}
	if (COM == command_type::none)
	{
		//printf("want to send\n");
		if (send_a_namber(fd, -8) < 0) return -1;
		sprintf (where, "Command is not right");
		len = strlen(where);
		if (send_a_namber(fd, len) < 0) return -1;
		if (send_a_str(fd, where, len) < 0) return -1; 
		return 0;
	}
	if (send_a_namber(fd, 0) < 0) return -1;
	return 0;
}


int send_a_namber(int fd, int num)
{
	int len_of_len = sizeof(int);
	int nbytes = 0, i = 0;
	for (i = 0; len_of_len > 0; i += nbytes, len_of_len -= nbytes)
	{
		nbytes = write (fd, (char *)&num + i, len_of_len);
		if (nbytes < 0)
		{
			perror ("Server: write length");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Server: write truncated lenth");
			return -1;
		}
	}
	return 0;

}


int send_a_str(int fd, char *s, int len)
{
	int i = 0, nbytes = 0; len++;
	for (i = 0; len > 0; i += nbytes, len -= nbytes)
	{
		nbytes = write (fd, s + i, len);
		if (nbytes < 0)
		{
			perror ("Server: write");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Server: write truncated");
			return -1;
		}
	}
	return 0;
}


int read_a_number(int fd, int& len)
{
	int nbytes = 0, i = 0;
	int len_of_len = sizeof (int);
	for (i = 0; len_of_len > 0; i += nbytes, len_of_len -= nbytes)
	{
		nbytes = read (fd, (char *)&len + i, len_of_len);
		if (nbytes < 0)
		{
			perror ("Server: read lenth");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Server: read truncated lenth");
			return -1;
		}
	}
	return 0;
}



int read_a_str(int fd, char* buf, int len)
{
	int i = 0, nbytes = 0; len++;
	for (i = 0; len > 0; i += nbytes, len -= nbytes)
	{
		nbytes = read (fd, buf + i, len);
		if (nbytes < 0)
		{
			perror ("Server: read");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Server: read truncated");
			return -1;
		}
	}
	// Длина находится в i
	if (i == 0)
	{
		// нет данных
		fprintf (stderr, "Server: no message\n");
		return -1;
	}
	else
	{
		// есть данные
		fprintf (stdout, "Server got %d bytes of message: %s\n", i, buf);
		return 0;
	}
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


/*


	int kol = 0;
	t = clock();
	command_type COM = command_type::none;
	command test;
	razbor HELP;
	bool err1 = true;
	
	while (fgets(buf, LEN, stdin) != nullptr)
	{
		char *s = buf, *end = nullptr;
		err1 = true;
		while ((s = strtok_r(s, ";", &end)))
		{
			//printf("s = %s\n", s);
			if ((err1 = test.read_command(s)) != true)
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
					printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);
					return 0;
				
				case command_type::insert:
					list.insert(test, ALL, GROUP);
					break;
				case command_type::select:
				case command_type:: del:
					head = list.apply(&test, ALL, GROUP, &HELP);
					list.do_st(head, &test, ALL, GROUP, kol);
					break;
				case command_type::none:
					break;

			}
			s = end;

		}
		if (err1 != true) printf("Comand is not right\n");
	}
	printf("There was not the command quit\n");
	t = (clock()-t)/CLOCKS_PER_SEC;
	printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], kol, t);

	return 0;
}



*/