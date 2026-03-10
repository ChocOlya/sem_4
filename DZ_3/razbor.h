# ifndef razbor_H
# define razbor_H
#include <stdio.h>
#include <string.h>
#include <memory>
#define LEN 1234



class razbor
{
	private:
		std::unique_ptr<char[]> mask;
		std::unique_ptr<char[]> sence;
		std::unique_ptr<char[]> min;
		std::unique_ptr<char[]> max;
		int flag = 0;
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
		int get_flag ()
		{
			return flag;
		}
		bool read (const char *s)
		{
			char buf_m[LEN];
			char buf[LEN];
			char buf_min[LEN];
			char buf_max[LEN];
			int k = 0;
			for (int i = 0; s[i]; i++)
			{
				if((flag == 0 || flag == 3 || flag == 4) && s[i] == '[')
				{
					buf_m[k] = '1';
					buf[k] = '@';
					i++;
					if (!s[i] || s[i] == '[' || s[i] == ']') return false;//[a
					if (s[i] == '\\')
					{
						i++;
						if (!s[i]) return false;
					} 
					if ((flag == 0 || flag == 4) && s[i] == '^' )
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
							flag = 4;
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
								//printf("case [^-a\n");
								if (flag == 4) return false;
								flag = 3;
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
									if (flag == 4) return false;
									flag = 3;
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
									flag = 4;
									k++;
									i++;
									if (!s[i] || s[i] != ']') return false;
								}
							}
						}
					}
					else 
					{
						if (flag == 4) return false;
						flag = 3;
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
				else if ((flag == 0 || flag == 1) && s[i] == '_')
				{
					flag = 1;
					buf_m[k] = '1';
					buf[k] = '@';
					buf_min[k] = '@';
					buf_max[k] = '@';
					k++;
				}
				else if ((flag == 0 || flag == 2) && s[i] == '%')
				{
					flag = 2;
					buf_m[k] = '1';
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
			//printf("MASK %s\nSence %s\nMin %s\nMax %s\nflag = %d\n", buf_m, buf, buf_min, buf_max, flag);
			erase ();
			return init (buf_m, buf, buf_min, buf_max);
		}
		int check(const char *str)
		{
			int i = 0;
			char *m = mask.get(), *s = sence.get(), *l = min.get(), *r = max.get();
			for (; str[i]; i++)
			{
				if (!m[i]) return 0;
				if (m[i] == '0')
				{
					if (str[i] != s[i]) return 0;
				}
				else
				{
					if (flag == 3)
					{
						if (str[i] < l[i] || str[i] > r[i]) return 0;
					}
					if (flag == 4)
					{
						if (str[i] >= l[i] && str[i] <= r[i]) return 0;
					}
				}
			}
			if (!m[i]) return 1;
			return 0;
		}

		int understand(char **s1, char **s2)
		{
		/*
		• "text%" – соответствует всем словам, начинающимся с "text", - 1
		• "%text" – соответствует всем словам, заканчивающихся на "text", - 2
		• "%text%" – соответствует всем словам, включающим в себя "text", - 3
		• "text1%text2" - 4*/
			char *m = mask.get(), *s = sence.get();
			int i = 0, flag1 = 0, flag2 = 0; 
			for (; m[i]; i++)
			{
				if (m[i] == '1')
				{
					if (i == 0) 
					{
						flag1 = 1;
					} 
					else 
					{
						flag2 = i;
						s[i] = '\0'; 
						//printf("%s\n%s\n", s, s + i + 1);
						break;
					}

				}
				
			}
			if (flag2)
			{
				if (m[flag2 + 1] == '\0')
				{
					if (flag1) 
					{
						*s1 = s + 1;
						return 3;
					}
					else 
					{
						*s1 = s;
						return 1;
					}
				}
				*s1 = s;
				*s2 = s + flag2 + 1;
				//printf("%s\n", *s1);
				return 4;
			} 
			if (flag1)
			{
				*s1 = s + 1;
				return 2;
			}
			*s1 = s;
			return 0;
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
		}

};


# endif // razbor