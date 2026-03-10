# ifndef record_H
# define record_H
# include <memory>
# include <stdio.h>
# include "condition.h"
# include "ordering.h"
# include <string.h>
# include <stdio.h>
# include "razbor.h"
# define LEN 1234
using namespace std;




enum class io_status
{
	success,
	eof,
	format,
	memory,
};



int cmp(record x, record y, ordering *place);
int cmp(record x, record y, ordering *place)
{
	int res = 0;
	if (res = cmp_0(x, y, place[0])) return res;
	if (res = cmp_0(x, y, place[1])) return res;
	return cmp_0(x, y, place[2]);
	
}



int cmp_0(record x, record y, ordering o)
{
	switch (o)
	{
		case ordering::name:
		return strcmp(x.get_name(), y.get_name());
		case ordering::phone:
		return x.get_phone() - y.get_phone();
		case ordering::group:
		return x.get_group() - y.get_group();
		case ordering::none:
		return 0;
	}
	return 0;
}





class record
{
	private:
		std::unique_ptr<char []> name = nullptr;
		int phone = 0;
		int group = 0;
	public:
		record () = default;
		~record () = default;
		const char * get_name () const { return name.get (); }
		int get_phone () const { return phone; }
		int get_group () const { return group; }
		int init (const char *n, int p, int g)
		{
			this->erase();
			phone = p;
			group = g;
			if (n)
			{
				name = make_unique<char []> (strlen (n) + 1);
				if (!name) return -1;
				strcpy (name.get(), n);
			}
			else name = nullptr;
			return 0;
		}
		bool set_name(char *n)
		{
			if (n)
			{
				name = make_unique<char []> (strlen (n) + 1);
				if (!name) return false;
				strcpy (name.get(), n);
			}
			else name = nullptr;
			return true;
		}
		void set_phone(int p)
		{ phone = p; }
		 void set_group(int g)
		{ group = g; }
		// Allow as return value for functions
		record (record &&x) = default; // move constructor
		// Assignement move operator
		record& operator= (record&& x) = default;
		// Prohoibit pass by value
		// (it is default when move constructor is declared)
		record (const record &x) = delete;
		// Prohoibit assignement operator
		// (it is default when move constructor is declared)
		record& operator= (const record&) = delete;
		
		bool compare_name (condition x, const record& y, razbor *Raz) const
		{// Check condition ’x’ for field ’name’ for ’this’ and ’y’
			switch (x)
			{
				case condition::none: // not specified
				return true; // unspecified opeation is true
				case condition::eq: // equal
				return strcmp(name.get(), y.name.get()) == 0;
				case condition::ne: // not equal
				return strcmp(name.get(), y.name.get()) != 0;
				case condition::lt: // less than
				return strcmp(name.get(), y.name.get()) < 0;
				case condition::gt: // less than
				return strcmp(name.get(), y.name.get()) > 0;
				case condition::le: // less equal
				return strcmp(name.get(), y.name.get()) <= 0;
				case condition::ge: // great equal
				return strcmp(name.get(), y.name.get()) >= 0;
				case condition::like: // strings only: match pattern
				return check(name.get(),Raz->get_mask(), Raz->get_sence(), Raz->get_min(), Raz->get_max());
				case condition::nlike:
				return (!check(name.get(),Raz->get_mask(), Raz->get_sence(), Raz->get_min(), Raz->get_max()));
			}
			return false;
		}
		bool compare_phone (condition x, const record& y) const
		{// Check condition ’x’ for field ’phone’ for ’this’ and ’y’
			switch (x)
			{
				case condition::none: // not specified
				return true; // unspecified opeation is true
				case condition::eq: // equal
				return phone == y.phone;
				case condition::ne: // not equal
				return phone != y.phone;
				case condition::lt: // less than
				return phone < y.phone;
				case condition::gt: // less than
				return phone > y.phone;
				case condition::le: // less equal
				return phone <= y.phone;
				case condition::ge: // great equal
				return phone >= y.phone;
				case condition::like: // strings only: match pattern
				case condition::nlike:
				return false; // cannot be used for phone
			}
			return false;
		}
		
		bool compare_group (condition x, const record& y) const
		{// Check condition ’x’ for field ’group’ for ’this’ and ’y’
			switch (x)
			{
				case condition::none: // not specified
				return true; // unspecified opeation is true
				case condition::eq: // equal
				return group == y.group;
				case condition::ne: // not equal
				return group != y.group;
				case condition::lt: // less than
				return group < y.group;
				case condition::gt: // less than
				return group > y.group;
				case condition::le: // less equal
				return group <= y.group;
				case condition::ge: // great equal
				return group >= y.group;
				case condition::like: // strings only: match pattern
				case condition::nlike:
				return false; // cannot be used for group
			}
			return false;
		}
		void print (ordering *place, FILE * fp = stdout)
		{
			for (int i = 0; i < 3; i++)
			{
				switch(place[i])
				{
					case ordering::name:
					if (i) fprintf(fp, " ");
					fprintf(fp, "%s", name.get());
					continue;
					case ordering::phone:
					if (i) fprintf(fp, " ");
					fprintf(fp, "%d", phone);
					continue;
					case ordering::group:
					if (i) fprintf(fp, " ");
					fprintf(fp, "%d", group);
					continue;
					case ordering::none:
					fprintf(fp, "\n");
					return;
				}
			}
			fprintf(fp, "\n");
			
		}
		io_status read (FILE *fp = stdin)
		{
			char buf[LEN];
			name = nullptr;
			if (fscanf (fp, "%s%d%d", buf, &phone, &group) != 3)
			{
				if (feof(fp)) return io_status::eof;
				return io_status::format;
			}
			if (init (buf, phone, group)) return io_status::memory;
			return io_status::success;
		}
		void erase ()
		{
			name.reset ();
			phone = 0;
			group = 0;
		}

};


# endif



