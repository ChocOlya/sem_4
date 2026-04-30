# include <stdio.h>
# include <string.h>
# include <memory>
# include <stdlib.h>
# include <vector>
# include <list>
# include <utility>
# include <string>
# include <algorithm>
# include "solve.h"
# define LEN 1234



io_status solve1(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int i = 1;
	std::vector <std::pair<std::string, int>> vec;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr; i++)
	{
		vec.push_back(std::make_pair(BUF, i));
	}
	//printf("Vector was constractid\n");
	std::sort(vec.begin(), vec.end());
	//printf("Vector was sorted\n");
	if (i == 0)
	{
		fclose(fp_in);
		fclose(fp_out);
		res = 0;
		return io_status::success;
	}
	int kol = 1;
	auto el = vec.begin(), end = vec.end();
	while (el != end - kol)//not last
	{
		if (el->first == (el + 1)->first)//need to delite one of them
		{
			//printf("Need to delite one of them they are %s and %s\n", el->first.c_str(), el->first.c_str());
			if (el->second < (el + 1)->second) 
			{
				if (el + 1 == end - kol)
				{
					vec.erase(el + 1);
					break;
				}
				vec.erase(el + 1);
			}
			else vec.erase(el);
			kol++;
		}
		else el++;
	}
	//printf("Deleted sth\n");
	std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
	//printf("Sorted again\n");
	res = vec.size();
	for (i = 0; i < res; i++)
		fprintf(fp_out, "%s", vec[i].first.c_str());
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}

/*
auto el = vec.begin(), end = vec.end();
while (el != end - 1)
{
	if (el->first == (el + 1)->first)
	{
		if (el->second < (el + 1)->second) vec.erase(el + 1);
		else vec.erase(el);
	}
	if (el == end - 1) break;
	else el++;
}
*/



io_status solve2(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int i = 1;
	std::list <std::pair<std::string, int>> lst;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr; i++)
	{
		lst.push_back(std::make_pair(BUF, i));
	}
	lst.sort();
	if (i == 0)
	{
		fclose(fp_in);
		fclose(fp_out);
		res = 0;
		return io_status::success;
	}
	auto el_n = lst.begin(), end = lst.end(), el = el_n++;
	for (;el_n != end; el = el_n++)//next exist
	{
		//printf("Curr is %d %s", el->second, el->first.c_str());
		if (el->first == el_n->first)//need to delite one of them
		{
			//printf("Need to delite one of them they are %s and %s\n", el->first.c_str(), el->first.c_str());
			if (el->second < el_n->second) 
			{
				lst.erase(el_n); // need to del second
				el_n = el;//el_n need to be current

			}
			else lst.erase(el);//el_n is curr
		}
		
	}
	
	lst.sort([](const auto& a, const auto& b) { return a.second < b.second; });
	end = lst.end();
	int kol = 0;
	for (el = lst.begin(); el != end; el++)
	{
		fprintf(fp_out, "%s", el->first.c_str());
		kol++;
	}
	res = kol;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}







io_status solve3(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int i = 1;
	std::vector <std::pair<std::string, std::pair<int, int>>> vec;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr; i++)
	{
		vec.push_back(std::make_pair(BUF, std::make_pair(i, 1)));
	}
	std::sort(vec.begin(), vec.end());
	if (i == 0)
	{
		fclose(fp_in);
		fclose(fp_out);
		res = 0;
		return io_status::success;
	}
	int kol = 1, j = 0;
	int unic = 1;
	int size = vec.size();
	for (i = 0; i < size - 1; i++)//not last
	{
		if (vec[i].first == vec[i + 1].first)
		{
			kol++;
		}
		else
		{
			unic++;
			for (j = 0; j < kol; j++)
				vec[i - j].second.second = kol;
			kol = 1;
		}
	}
	for (j = 0; j < kol; j++)
		vec[i - j].second.second = kol;
	std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) { return a.second.first < b.second.first; });
	res = unic;
	for (i = 0; i < size; i++)
		fprintf(fp_out, "%d %d %s", vec[i].second.first, vec[i].second.second, vec[i].first.c_str());
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}




io_status solve4(const char *name_in, const char *name_out, int &res)
{
	FILE *fp_in, *fp_out;
	char BUF[LEN];int i = 1;
	std::list <std::pair<std::string, std::pair<int, int>>> lst;
	
	if (!(fp_in = fopen(name_in, "r"))) return io_status::fopen_1;
	if (!(fp_out = fopen(name_out, "w")))
	{
		fclose(fp_in);
		return io_status::fopen_2;
	}
	for(; fgets(BUF, LEN, fp_in) != nullptr; i++)
	{
		lst.push_back(std::make_pair(BUF, std::make_pair(i, 1)));
	}
	lst.sort();
	if (i == 0)
	{
		fclose(fp_in);
		fclose(fp_out);
		res = 0;
		return io_status::success;
	}
	int kol = 1, unic = 1, j = 0;
	auto el_n = lst.begin(), end = lst.end(), el = el_n++;
	for (;el_n != end; el = el_n++)//next exist
	{
		if (el->first == el_n->first)//need to delite one of them
		{
			kol++;
		}
		else
		{
			unic++;
			el->second.second = kol;
			for (j = 0; j < kol - 1; j++)
			{
				el--;
				el->second.second = kol;
			}
			kol = 1;
		}
	}
	el->second.second = kol;
	for (j = 0; j < kol - 1; j++)
	{
		el--;
		el->second.second = kol;
	}
	lst.sort([](const auto& a, const auto& b) { return a.second.first < b.second.first; });
	end = lst.end();
	for (el = lst.begin(); el != end; el++)
	{
		fprintf(fp_out, "%d %d %s", el->second.first, el->second.second, el->first.c_str());
	}
	res = unic;
	fclose(fp_in);
	fclose(fp_out);
	return io_status::success;
}




io_status solve5(const char *name_a, const char *name_b, const char *name_out, const char * tau, int &res)
{
	FILE *fp_a, *fp_b, *fp_out;
	char BUF[LEN];int i = 1;
	char buf0[LEN];
	char *s, *end_s;
	sprintf(buf0, "%s\n", tau);
	char *t = buf0;
	printf("%s = t\n", t);
	std::vector <std::string> words;
	std::vector <std::pair<std::string, int>> vec_b;
	
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
			printf("%s\n", s);
			words.push_back(s);
		}
	}
	printf("END\n");
	std::sort(words.begin(), words.end());

	auto end = std::unique(words.begin(), words.end());
	words.erase(end, words.end());
	//auto beg = words.begin();
	end = words.end();

	int size = words.size();
	for (i = 0; i <  size; i++) printf("%s\n", words[i].c_str());

	int kol = 0, kol_all = 0;
		
	for  (i = 0; fgets(BUF, LEN, fp_b) != nullptr; i++)
	{
		kol = 0;
		printf("string = %st = %st_len = %d\n", BUF, t, (int)strlen(t));
		vec_b.push_back(std::make_pair(BUF, 0));
		if ((s = strtok_r(BUF, t, &end_s)) != nullptr)
		{
			printf("%s miu\n", s);
			if (std::binary_search(words.begin(), words.end(), s) == true)
				kol++;
		}
		while ((s = strtok_r(nullptr, t, &end_s)) != nullptr)
		{
			printf("%s miu\n", s);
			if (std::binary_search(words.begin(), words.end(), s) == true)
				kol++;
		}
		vec_b[i].second = kol;
		kol_all += kol;

	}
	res = kol_all;
	size = vec_b.size();
	for (i = 0; i < size; i++)
		fprintf(fp_out, "%d %s", vec_b[i].second, vec_b[i].first.c_str());
	fclose(fp_a);
	fclose(fp_b);
	fclose(fp_out);
	return io_status::success;
}




/*
io_status solve5(const char *name_a, const char *name_b, const char *name_out, int &res);
io_status solve6(const char *name_a, const char *name_b, const char *name_out, int &res);*/