# include <stdio.h>
# include <string.h>
# include <memory>
# include <stdlib.h>
# include <map>
# include <set>
# include <utility>
# include <string>
# include <algorithm>
# include "solve.h"
# define LEN 1234



io_status solve1(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int kol = 0;
	std::set <std::string> set;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr;)
	{
		if (set.find(BUF) == set.end())
		{
			kol++;
			set.insert(BUF);
			fprintf(fp_out, "%s", BUF);
		}
	}
	res = kol;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}



io_status solve2(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int kol = 0;
	std::map <std::string, int> map;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr;)
	{
		if (map.find(BUF) == map.end())
		{
			kol++;
			map[BUF] = 0;
			fprintf(fp_out, "%s", BUF);
		}
	}
	res = kol;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}




io_status solve3(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int kol = 0, unic = 0, eq = 0;
	std::multiset <std::string> mset;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr;)
	{
		kol++;
		mset.insert(BUF);
		eq = mset.count(BUF);
		fprintf(fp_out, "%d %d %s", kol, eq, BUF);
		if (eq == 1) unic++;
	}
	res = unic;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}




io_status solve4(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int kol = 0, unic = 0, eq = 0;
	std::map <std::string, int> map;
	auto el = map.end();
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr;)
	{
		kol++;
		if ((el = map.find(BUF)) == map.end())
		{
			eq = 1;
			unic++;
			map[BUF] = 1;
		}
		else
			eq = ++map[BUF];
		fprintf(fp_out, "%d %d %s", kol, eq, BUF);
	}
	res = unic;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}




io_status solve5(const char *name_a, const char *name_b, const char *name_out, const char * tau, int &res)
{
	FILE *fp_a, *fp_b, *fp_out;
	char BUF[LEN], Buf_cpy[LEN];int i = 1;
	char buf0[LEN];
	char *s, *end_s;
	sprintf(buf0, "%s\n", tau);
	char *t = buf0;
	std::set <std::string> words;
	
	if (!(fp_a = fopen(name_a, "r"))) return io_status::fopen_1;
	if (!(fp_b = fopen(name_b, "r")))
	{
		fclose(fp_a);
		return io_status::fopen_2;
	}
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_a);
		fclose(fp_b);
		return io_status::fopen_3;
	}
	while (fgets(BUF, LEN, fp_a) != nullptr)
	{
		s = BUF;
		for (; (s = strtok_r(s, t, &end_s)) != nullptr; s = end_s)
		{
			words.insert(s);
		}
	}
	auto end = words.end();
	int kol = 0, kol_all = 0;	
	for  (i = 0; fgets(BUF, LEN, fp_b) != nullptr; i++)
	{
		kol = 0;
		strcpy(Buf_cpy, BUF);
		s = BUF;
		while ((s = strtok_r(s, t, &end_s)) != nullptr)
		{
			//printf("%s miu\n", s);
			if (words.find(s) != end)
				kol++;
			s = nullptr;
		}
		kol_all += kol;
		fprintf(fp_out, "%d %s", kol, Buf_cpy);

	}
	res = kol_all;
	
	fclose(fp_a);
	fclose(fp_b);
	fclose(fp_out);
	return io_status::success;
}



io_status solve6(const char *name_a, const char *name_b, const char *name_out, const char * tau, int &res)
{
	FILE *fp_a, *fp_b, *fp_out;
	char BUF[LEN], Buf_cpy[LEN];int i = 1;
	char buf0[LEN];
	char *s, *end_s;
	sprintf(buf0, "%s\n", tau);
	char *t = buf0;
	std::map <std::string, int> words;
	
	if (!(fp_a = fopen(name_a, "r"))) return io_status::fopen_1;
	if (!(fp_b = fopen(name_b, "r")))
	{
		fclose(fp_a);
		return io_status::fopen_2;
	}
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_a);
		fclose(fp_b);
		return io_status::fopen_3;
	}
	while (fgets(BUF, LEN, fp_a) != nullptr)
	{
		s = BUF;
		for (; (s = strtok_r(s, t, &end_s)) != nullptr; s = end_s)
		{
			words[s] = 0;
		}
	}
	auto end = words.end();
	int kol = 0, kol_all = 0;	
	for  (i = 0; fgets(BUF, LEN, fp_b) != nullptr; i++)
	{
		kol = 0;
		strcpy(Buf_cpy, BUF);
		s = BUF;
		while ((s = strtok_r(s, t, &end_s)) != nullptr)
		{
			if (words.find(s) != end)
				kol++;
			s = nullptr;
		}
		kol_all += kol;
		fprintf(fp_out, "%d %s", kol, Buf_cpy);

	}
	res = kol_all;
	
	fclose(fp_a);
	fclose(fp_b);
	fclose(fp_out);
	return io_status::success;
}

