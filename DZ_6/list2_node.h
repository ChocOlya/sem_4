# ifndef LIST2_NODE
# define LIST2_NODE
#include "definition_of_k.h"

int hash_f(const record& x);

class list2_node : public record
{
	private:
		list2_node * next = nullptr;
		list2_node * prev = nullptr;
		list2_node * next_select = nullptr;
	public:
		list2_node ()  = default;
		list2_node (const list2_node&) = delete;
		list2_node (list2_node&& r) : record((record&&)r)
		{
			next = r.next; r.next = nullptr;
			prev = r.prev; r.prev = nullptr;
		}
		~list2_node ()
		{
			next = nullptr;
			prev = nullptr;
		}
		list2_node& operator= (const list2_node&) = delete;
		list2_node&& operator= (list2_node&& r)
		{
			*(record*)(this) = (record&&)r;
			next = r.next;
			prev = r.prev; 
			r.next = nullptr;
			r.prev = nullptr;
			return (list2_node&&)*this;
		}
		
		list2_node * get_next () const
		{
			return next;
		}
		void set_next (list2_node *r)
		{
			next = r;
		}
		list2_node * get_prev () const
		{
			return prev;
		}
		void set_prev (list2_node *r)
		{
			prev = r;
		}
		list2_node * get_next_select () const
		{
			return next_select;
		}
		void set_next_select (list2_node *r)
		{
			next_select = r;
		}
		int operator< (const list2_node& x) const { return this->cmp (x) < 0; }
		int operator<= (const list2_node& x) const { return this->cmp (x) <= 0; }
		int operator> (const list2_node& x) const { return this->cmp (x) > 0; }
		int operator>= (const list2_node& x) const { return this->cmp (x) >= 0; }
		int operator== (const list2_node& x) const { return this->cmp (x) == 0; }
		int operator!= (const list2_node& x) const { return this->cmp (x) != 0; }


		int cmp (const list2_node& x) const
		{ 
			const char * name1 = this->get_name(), *name2 = x.get_name();
			if (name1 == nullptr)
			{
				if (name2 == nullptr) return 0;
				return -1;
			} 
			if (name2 == nullptr) return 1;
			return strcmp(this->get_name(), x.get_name());
		}

	friend class list2;
};



int hash_f(const record& x)
{
	const char *s = x.get_name();
	int res = 0;
	for (int i = 0; s[i] != '\0'; i++) res += s[i];
	res = res % K;
	return res; 
}



#endif //lis2_node

