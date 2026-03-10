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


