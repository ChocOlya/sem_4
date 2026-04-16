#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
// Определимся с портом, адресом сервера и другими константами.
// В данном случае берем произвольный порт и адрес обратной связи
// (тестируем на одной машине).
#define SERVER_PORT 5555
#define SERVER_NAME "127.0.0.1"
#define BUFLEN 1234
// Две вспомогательные функции для чтения/записи (см. ниже)
void writeToServer (int fd, char *buf);
void readFromServer (int fd, int& flag);

int send_a_namber(int fd, int num);
int send_a_str(int fd, char *s, int len);
int read_a_number(int fd, int& kol);
int read_a_str(int fd, char* buf, int len);


int main (void)
{
	int err;
	int sock;
	struct sockaddr_in server_addr;
	struct hostent *hostinfo;
	char buf[BUFLEN];
	// Получаем информацию о сервере по его DNS имени
	// или точечной нотации IP адреса.
	hostinfo = gethostbyname (SERVER_NAME);
	if (hostinfo == NULL)
	{
		fprintf (stderr, "Unknown host %s.\n", SERVER_NAME);
		return -1;
	}
	// Заполняем адресную структуру для последующего
	// использования при установлении соединения
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (SERVER_PORT);
	server_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	// Создаем TCP сокет.
	sock = socket (PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror ("Client: socket was not created");
		return -1;
	}
	// Устанавливаем соединение с сервером
	err = connect (sock, (struct sockaddr *) &server_addr, sizeof (server_addr));
	if (err < 0)
	{
		perror ("Client: connect failure");
		return -1;
	}
	fprintf (stdout, "Connection is ready\n");
	// Обмениваемся данными
	int flag = 0;
	while (fgets(buf, BUFLEN, stdin) != nullptr)
	{
		writeToServer (sock, buf);
		if (strcmp(buf, "stop;") == 0)
		{
			close (sock);
			return 0;
		}
		readFromServer (sock, flag);
		if (flag == 1)
		{
			close (sock);
			return 0;
		}
	}
	
	// Закрываем socket
	close (sock);
	return 0;
}


void writeToServer (int fd, char *buf)
{
	int len = 0;
	fprintf (stdout, "Send to server > ");
	// Удаляем завершающий символ ’\n’ (если есть)
	for (len = 0; buf[len]; len++)
		if (buf[len] == '\n')
		{
			buf[len] = 0;
			break;
		}
	// Длина сообщения
	len++;
	// Пересылаем длину сообщения
	if (send_a_namber(fd, len) < 0) return;
	if (send_a_str(fd, buf, len) < 0) return;
	return;
}



void readFromServer (int fd, int& flag)
{
	int kol = 0, len = 0;
	char buf[BUFLEN];
	// Получаем длину сообщения
	fprintf (stdout, "Server’s replay:\n");
	if (read_a_number(fd, kol) < 0) return;
	if (kol == -7)
	{
		printf("(1000)-7\n");
		flag = 1;
		if (read_a_number(fd, len) < 0) return;
		// Получаем len байт
		if (read_a_str(fd, buf, len) < 0) return;
		if (len == 0)
		{
			// нет данных
			fprintf (stderr, "Client: no message\n");
		}
		else
		{
			// ответ успешно прочитан
			fprintf (stdout, "%s\n", buf);
		}
		return;
	}
	if (kol == 0)
	{
		fprintf (stderr, "Client: no message\n");
		return;
	}
	for (int j = 0; j < kol; j++)
	{
		if (read_a_number(fd, len) < 0) return;
		// Получаем len байт
		if (read_a_str(fd, buf, len) < 0) return;
		// Длина находится в i
		if (len == 0)
		{
			// нет данных
			fprintf (stderr, "Client: no message\n");
		}
		else
		{
			// ответ успешно прочитан
			fprintf (stdout, "%s\n", buf);
		}
	}
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
			perror ("Client: write length");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Client: write truncated lenth");
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
			perror ("Client: write");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Client: write truncated");
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
			perror ("Client: read lenth");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Client: read truncated lenth");
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
			perror ("Client: read");
			return -1;
		}
		else if (nbytes == 0)
		{
			perror ("Client: read truncated");
			return -1;
		}
	}
	// Длина находится в i
	if (i == 0)
	{
		// нет данных
		fprintf (stderr, "Client: no message\n");
		return -1;
	}
	else
	{
		// есть данные
		fprintf (stdout, "Client got %d bytes of message: %s\n", i, buf);
		return 0;
	}
	return 0;
}

