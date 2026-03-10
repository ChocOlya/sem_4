# ifndef razbor_H
# define razbor_H
#include <stdio.h>
#include <string.h>
#include <memory>
#define LEN 1234




bool check(const char *str, char * m, char * s, char * l, char *r)
{
	int i = 0;
	for (; str[i]; i++)
	{
		if (!m[i]) return false;
		//printf("m[%d] = %c\n", i, m[i]);
		if (m[i] == '0')
		{
			if (str[i] != s[i]) return false;
		}
		else if (m[i] == '3')
		{
			if (str[i] < l[i] || str[i] > r[i]) return false;
		}
		else if (m[i] == '4')
		{
			if (str[i] >= l[i] && str[i] <= r[i]) return false;
		}
		else if(m[i] == '2')
		{
			//printf("biaka\n");
			if (m[i + 1] == '\0') return true;//если процентик стоял в конце - все ок
			//printf("biaka2\n");
			for(int j = i; m[j]; j++)
			{
				if (check(str + j, m + j + 1, s + j + 1, l + j + 1, r + j + 1)) return true;
			} 
			return false;
		}
	}
	while (m[i] && m[i] == '2') i++;
	if (!m[i]) return true;
	return false;
}






class razbor
{
	private:
		std::unique_ptr<char[]> mask;
		std::unique_ptr<char[]> sence;
		std::unique_ptr<char[]> min;
		std::unique_ptr<char[]> max;
	public:
		razbor () = default;
		razbor (const razbor& x) = delete;
		// По умолчанию: переместить все поля класса - подходит
		razbor (razbor&& x) = default;
		~razbor () = default;
		razbor& operator= (const razbor& x) = delete;
		// По умолчанию: присвоить с перемещением все поля класса - подходит
		razbor& operator= (razbor&& x) = default;
		void print (FILE * fp = stdout) const
		{
			fprintf (fp, "%s\n %s\n", mask.get (), sence.get());
		}
		char * get_mask()
		{ return mask.get(); }
		char * get_sence()
		{ return sence.get(); }
		char * get_min()
		{ return min.get(); }
		char * get_max()
		{ return max.get(); }
		bool read (const char *s)
		{
			/*
			1 - _
			2 - %
			3 - [
			4 - [^
			*/
			char buf_m[LEN];
			char buf[LEN];
			char buf_min[LEN];
			char buf_max[LEN];
			int k = 0;
			for (int i = 0; s[i]; i++)
			{
				if(s[i] == '[')
				{
					buf[k] = '@';
					i++;
					if (!s[i] || s[i] == '[' || s[i] == ']') return false;//[a
					if (s[i] == '\\')
					{
						i++;
						if (!s[i]) return false;
					} 
					if (s[i] == '^' )
					{
						i++;
						if (!s[i] || s[i] == '[' || s[i] == ']') return false;
						if (s[i] == '\\')
						{
							i++;
							if (!s[i]) return false;
						}
						if (s[i] != '-')
						{
							//printf("case [^a\n");
							buf_m[k] = '4';
							buf_min[k] = s[i];
							i++;
							if (!s[i] || s[i] == '[' || s[i] == ']') return false;
							if (s[i] == '\\')
							{
								i++;
								if (!s[i]) return false;
							}
							if (s[i] != '-') return false;
							i++;
							if (!s[i] || s[i] == '[' || s[i] == ']') return false;
							if (s[i] == '\\')
							{
								i++;
								if (!s[i]) return false;
							}
							buf_max[k] = s[i];
							k++;
							i++;
							if (!s[i] || s[i] != ']') return false;							
						}
						else //[^-
						{
							i++;
							if (!s[i] || s[i] == ']' || s[i] == '[') return false;
							if (s[i] == '\\')
							{
								i++;
								if (!s[i]) return false;
							}
							if (s[i] != '-') // flag = 3 [^-s
							{
								//printf("case [^-a\n")
								buf_m[k] = '3';
								buf_min[k] = '^';
								buf_max[k] = s[i];
								k++;
								i++;
								if (!s[i] || s[i] != ']') return false;
							}
							else //[^--
							{
								i++;
								if (!s[i]) return false;
								if (s[i] == ']')
								{
									//printf("case [^--]\n");
									buf_m[k] = '3';
									buf_min[k] = '^';
									buf_max[k] = '-';
									k++;
								}
								else
								{
									//printf("case [^--a]\n");
									if (s[i] == '\\')
									{
										i++;
										if (!s[i]) return false;
									}
									else if (s[i] == '[' || s[i] == ']') return false;
									buf_min[k] = '-';
									buf_max[k] = s[i];
									buf_m[k] = '4';
									k++;
									i++;
									if (!s[i] || s[i] != ']') return false;
								}
							}
						}
					}
					else 
					{
						buf_m[k] = '3';;
						buf_min[k] = s[i];
						i++;
						if (!s[i] || s[i] == '[' || s[i] == ']') return false;
						if (s[i] == '\\')
						{
							i++;
							if (!s[i]) return false;
						} 
						if (s[i] != '-') return false;
						i++;
						if (!s[i] || s[i] == '[' || s[i] == ']') return false;
						if (s[i] == '\\')
						{
							i++;
							if (!s[i]) return false;
						}
						buf_max[k] = s[i];
						i++;
						if (!s[i] || s[i] != ']') return false;
						k++;
					}

				}
				else if (s[i] == '_')
				{
					buf_m[k] = '1';
					buf[k] = '@';
					buf_min[k] = '@';
					buf_max[k] = '@';
					k++;
				}
				else if (s[i] == '%')
				{
					buf_m[k] = '2';
					buf[k] = '@';
					buf_min[k] = '@';
					buf_max[k] = '@';
					k++;
				}
				else if (s[i] == '\\')
				{
					i++;
					if (!s[i]) return false;
					buf_m[k] = '0';
					buf[k] = s[i];
					buf_min[k] = '@';
					buf_max[k] = '@';
					k++;
				}
				else
				{
					buf_m[k] = '0';
					buf[k] = s[i];
					buf_min[k] = '@';
					buf_max[k] = '@';
					k++;

				}
			}
			buf_m[k] = '\0';
			buf[k] = '\0';
			buf_min[k] = '\0';
			buf_max[k] = '\0';
			printf("MASK %s\nSence %s\nMin %s\nMax %s\n", buf_m, buf, buf_min, buf_max);
			erase ();
			return init (buf_m, buf, buf_min, buf_max);
		}
		
		private:
		bool init (const char * s1, const char * s2, const char * s3, const char * s4)
		{
			if (s1 != nullptr)
			{
				size_t len = strlen (s1);
				mask = std::make_unique<char[]> (len + 1);
				if (mask != nullptr)
				{
					for (size_t i = 0; i <= len; i++)
						mask[i] = s1[i];
				}
				else return false;
			}
			if (s2 != nullptr)
			{
				size_t len = strlen (s2);
				sence = std::make_unique<char[]> (len + 1);
				if (sence != nullptr)
				{
					for (size_t i = 0; i <= len; i++)
						sence[i] = s2[i];
				}
				else return false;
			}
			if (s3 != nullptr)
			{
				size_t len = strlen (s3);
				min = std::make_unique<char[]> (len + 1);
				if (min != nullptr)
				{
					for (size_t i = 0; i <= len; i++)
						min[i] = s3[i];
				}
				else return false;
			}
			if (s4 != nullptr)
			{
				size_t len = strlen (s4);
				max = std::make_unique<char[]> (len + 1);
				if (max != nullptr)
				{
					for (size_t i = 0; i <= len; i++)
						max[i] = s4[i];
				}else return false;
			}
			return true;
		}
		void erase ()
		{
			mask.reset ();
			sence.reset ();
			min.reset ();
			max.reset ();
		}

};


# endif // razbor