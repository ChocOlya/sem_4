# ifndef list2_H
# define list2_H
# include <stdio.h>
# include "command.h"
# include "razbor.h"
# include "avl_tree.h"
# include "list2_node.h"




class list2
{
	private:
		list2_node * head = nullptr;
	public:
		list2 () = default;
		~list2 ()
		{
			this->delete_list();
		}
		list2_node *get_head() { return head; }
		io_status read (int K, avl_tree * garden, FILE *fp = stdin)
		{
			list2_node buf;
			io_status ret;
			list2_node *curr, *tail;
			if ((ret = buf.read(fp)) != io_status::success)
			{
				return ret;
			}
			head = new list2_node;
			if (head == nullptr)
				return io_status::memory;
			*head = (list2_node&&) buf;
			curr = head;
			int res = hash_f(K, *curr);
			garden[res].read(curr);
			//garden[res].print();
			//printf("\n");
			while ((ret = buf.read(fp)) == io_status::success)
			{
				tail = new list2_node;
				if (tail == nullptr)
				{
					this->delete_list();
					return io_status::memory;
				}
				*tail = (list2_node&&) buf;
				curr->set_next(tail);
				tail->set_prev(curr);
				curr = tail;
				res = hash_f(K, *curr);
				garden[res].read(curr);
				//garden[res].print();
				//printf("\n");

			} 
			if(!feof(fp))
			{
				this->delete_list();
				return io_status::format;
			}
			return io_status::success;
		}
		list2_node * apply(int K, command* test, avl_tree * garden, razbor *HELP = nullptr)
		{
			list2_node *curr = head;
			list2_node *head_el = nullptr;
			list2_node *next_el = nullptr;
			//printf("buuu0\n");
			if (test->get_c_name() == condition::eq && test->get_op() != operation::lor)//when we know the name of evrybody
			{
				//printf("IN TREE\n");
				int index = hash_f(K, *test);
				head_el = garden[index].find_in_tree(test);
				return head_el;
			}
			else
			{
				//printf("NOT IN TREE\n");
				while(curr != nullptr)
				{	
					//printf("muuu\n");
					if (test->apply(*curr, HELP) == true)
					{
						//printf("buuu\n");
						if (head_el == nullptr)
						{
							head_el = curr;
							next_el = curr;
							//printf("We found the first person\n");
							//head_el->print();
						}
						else
						{
							next_el->next_select = curr;
							next_el = curr;
						}
					}
					curr = curr->next;
					//printf("muuuuuuuuuuuuu\n");
				}
				if (next_el != nullptr) next_el->next_select = nullptr;
				return head_el;
			}
			return nullptr;
		}

		void do_st(int K, list2_node *head_el, command* test, avl_tree * garden, int& kol)
		{

			if (test->get_type() == command_type::select)
			{
				list2_node *head_new = head_el;
				//print_select(head_el);
				ordering *oR = test->get_order_by();
				if (oR[0] != ordering::none) 
				{
					//test->print_order_by();
					head_new = sort(head_el, oR);
				}
				//print_select(head_new);
				ordering *order = test->get_order();
				while (head_new != nullptr)
				{
					kol++;
					head_new->print(order);
					head_new = head_new->next_select;
				}
				printf("\n");
				return;
			}
			if (test->get_type() == command_type::del)
			{
				list2_node *curr = head_el;
				// printf("We want to delete:\n");
				// print_select(head_el);
				// printf("From:\n");
				// this->print();
				

				while(head_el != nullptr)
				{
					//printf("pupupu\n");
					if (head == head_el)
					{
						//printf("Case1\n");
						head = head_el->next;
						if (head != nullptr) head->prev = nullptr;
						curr = head_el->next_select;
						// printf("Delete the member ");
						// head_el->print();
						int index = hash_f(K, *head_el);
						garden[index].delete_node(head_el);
						garden[index].print();
						// printf("NOW:\n");
						// this->print();
						delete head_el;
						head_el = curr;
					}
					else
					{
						//printf("Case2\n");
						if (head_el->prev != nullptr) head_el->prev->next = head_el->next;
						if (head_el->next != nullptr) head_el->next->prev = head_el->prev;
						curr = head_el->next_select;
						// printf("Delete the member ");
						// head_el->print();
						int index = hash_f(K, *head_el);
						//garden[index].print();
						garden[index].delete_node(head_el);
						//garden[index].print();
						// printf("NOW:\n");
						//this->print();
						delete head_el;
						head_el = curr;
					}
				}
				return;
			}
		}

		static void print_select(list2_node *head)
		{
			while (head != nullptr)
			{
				head->print();
				head = head->next_select;
			}
		}


		bool find(command *test)
		{
			list2_node *curr = head;
			while (curr != nullptr)
			{
				if (equal(*curr, *test) == true) return true;
				curr = curr->next;
			}
			//printf("FALSE\n");
			return false;
		}


		static list2_node *sort(list2_node *head, ordering* place)
		{
			int len = 1, i = 0, len1 = 0, len2 = 0, kol = 0; 
			list2_node *start_sorted = nullptr, *end_sorted = nullptr, *ost = nullptr;
			list2_node *curr1 = head, *curr2 = nullptr, *pred_curr2 = nullptr;
			list2_node *start_pred = nullptr, *end_pred = nullptr;



			if(head == nullptr)
				return nullptr;
			while (kol != 1)
			{
				kol = 0;
				start_pred = nullptr;
				end_pred = nullptr;
				ost = head;
				while (ost != nullptr)
				{
					curr1 = ost;
					curr2 = curr1;
					start_sorted = nullptr;
					end_sorted = nullptr;
					pred_curr2 = nullptr;
					kol++;
					for(i = 0; i < len && curr2; i++, pred_curr2 = curr2, curr2 = curr2->next_select);
					len1 = 0;//kolvo, written of the first
					len2 = 0;//kolvo, written of the second
					while(curr1 && curr2 && len1 < len && len2 < len)
					{				
						if (cmp(*curr1, *curr2, place) < 0)//(*curr1 < *curr2)
						{
							if (start_sorted == nullptr) 
								start_sorted = curr1;

							else end_sorted->next_select = curr1;
							end_sorted = curr1;
							curr1 = curr1->next_select;
							end_sorted->next_select = nullptr; 
							len1++;
						}
						else
						{
							if (start_sorted == nullptr) 
								start_sorted = curr2;
						
							else end_sorted->next_select = curr2;
							end_sorted = curr2;
							curr2 = curr2->next_select;
							end_sorted->next_select = nullptr;
							len2++;
						}
					}
					if (curr1 && len1 < len)
					{
						if (start_sorted == nullptr) start_sorted = curr1;
						else end_sorted->next_select = curr1;
						end_sorted = pred_curr2;
						end_sorted->next_select = curr2;
					}
					else
					{
						end_sorted->next_select = curr2;
						for (i = 0; curr2 && i < (len - len2); i++, pred_curr2 = curr2, curr2 = curr2->next_select);
						end_sorted = pred_curr2;
						end_sorted->next_select =  curr2;

					}
					if(start_pred == nullptr) start_pred = start_sorted;
					else end_pred->next_select = start_sorted;
					end_pred = end_sorted;
					

					ost = end_pred->next_select;
					if (kol == 1) head = start_pred;
				}		
				
				len *= 2;
				
			}
			//printf("len = %d\n", len);
			return head;
		}

		void print ()
		{
			list2_node *curr;
			unsigned int i = 0;
			for (i = 0, curr = head; curr != nullptr; curr = curr->get_next(), i++)
			{
				curr->print();
			}
		}


		// void add_value(int K, command * test, avl_tree *garden)
		// {
		// 	list2_node *curr = head;
		// 	head = new list2_node;
		// 	head->next = curr;
		// 	if (curr != nullptr) curr->prev = head;
		// 	// head->set_group(test->get_group());
		// 	// head->set_phone(test->get_phone());
		// 	*(record*)head = static_cast<record&&>(*test);
		// 	garden[hash_f(K, *head)].read(head);

		// 	return;


		// }


		void insert(int K, command * test, avl_tree * garden)
		{
			int index = hash_f(K, *test);
			list2_node *el = garden[index].read_record(*test);//find in tree and retern lis2_node where it would lie
			if (el == nullptr) return;
			el->next = head;
			if (head != nullptr) head->prev = el;
			head = el;
			//garden[index].print();
		}

	private:
		void delete_list()
		{
			list2_node *curr = head, *next;
			for (curr = head; curr; curr = next)
			{
				next = curr->next;
				delete curr;
			}
		}


};

# endif // list2


