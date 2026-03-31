# ifndef command_H
# define command_H
# include <stdio.h>
# include "record.h"
// # include "condition.h"
 # include "operation.h"
// # include "ordering.h"
# include "command_type.h"



enum class by_who
{
	name,
	phone,
	group,
	none,
};


bool where (char *s);
int where_ord (char * s);
bool read_or(ordering *place, char * s, int i);
int read_nach(char *s, ordering *place, char ** end);
bool read_ending(char *s, ordering *place);
int check_com(command_type f, char *con, char *who, char **end2);

bool where (char *s)
{
	if (strcmp(s, "where") == 0) return true;
	return false;

}


int where_ord (char * s, char ** end)
{
	if (strcmp(s, "where") == 0) return 1;
	if (strcmp(s, "order") == 0)
	{
		s = strtok_r(*end, " \t\n", end);
		if (strcmp(s, "by") == 0) return 2;
		return 0;
	}
	return false;
}


bool read_or(ordering *place, char * s, int i)
{
	if (strcmp(s, "*") == 0)
	{
		if (i == 0) 
		{
			place[0] = ordering::name;
			place[1] = ordering::phone;
			place[2] = ordering::group;
			return true;
		}
		return false;
	}
	if (strcmp(s, "name") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::name;
		return true;
	}
	if (strcmp(s, "phone") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::phone;
		return true;
	}
	if (strcmp(s, "group") == 0) 
	{
		if (place[i] != ordering::none) return false;
		place[i] = ordering::group;
		return true;
	}
	return false;

}


int check_com(command_type f, char *con, char *who, char **end2)
{
	//printf("CHEEEK\n");
	if (f == command_type::select)
	{
		//printf("hello\n");
		if(con == nullptr) 
		{
			*end2 = who;
			//printf("22222222\n");
			return 2;
		}
		if (strcmp(con, "order") == 0)
		{
			//printf("case1\n");
			who = strtok_r(who, " \t\n", end2);
			if (strcmp(who, "by") == 0) 
			{
				//printf("1111111\n");
				return 1;
			}
			return 0;
		}
		
		//printf("bad\n");
	}
	if (f == command_type::del)
	{
		//printf("wow\n");
		if (con == nullptr) return 1;
	}
	return 0;

}



bool read_ending(char *s, ordering *place)
{
	char *end = nullptr;
	s = strtok_r(s, " ,\t\n", &end);
	if (s == nullptr) return false;
	//printf("%s\n", s);	
	if (read_or(place, s, 0) == false) return false;
	s = strtok_r(end, " ,\t\n", &end);
	if (s == nullptr) return true;	
	if (read_or(place, s, 1) == false) return false;
	s = strtok_r(end, " ,\t\n", &end);
	if (s == nullptr) return true;
	//printf("%s\n", s);
	if (read_or(place, s, 2) == false) return false;
	s = strtok_r(end, " ,\t\n", &end);
	if (s == nullptr) return true;
	return false;
}



int read_nach(char *s, ordering *place, char ** end)
{
	int res = 0;
	s = strtok_r(s, " ,\t\n", end);
	if (s == nullptr) return 0;
	//printf("%s\n", s);	
	if (read_or(place, s, 0) == false) return 0;

	s = strtok_r(*end, " ,\t\n", end);
	if (s == nullptr) return 3;	
	//printf("%s\n", s);
	if ((res = where_ord(s, end)) != 0) return res;	
	if (read_or(place, s, 1) == false) return 0;


	s = strtok_r(*end, " ,\t\n", end);
	if (s == nullptr) return 3;
	//printf("%s\n", s);

	if ((res = where_ord(s, end)) != 0) return res;

	//printf("Case word word word where\n");
	if (read_or(place, s, 2) == false) return false;
	s = strtok_r(*end, " ,\t\n", end);
	if (s == nullptr) return 3;
	//printf("%s\n", s);
	if ((res = where_ord(s, end)) != 0) return res;
	return 0;
}




class command : public record
{
	private:
		static const int max_items = 3;
		command_type type = command_type::none;
		condition c_name = condition::none;
		condition c_phone = condition::none;
		condition c_group = condition::none;
		operation op = operation::none;
		ordering order[max_items] = { };
		ordering order_by[max_items] = { };
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

		command_type get_type()
		{ return type; }


		ordering * get_order()
		{ return order; }


		ordering * get_order_by()
		{ return order_by; }


		operation get_op()
		{ return op; }


		bool read_command(char *s)
		{
			this->erase_command();
			//printf("buuu\n");
			char *start = s, *end = nullptr;
			start = strtok_r(s, " \t\n", &end);
			//printf("start = %s\n", start);
			if (start == nullptr) return true;
			if (strcmp(start, "quit") == 0)
			{
				type = command_type::quit;
				return true;
			} 
			if (strcmp(start, "insert") == 0)
			{
				type = command_type::insert;

				//read name
				s = strtok_r(end, " (,\t\n", &end);
				if (s == nullptr) return false;
				//printf("name = %s\n", s);
				if (this->set_name(s) != true) return false;

				//read phone
				s = strtok_r(end, " ,\t\n", &end);
				if (s == nullptr) return false;
				//printf("phone = %s\n", s);
				int ph = 0;
				if (sscanf(s, "%d", &ph) != 1) return false;
				this->set_phone(ph);

				//read group 
				s = strtok_r(end, " ),\t\n", &end);
				//printf("group = %s\n", s);
				if (s == nullptr) return false;
				int gr = 0;
				if (sscanf(s, "%d", &gr) != 1) return false;
				this->set_group(gr);
				return true;
			}
			if (strcmp(start, "select") == 0)
			{
				type = command_type::select;
				int res = 0;
				char *end1 = nullptr, *end2 = nullptr;
				if ((res = read_nach(end, order, &end1)) == 0) return false;
				//printf("We read the what filds should be printed\n");
				if (res == 1)//where
				{
					//printf("read where\n");
					if ((res = this->parse(end1, &end2)) == 0) return false;
					//printf("We read the uslovia poiska %d\n", res);
					if (res == 2) return true;
					if (read_ending(end2, order_by) == false) return false;
					//printf("where is the problem?\n");
					return true;
				}

				if (res == 2)//not where
				{
					if (read_ending(end1, order_by) == false) return false;
					//printf("where is the problem?\n");
					return true;
				}
				if (res == 3) return true;//nth
				
			}
			if (strcmp(start, "delete") == 0)
			{
				type = command_type::del;
				char *end2 = nullptr;
				s = strtok_r(end, " \t\n", &end);
				if (s == nullptr) return true;
				if (where(s) == false) return false;
				//printf("delete where\n");
				if (this->parse(end, &end2) == false) return false;
				//printf("We read the uslovia poiska\n");
				return true;
			}
			return false;

		}

		bool read_com(char *who, char *what, char *where, char **ost)
		{
			if (strcmp(who, "name") == 0)
			{
				if (c_name != condition::none) 
				{
					printf(" Filds are not different\n");
					return false;
				}
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
				else if(strcmp(what, "not") == 0)
				{
					what = strtok_r(where, " \t\n", &where);
					if (what == nullptr) return false;
					//printf("what %s\n", what);
					if (strcmp(what, "like") == 0)
						c_name = condition::nlike;
					else return false;
					//printf("ok\n");
				}
				else
					return false;
				if (where == nullptr) return false;
				where = strtok_r(where, " \t\n", ost);
				//printf("where %s\n", where);
				// if (*ost == nullptr) printf("GOOD\n");
				if (this->set_name(where) != true) return false;
				return true;
				
			}
			if (strcmp(who, "phone") == 0)
			{
				if (c_phone != condition::none) 
				{
					printf(" Filds are not different\n");
					return false;
				}
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
				if (where == nullptr) return false;
				where = strtok_r(where, " \t\n", ost);
				//printf("where %s\n", where);
				if (sscanf(where, "%d", &ph) != 1)
					return false;
				set_phone(ph);
				return true;
			}
			if (strcmp(who, "group") == 0)
			{
				if (c_group != condition::none) 
				{
					printf(" Filds are not different\n");
					return false;
				}
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
				if (where == nullptr) return false;
				where = strtok_r(where, " \t\n", ost);
				//printf("where %s\n", where);
				if (sscanf(where, "%d", &gr) != 1)
					return false;
				set_group(gr);
				return true;
			}
			return false;	
		}
		

		bool read_op(char *con)
		{
			if (strcmp(con, "and") == 0)
			{
				if (op == operation::lor) return false;
				op = operation::land;
				return true;
			}
			if (strcmp(con, "or") == 0)
			{
				if (op == operation::land) return false;
				op = operation::lor;
				return true;
			}
			return false;
		}

		int parse (char * string, char **end2)
		{
			int res = 0;
			char *who = nullptr, *what = nullptr, *where = nullptr, *con = nullptr;
			who = strtok_r(string, " \t\n", &what);
			//printf("who %s\n", who);
			if (who == nullptr) return 0; 
			what = strtok_r(what, " \t\n", &where);
			if (what == nullptr) return 0;
			//printf("what %s\n", what);
			if (this->read_com(who, what, where, &con) == false) return 0;
			con = strtok_r(con, " \t\n", &who);
			//printf("truble\n");
			//if (con == nullptr) return true;
			//printf("pupupupu0\n");
			if ((res = check_com(type, con, who, end2)) != 0) return res;
			//printf("op %s\n", con);
			if(this->read_op(con) == false) return 0;
			//printf("pupupupu\n");

			if (who == nullptr) return 0;
			who = strtok_r(who, " \t\n", &what);
			//printf("who %s\n", who);
			if (who == nullptr) return 0; 
			what = strtok_r(what, " \t\n", &where);
			if (what == nullptr) return 0;
			//printf("what %s\n", what);
			if (this->read_com(who, what, where, &con) == false) return 0;
			con = strtok_r(con, " \t\n", &who);
			//if (con == nullptr) return true;
			if ((res = check_com(type, con, who, end2)) != 0) return res;
			//printf("op %s\n", con);
			if(this->read_op(con) == false) return 0;

			if (who == nullptr) return 0;
			who = strtok_r(who, " \t\n", &what);
			//printf("who %s\n", who);
			if (who == nullptr) return 0; 
			what = strtok_r(what, " \t\n", &where);
			if (what == nullptr) return 0;
			//printf("what %s\n", what);
			if (this->read_com(who, what, where, &con) == false) return 0;
			con = strtok_r(con, " \t\n", &who);
			//if (con == nullptr) return true;
			if ((res = check_com(type, con, who, end2)) != 0) return res;
			return 0;
		}
		bool like()
		{
			if (c_name == condition::like || c_name == condition::nlike) return true;
			return false;
		}
		// Print parsed structure
		void print (FILE *fp = stdout) const
		{
			if (type == command_type::insert) 
			{
				fprintf(fp, "We want to add a person\n");
				const record *a = this;
				a->print(order);
				return;
			}
			if (type == command_type::quit)
			{
				fprintf(fp, "We want to QUIT\n");
				return;
			}
			if (type == command_type::none)
			{
				return;
			}
			if (type == command_type::select) fprintf(fp, "We want to find people with\n");
			if (type == command_type::del) fprintf(fp, "We want to delete people with\n");
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
					case condition::nlike:
					fprintf(fp, "not like ");
					break;
					case condition::none:
					break;
				}
				fprintf(fp, "%s\n", this->get_name());
				if (op != operation::none)
				{
					if (op == operation::lor) printf("OR\n");
					else printf("AND\n");
				}
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
					case condition::nlike:
					case condition::none:
					break;
				}
				fprintf(fp, "%d\n", this->get_phone());
				if (op != operation::none)
				{
					if (op == operation::lor) printf("OR\n");
					else printf("AND\n");
				}
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
					case condition::nlike:
					case condition::none:
					break;
				}
				fprintf(fp, "%d\n", this->get_group());
			}
			printf("\n");
		}
		void print_order_by()
		{
			printf("Order by:\n");
			for (int i = 0; i < max_items; i++)
			{
				switch (order_by[i])
				{
					case ordering::none:
					break;
					case ordering::name:
					printf(" name");
					break;
					case ordering::phone:
					printf(" phone");
					break;
					case ordering::group:
					printf(" group");
					break;
				}
			}
				
			printf("\n");
		}


		condition get_c_name()
		{ return c_name; }
		condition get_c_phone()
		{ return c_phone; }
		condition get_c_group()
		{ return c_group; }
		




		void erase_command ()
		{
			this->erase();
			type = command_type::none;
			c_name = condition::none;
			c_phone = condition::none;
			c_group = condition::none;
			op = operation::none;

			for (int i = 0; i < max_items; order[i] = ordering::none, order_by[i] = ordering::none, i++);
		}


		bool apply_ph_and_gr(const record& x)
		{
			if (c_phone != condition::none)
			{
				bool res = x.compare_phone(c_phone, *this);
				if (res == false) return false;
			}
			if (c_group != condition::none)
			{
				bool res = x.compare_group(c_group, *this);
				if (res == false) return false;
			}
			return true;

		}


		// Apply command, return comparision result for record ’x’
		bool apply (const record& x, razbor *Raz = nullptr) const
		{
			//printf("pupupupu\n");
			if (c_phone != condition::none)
			{
				bool res = x.compare_phone(c_phone, *this);
				if (op != operation::lor)
				{
					if (res == false) return false;
				}
				else if (res == true) return true;
			}
			if (c_group != condition::none)
			{
				bool res = x.compare_group(c_group, *this);
				if (op != operation::lor)
				{
					if (res == false) return false;
				}
				else if (res == true) return true;//one of conditions is right
			}
			if (c_name != condition::none)
			{
				//printf("Lets check name\n");
				bool res = x.compare_name(c_name, *this, Raz);
				return res;
			}
			if (op == operation::lor) return false;
			return true;
		}





		bool apply_another(by_who who, const record& x, razbor* Rez = nullptr)
		{
			switch (who)
			{
				case by_who::none:
					return apply(x, Rez);
				case by_who::name:
					if (!(x.compare_group(c_group, *this))) return false;
					return x.compare_phone(c_phone, *this);
				case by_who::phone:
					if (!(x.compare_group(c_group, *this))) return false;
					return x.compare_name(c_name, *this, Rez);
				case by_who::group:
					if (!(x.compare_phone(c_phone, *this))) return false;
					return x.compare_name(c_name, *this, Rez);
				
			}
			return false;
		}



};











# endif