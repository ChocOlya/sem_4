# ifndef pair_of_gardens_H
# define pair_of_gardens_H

# include "avl_tree.h"


class pair_of_gardens
{
	private:
		avl_tree *garden_name = nullptr;
		avl_tree *garden_phone = nullptr;
		static int K1;
		static int K2;
	public:
	pair_of_gardens() = default;
	pair_of_gardens(const pair_of_gardens&) = delete;
	~pair_of_gardens()
	{
		if (garden_name != nullptr) delete[] garden_name;
		if (garden_phone != nullptr) delete[] garden_phone;
	}
	void set_K1(int K)
	{ K1 = K; }
	void set_K2(int K)
	{ K2 = K; }
	void alloc()
	{
		if (garden_name == nullptr)
		{
			garden_name = new avl_tree [K1];
			for (int i = 0; i < K1; i++) garden_name[i].set_con(ordering::name);
		}
		if (garden_phone == nullptr)
		{
			garden_phone = new avl_tree [K2];
			for (int i = 0; i < K2; i++) garden_phone[i].set_con(ordering::phone);
		}
	}
	void add_value(list2_node *x)
	{
		if (garden_phone == nullptr || garden_name == nullptr) this->alloc();
		int index_n = 0, index_ph = 0;
		hash_f(K1, K2, *x, index_n, index_ph);
		garden_name[index_n].read(x);
		garden_phone[index_ph].read(x);
	}
	list2_node * insert(record& x)
	{
		int index_n = 0, index_ph = 0;
		hash_f(K1, K2, x, index_n, index_ph);
		list2_node *el = garden_phone[index_ph].read_record(x);//find in tree and retern lis2_node where it would lie
		if (el == nullptr) return nullptr;
		garden_name[index_n].read(el);
		return el;
	}
	void del_value(list2_node *el)
	{
		int index_n = 0, index_ph = 0;
		hash_f(K1, K2, *el, index_n, index_ph);
		garden_name[index_n].delete_node(el);
		garden_phone[index_ph].delete_node(el);
	}


	list2_node * name_OR_phone(command *test, int& kol)
	{
		int index_n = 0, index_ph = 0;
		list2_node *end = nullptr, *head_el = nullptr, *pupu = nullptr;
		hash_f(K1, K2, *test, index_n, index_ph);
		head_el = garden_name[index_n].find_in_tree(test, &end, kol);
		if (head_el == nullptr) return garden_phone[index_ph].find_in_tree(test, &pupu, kol);
		end->next_select = end;	
		end->next_select = garden_phone[index_ph].find_in_tree(test, &pupu, kol);
		return head_el;///////CHANGEEEEEEEE
	}


	list2_node *name_phone_AND(command *test, int& kol, razbor *HELP = nullptr)//op = land, name or phone =
	{
		int index_n = 0, index_ph = 0;
		list2_node *pupu = nullptr;
		hash_f(K1, K2, *test, index_n, index_ph);
		if (test->get_c_phone() == condition::eq)
			return garden_phone[index_ph].find_in_tree(test, &pupu, kol, HELP);
		return garden_name[index_n].find_in_tree(test, &pupu, kol);
	}


	list2_node *group_AND(command *test, razbor *HELP, int& kol)
	{
		list2_node *HEAD = nullptr, *END = nullptr, *this_head = nullptr, *this_end = nullptr;
		//printf("GROUP AND\n");
		for (int i = 0; i < K1; i++)
		{
			//printf("cheeeck %d\n", K1);
			this_head = garden_name[i].find_in_all_tree(test, HELP, &this_end, kol);
			//printf("found in tree umber %d\n", i);
			if (this_head != nullptr)
			{
				if (HEAD == nullptr)
				{
					//printf("yeee\n");
					HEAD = this_head;
					END = this_end;
					//HEAD->print();
					//END->print();
				}
				else
				{
					END->next_select = this_head;
					END = this_end;
				}
			}
			this_end = nullptr;
		}
		return HEAD;

	}


	friend class list2;

};






# endif //pair of gardens