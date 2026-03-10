# ifndef command_H
# define command_H
# include <stdio.h>
# include "record.h"



class command : public record
{
	private:
		condition c_name = condition::none;
		condition c_phone = condition::none;
		condition c_group = condition::none;
	public:
		command () = default;
		~command () = default;
		// Convert string command to data structure
		// Example 1: "phone = 1234567" parsed to
		// (command::phone = 1234567, command::c_phone = condition::eq)
		// other fields are unspecified
		// Example 2: "name like St%" parsed to
		// (command::name = "St%", command::c_name = condition::like)
		// other fields are unspecified
		bool parse (char * string)
		{
			char *who = nullptr, *what = nullptr, *where = nullptr;
			who = strtok_r(string, " \n", &what);
			//printf("%s\n", who);
			if (who == nullptr || what == nullptr) return false; 
			what = strtok_r(what, " \n", &where);
			int i = 0;
			for (i = 0; where[i] && where[i] != '\n'; i++);
			where[i] = '\0';
			if (strcmp(who, "name") == 0)
			{
				if (strcmp(what, "=") == 0)
					c_name = condition::eq;
				else if(strcmp(what, "<>") == 0)
					c_name = condition::ne;
				else if(strcmp(what, ">") == 0)
					c_name = condition::gt;
				else if(strcmp(what, "<") == 0)
					c_name = condition::lt;
				else if(strcmp(what, ">=") == 0)
					c_name = condition::ge;
				else if(strcmp(what, "<=") == 0)
					c_name = condition::le;
				else if(strcmp(what, "like") == 0)
					c_name = condition::like;
				else
					return false;
				if (this->init(where, 0, 0) != 0) return false;
				return true;
				
			}
			if (strcmp(who, "phone") == 0)
			{
				if (strcmp(what, "=") == 0)
					c_phone = condition::eq;
				else if(strcmp(what, "<>") == 0)
					c_phone = condition::ne;
				else if(strcmp(what, ">") == 0)
					c_phone = condition::gt;
				else if(strcmp(what, "<") == 0)
					c_phone = condition::lt;
				else if(strcmp(what, ">=") == 0)
					c_phone = condition::ge;
				else if(strcmp(what, "<=") == 0)
					c_phone = condition::le;
				else
					return false;
				int ph = 0;
				if (sscanf(where, "%d", &ph) != 1)
					return false;
				if (this->init(nullptr, ph, 0) != 0) return false;
				return true;

			}
			if (strcmp(who, "group") == 0)
			{
				if (strcmp(what, "=") == 0)
					c_group = condition::eq;
				else if(strcmp(what, "<>") == 0)
					c_group = condition::ne;
				else if(strcmp(what, ">") == 0)
					c_group = condition::gt;
				else if(strcmp(what, "<") == 0)
					c_group = condition::lt;
				else if(strcmp(what, ">=") == 0)
					c_group = condition::ge;
				else if(strcmp(what, "<=") == 0)
					c_group = condition::le;
				else
					return false;
				int gr = 0;
				if (sscanf(where, "%d", &gr) != 1)
					return false;
				if (this->init(nullptr, 0, gr) != 0) return false;
				return true;
				
			}
			return false;
		}
		bool like()
		{
			if (c_name == condition::like) return true;
			return false;
		}
		// Print parsed structure
		void print (FILE *fp = stdout) const
		{
			fprintf(fp, "We want to find people with ");
			if (c_name != condition::none)
			{
				fprintf(fp,"name ");
				switch(c_name)
				{
					case condition::eq:
					fprintf(fp, "= ");
					break;
					case condition::ne:
					fprintf(fp, "<> ");
					break;
					case condition::lt:
					fprintf(fp, "< ");
					break;
					case condition::gt:
					fprintf(fp, "> ");
					break;
					case condition::le:
					fprintf(fp, "<= ");
					break;
					case condition::ge:
					fprintf(fp, ">= ");
					break;
					case condition::like:
					fprintf(fp, "like ");
					break;
					case condition::none:
					break;
				}
				fprintf(fp, "%s\n", this->get_name());
			}
			if (c_phone != condition::none)
			{
				fprintf(fp, "phone ");
				switch(c_phone)
				{
					case condition::eq:
					fprintf(fp, "= ");
					break;
					case condition::ne:
					fprintf(fp, "<> ");
					break;
					case condition::lt:
					fprintf(fp, "< ");
					break;
					case condition::gt:
					fprintf(fp, "> ");
					break;
					case condition::le:
					fprintf(fp, "<= ");
					break;
					case condition::ge:
					fprintf(fp, ">= ");
					break;
					case condition::like:
					case condition::none:
					break;
				}
				fprintf(fp, "%d\n", this->get_phone());
			}
			if (c_group!= condition::none)
			{
				fprintf(fp, "group ");
				switch(c_group)
				{
					case condition::eq:
					fprintf(fp, "= ");
					break;
					case condition::ne:
					fprintf(fp, "<> ");
					break;
					case condition::lt:
					fprintf(fp, "< ");
					break;
					case condition::gt:
					fprintf(fp, "> ");
					break;
					case condition::le:
					fprintf(fp, "<= ");
					break;
					case condition::ge:
					fprintf(fp, ">= ");
					break;
					case condition::like:
					case condition::none:
					break;
				}
				fprintf(fp, "%d\n", this->get_group());
			}
		}
		// Apply command, return comparision result for record ’x’
		bool apply (const record& x) const
		{
			if (c_name != condition::none)
			{
				return x.compare_name(c_name, *this);
			}
			if (c_phone != condition::none)
			{
				return x.compare_phone(c_phone, *this);
			}
			if (c_group != condition::none)
			{
				return x.compare_group(c_group, *this);
			}
			return true;
		}
};
# endif