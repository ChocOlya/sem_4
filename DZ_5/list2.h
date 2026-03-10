# ifndef list2_H
# define list2_H
# include <stdio.h>
# include "command.h"
# include "razbor.h"


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
	friend class list2;
};




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
		io_status read (FILE *fp = stdin)
		{
			list2_node buf;
			io_status ret;
			list2_node *curr, *tail;
			if ((ret = buf.read(fp)) != io_status::success)
				return ret;
			head = new list2_node;
			if (head == nullptr)
				return io_status::memory;
			*head = (list2_node&&) buf;
			curr = head;
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
			} 
			if(!feof(fp))
			{
				this->delete_list();
				return io_status::format;
			}
			return io_status::success;
		}
		list2_node * apply(comand test, int& kol)
		{
			list2_node *curr = head;
			list2_node *head_el = nullptr;
			list2_node *next_el = nullptr;
			while(curr != nullptr)
			{	
				if (test.apply(*curr) == true)
				{
					if (type == comand_type::select) kol++;
					if (head_el == nullptr)
					{
						head_el = curr;
						next_el = curr;
					}
					else
					{
						next_el->next_select = curr;
					}
				}
				curr = curr->next;
			}
			if (next_el != nullptr) next_el->next_select = nullptr;
			return head_el;
		}

		void do_st(list2_node *head_el, command test)
		{

			if (test->get_type() == comand_type::select)
			{
				list2_node head_new = nullptr;
				head_new = sort(head_el, test->get_order_by());
				ordering *order = test->get_order();
				while (head_new != nullptr)
				{
					head_new->print(order);
					head_new = head_new->next_select;
				}
				return;
			}
			if (test->get_type() == comand_type::del)
			{
				list2_node *curr = head_el;
				while(head_el != nullptr)
				{
					if (head == head_el)
					{
						head = head_el->next;
						head->prev = nullptr;
						curr = head_el->next_select;
						delete[] head_el;
						head_el = curr;
					}
					else
					{
						head_el->prev->next = head_el->next;
						head_el->next->prev = head_el->prev;
						curr = head_el->next_select;
						delete[] head_el;
						head_el = curr;
					}
				}
				return;
			}
		}


		static void sort(list2_node *head, ordering place)
		{
			int len = 1, i = 0, len1 = 0, len2 = 0, kol = 0; 
			list_node *start_sorted = nullptr, *end_sorted = nullptr, *ost = nullptr;
			list_node *curr1 = head, *curr2 = nullptr, *pred_curr2 = nullptr;
			list_node *start_pred = nullptr, *end_pred = nullptr;



			if(head == nullptr)
				return;
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
						if (cmp(*curr1, *curr2) < 0)//(*curr1 < *curr2)
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
				return head;
			}
			//printf("len = %d\n", len);

		}

		// void print (unsigned int r = 10, FILE *fp = stdout)
		// {
		// 	list2_node *curr;
		// 	unsigned int i = 0;
		// 	for (i = 0, curr = head; curr != nullptr && i < r; curr = curr->get_next(), i++)
		// 	{
		// 		curr->print(fp);
		// 	}
		// }

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


