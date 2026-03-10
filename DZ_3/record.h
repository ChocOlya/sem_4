# ifndef record_H
# define record_H
# include <memory>
# include <stdio.h>
# include "condition.h"
# include <string.h>
# include <stdio.h>
# define LEN 1234
using namespace std;




enum class io_status
{
	success,
	eof,
	format,
	memory,
};

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
		
		bool compare_name (condition x, const record& y) const
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
				return false;
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
				return false; // cannot be used for group
			}
			return false;
		}
		void print (FILE * fp = stdout)
		{
			fprintf(fp, "%s %d %d\n", name.get(), phone, group);
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
};


# endif



