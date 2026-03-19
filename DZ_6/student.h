# ifndef student_H
# define student_H
#include <stdio.h>
#include <string.h>
#include <memory>


enum class io_status
{
	success,
	eof,
	format,
	memory,
};



class student
{
	private:
		std::unique_ptr<char[]> name;
		int value = 0;
	public:
		student () = default;
		student (const student& x) = delete;
		// По умолчанию: переместить все поля класса - подходит
		student (student&& x) = default;
		~student () = default;
		student& operator= (const student& x) = delete;
		// По умолчанию: присвоить с перемещением все поля класса - подходит
		student& operator= (student&& x) = default;
		void print (FILE * fp = stdout) const
		{
			fprintf (fp, "%s %d\n", name.get (), value);
		}
		io_status read (FILE * fp = stdin)
		{
			const int LEN = 1234;
			char n[LEN];
			int v;
			if (fscanf (fp, "%s%d", n, &v) != 2)
				return io_status::format;
			erase ();
			return init (n, v);
		}
		int cmp (const student& x) const
		{
			// Используем переопределенную функцию сравнения с nullptr
			if (name == nullptr)
			{
				if (x.name != nullptr)
					return -1;
				return value - x.value;
			}
			if (x.name.get () == nullptr)
				return 1;
			int res = strcmp (name.get (), x.name.get ());
			if (res)
				return res;
			return value - x.value;
		}
		int operator< (const student& x) const { return cmp (x) < 0; }
		int operator<= (const student& x) const { return cmp (x) <= 0; }
		int operator> (const student& x) const { return cmp (x) > 0; }
		int operator>= (const student& x) const { return cmp (x) >= 0; }
		int operator== (const student& x) const { return cmp (x) == 0; }
		int operator!= (const student& x) const { return cmp (x) != 0; }
	private:
		io_status init (const char * n, int v)
		{
			value = v;
			if (n != nullptr)
			{
				size_t len = strlen (n);
				name = std::make_unique<char[]> (len + 1);
				if (name != nullptr)
				{
					for (size_t i = 0; i <= len; i++)
						name[i] = n[i];
				}
				else
					return io_status::memory;
			}
			return io_status::success;
		}
		void erase ()
		{
			value = 0;
			name.reset ();
		}
	public:
		int check01(int *a)
		{
			int i = 0;
			char *n = name.get();
			for (i = 0; n[i] != '\0'; i++)
			{
				if (a[(int)n[i]] == 0) break;
			}
			if (n[i] == '\0') return 1;
			return 0;
		}
};


# endif // student