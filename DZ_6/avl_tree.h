# ifndef tree_H
# define tree_H
# include "list2_node.h"
# include "command.h"




class list_node 
{
	private:
		list2_node * el = nullptr;
		list_node * next_ = nullptr;
	public:
		list_node ()  = default;
		list_node (const list_node&) = delete;
		list_node (list_node&& r)
		{
			el = r.el; r.el = nullptr;
			next_ = r.next_; r.next_ = nullptr;
		}
		~list_node ()
		{
			// printf("I RM!!\n");
			// print();
			next_ = nullptr;
		}
		list_node& operator= (const list_node&) = delete;
		list_node&& operator= (list_node&& r)
		{
			el = r.el; r.el = nullptr;
			next_ = r.next_;
			r.next_ = nullptr;
			return (list_node&&)*this;
		}
		list_node * get_next_ () const
		{
			return next_;
		}
		void set_next_ (list_node *r)
		{
			next_ = r;
		}
		
	friend class avl_tree_node;
	friend class avl_tree;
};



class avl_tree_node
{
	private:
		list2_node *el = nullptr;
		avl_tree_node * left = nullptr;
		avl_tree_node * right = nullptr;
		list_node * head = nullptr;
		int balance = 0;
	public:
		avl_tree_node() = default;
		avl_tree_node(const avl_tree_node& x) = delete;
		avl_tree_node(avl_tree_node&& x) 
		{
			el = x.el;
			head = x.head;
			erase_links ();
			x.erase_links ();
		}
		~avl_tree_node()
		{
			list_node *curr = head, *next = nullptr;
			for (; curr; curr = next)
			{
				next = curr->next_;
				curr->el = nullptr;//i don't know what i want to do
				delete curr;
			}
			erase_links ();
		}

		/*int operator< (const avl_tree_node& x) const { return cmp (x) < 0; }
		int operator<= (const avl_tree_node& x) const { return cmp (x) <= 0; }
		int operator> (const avl_tree_node& x) const { return cmp (x) > 0; }
		int operator>= (const avl_tree_node& x) const { return cmp (x) >= 0; }
		int operator== (const avl_tree_node& x) const { return cmp (x) == 0; }
		int operator!= (const avl_tree_node& x) const { return cmp (x) != 0; }


		int cmp (avl_tree_node& x)
		{ 
			char * name1 = this->get_name(), *name2 = x.get_name();
			if (name1 == nullptr)
			{
				if (name2 == nullptr) return 0;
				return -1;
			} 
			if (name2 == nullptr) return 1;
			return strcmp(this->get_name(), x.get_name());
		}*/
		bool del_el(list2_node * x)
		{
			if (head == nullptr)
			{
				if (x == el)
				{
					el = nullptr;
					return true;
				}
				return false;
			}
			else 
			{
				list_node *curr = head;
				list_node *prev = nullptr;
				for(; curr; prev = curr, curr = curr->next_)
				{
					if (x == curr->el)
					{
						if (head == curr)
						{
							head = curr->next_;
							curr->next_ = nullptr;
							curr->el = nullptr;
							delete curr;
							// printf("well\n");
							break;
						}
						else 
						{
							prev->next_ = curr->next_;
							curr->el = nullptr;
							curr->next_ = nullptr;
							delete curr;
							break;
						}
					}
				}
				if (head == nullptr) return true;
				el = head->el;
				if (head ->next_ == nullptr)
				{
					head->el = nullptr;
					delete head; 
					head = nullptr;
				}
				return false;
			}
			return false;
		}

		list2_node *set_el(record& x, list2_node * curr)
		{
			if (el == nullptr) 
			{
				if (curr == nullptr)
				{
					curr = new list2_node;
					*(record*)curr = static_cast<record&&>(x);
				}
				el = curr;
				return curr;
			}
			else
			{
				if (head != nullptr)//there already was the list
				{
					list_node *miu = head;
					for (; miu != nullptr; miu = miu->next_)
					{
						if (equal_ph_g(x, *(miu->el))) return nullptr;//there is curr in struct
					}

					list_node * now = new list_node;
					if (curr == nullptr)
					{
						curr = new list2_node;
						*(record*)curr = static_cast<record&&>(x);
					}
					now->el = curr;
					now->next_ = head;
					head = now;
					return curr;
				}
				else
				{
					if (equal_ph_g(*el, x)) return nullptr;
					list_node * now = new list_node;
					now->el = el;;
					head = new list_node;
					if (curr == nullptr)
					{
						curr = new list2_node;
						*(record*)curr = static_cast<record&&>(x);
					}
					head->el = curr;
					head->next_ = now;
					return curr;
				}
			}
		}
		avl_tree_node& operator= (const avl_tree_node& x) = delete;
		avl_tree_node& operator= (avl_tree_node&& x)
		{
			if (el == x.el && head == x.head)
				return *this;
			el = x.el; head = x.head;
			erase_links ();
			x.erase_links ();
			return *this;
		}

		void print(FILE *fp = stdout)
		{
			if (balance == 1 || balance == 0) fprintf(fp, " ");
			fprintf(fp, "%d ", balance);
			if (head == nullptr) el->print(nullptr, fp);
			else
			{
				printf( "%ss:\n", el->get_name());
				list_node *curr = head;
				for(; curr; curr= curr->next_)
					(curr->el)->print(nullptr, fp);
			}

		}
	private:
		void erase_links ()
		{ left = nullptr; right = nullptr; balance = 0; }


	friend class avl_tree;
};


class avl_tree
{
	private:
		avl_tree_node * root = nullptr;
	public:
		avl_tree () = default;
		avl_tree (const avl_tree& x) = delete;
		avl_tree (avl_tree&& x)
		{
			root = x.root; x.root = nullptr;
		}
		~avl_tree ()
		{
			delete_subtree (root);
			root = nullptr;
		}

		void read (list2_node * curr)
		{ 
			list2_node *ret = nullptr;
			root = add_node(*curr, &ret, curr);
				// this->print(5);
				// printf("\n\n\n\n\n\n\n");
		}


		list2_node * read_record(record& x)
		{
			list2_node *ret = nullptr;
			root = add_node(x, &ret);
			return ret;
		}



		void print (int r = 10, FILE *fp = stdout)
		{
			print_subtree (root, 0, r, fp);
		}
	private:
		static void delete_subtree (avl_tree_node * curr)
		{
			if (curr == nullptr)
				return;
			delete_subtree (curr->left);
			delete_subtree (curr->right);
			delete curr;
		}


		avl_tree_node * add_node(record& x, list2_node **res, list2_node *curr = nullptr)
		{
			int delta = 0;
			if (root == nullptr)
			{
				avl_tree_node * y = new avl_tree_node;
				if (curr == nullptr)
				{
					curr = new list2_node;
					*(record*)curr = static_cast<record&&>(x);
				}
				y->el = curr;
				*res = curr;
				root = y;
				root->balance = 0;
				return root;
			}
			// printf("I wanted to add ");	
			// y->print();
			return add_avl_subtree(root, x, curr, delta, res);
		}

		static avl_tree_node* add_avl_subtree(avl_tree_node *curr, record& who, list2_node *x, int& delta0, list2_node** res)
		{
			int re = strcmp(who.get_name(), curr->el->get_name());
			if (re < 0)//<
			{
				if (curr->left == nullptr)
				{
					avl_tree_node * y = new avl_tree_node;
					*res = y->set_el(who, x);
					curr->left = y;
					curr->balance = curr->balance - 1;
					if (curr->balance == -1) delta0 = 1;
					else delta0 = 0; 
					return curr;
				}
				else
				{
					curr->left = add_avl_subtree(curr->left, who, x, delta0, res);
					if (delta0 == 0) return curr;
					curr->balance = curr->balance - delta0;
					if (curr->balance == -1) delta0 = 1;
					else
					{
						if (curr->balance == -2) curr = fix_left_subtree(curr, delta0);
						else delta0 = 0;
					} 
					return curr;

				}
			}
			else if (re > 0)
			{
				if (curr->right == nullptr)
				{
					avl_tree_node *y = new avl_tree_node;
					*res = y->set_el(who, x);
					curr->right = y;
					curr->balance = curr->balance + 1;
					if (curr->balance == 1) delta0 = 1;
					return curr;
				}
				else
				{
					curr->right = add_avl_subtree(curr->right, who, x, delta0, res);
					if (delta0 == 0) return curr;
					curr->balance = curr->balance + delta0;
					if (curr->balance == 1) delta0 = 1;
					else 
					{
						if (curr->balance == 2) curr = fix_right_subtree(curr, delta0);
						else delta0 = 0;
					}
					return curr;

				}
			}
			else
			{
				*res = curr->set_el(who, x);
				return curr;
			}
			return curr;
		}


		static avl_tree_node * fix_left_subtree(avl_tree_node *A, int& delta0)
		{
			//printf("FIX left\n");
			//A->balance == -2
			avl_tree_node *B = A->left;
			avl_tree_node *curr = nullptr;
			delta0 = 0;
			if (B->balance == 0) 
			{
				//printf("CASE 0!\n");
				delta0 = 1;
				A->left = B->right;
				B->right = A;

				A->balance = -1;
				B->balance = 1;
				return B;

			}
			if (B->balance == -1)
			{
				//printf("CASE 1!\n");
				B->balance = 0;
				A->balance = 0;
				A->left = B->right;
				B->right = A;
				return B;
			}
			else//1
			{
				//printf("CASE -1!\n");
				curr = B->right;//C
				if (curr->balance == 0)
				{
					A->balance = 0;
					B->balance = 0;
				}
				else if (curr->balance == 1)
				{
					A->balance = 0;
					B->balance = -1;
				}
				else //-1
				{
					A->balance = 1;
					B->balance = 0;
				}
				curr->balance = 0;
				B->right = curr->left;
				A->left= curr->right;
				curr->left = B;
				curr->right = A;
				return curr;
			}
			return curr;

		}


		static avl_tree_node * fix_right_subtree(avl_tree_node *A, int& delta0)
		{
			//printf("FIX right\n");
			//A->balance == 2
			avl_tree_node *B = A->right;
			avl_tree_node *curr = nullptr;
			delta0 = 0;
			if (B->balance == 0) 
			{
				//printf("CASE 0!\n");
				A->right = B->left;
				B->left = A;
				A->balance = 1;
				B->balance = -1;
				delta0 = 1;
				return B;

			}
			if (B->balance == 1)
			{
				//printf("CASE 1!\n");
				B->balance = 0;
				A->balance = 0;
				A->right = B->left;
				B->left = A;
				return B;
			}
			else//-1
			{
				//printf("CASE -1!\n");
				curr = B->left;//C
				if (curr->balance == 0)
				{
					A->balance = 0;
					B->balance = 0;
				}
				else if (curr->balance == -1)
				{
					A->balance = 0;
					B->balance = 1;
				}
				else//1
				{
					A->balance = -1;
					B->balance = 0;
				}
				curr->balance = 0;
				B->left = curr->right;
				A->right= curr->left;
				curr->right = B;
				curr->left = A;
				return curr;
			}
			return curr;
		}


		void delete_node(list2_node * curr)
		{
			int delta = 0;
			if (root == nullptr)
			{
				return;
			}
			root = delete_avl_subtree(root, curr, delta);
			return;
		}




		static avl_tree_node * delete_avl_subtree(avl_tree_node *curr, list2_node *x, int& delta0)
		{
			if (*x < *curr->el)//<
			{
				if (curr->left == nullptr)//not such el
				{
					printf("WF!?\n");
					return curr;
				}
				else
				{
					curr->left = delete_avl_subtree(curr->left, x, delta0);
					if (delta0 == 0) return curr;
					curr->balance = curr->balance + delta0;/// vetka in left subtree became less
					//curr->ballance could be 0 or 1 or 2
					if (curr->balance == 0) delta0 = 1;//-1 -> 0 so the lenth of the biggest vetka became shotter
					else
					{
						if (curr->balance == 2) curr = fix_right_subtree(curr, delta0);
						else delta0 = 0;// 0 -> 1
					} 
					return curr;

				}
			}
			else if (*x > *curr->el)
			{
				if (curr->right == nullptr)
				{
					printf("WF!?\n");
					return curr;
				}
				else
				{
					curr->right = delete_avl_subtree(curr->right, x, delta0);
					if (delta0 == 0) return curr;
					curr->balance = curr->balance - delta0;/// vetka in right subtree became less
					//curr->ballance could be -2 or -1 or 0
					if (curr->balance == 0) delta0 = 1;// 1 -> 0
					else 
					{
						if (curr->balance == -2) curr = fix_left_subtree(curr, delta0);// -1 -> -2
						else delta0 = 0;//0 -> -1
					}
					return curr;

				}
			}
			else
			{
				bool what = curr->del_el(x);
				if (what == false) return curr;//we should not delete the vershina
				printf("WE NEED TO DELETE IT\n");
				if (curr->left == nullptr)
				{
					avl_tree_node *ret = curr->right;
					delete curr;
					delta0 = 1;//in any case the lenth of the biggest vetka became shotter
					return ret;
				}
				if (curr->right == nullptr)
				{
					avl_tree_node *ret = curr->left;
					delete curr;
					delta0 = 1;//in any case the lenth of the biggest vetka became shotter
					return ret;
				}
				avl_tree_node *pupu = curr->left, *pupu2 = curr->right;
				int bal = curr->balance;
				delete curr;
				curr = nullptr;
				printf("well, deleted want to find the biggest in left side\n");
				pupu = delete_the_end(pupu, delta0, &curr);
				//pupu->print();
				// curr->print();
				// pupu2->print();
				curr->left = pupu;//we reterned the new root of left sabtree
				curr->right = pupu2;
				curr->balance = bal + delta0;

				if (delta0 == 0) return curr;
				if (curr->balance == 0) delta0 = 1;
				else
				{
					if (curr->balance == 2) curr = fix_right_subtree(curr, delta0);
					delta0 = 0;

				}
				return curr;
			}
			return curr;
		}


		


		static avl_tree_node * delete_the_end(avl_tree_node *curr, int& delta0, avl_tree_node ** where)
		{
			if (curr->right == nullptr)
			{
				*where = curr;
				printf("We got the biggest ");
				curr->print();
				delta0 = 1;
				curr = curr->left;
				return curr;
			}
			else
			{
				curr->right = delete_the_end(curr->right, delta0, where);
				if (delta0 == 0) return curr;
				curr->balance = curr->balance - delta0;/// vetka in right subtree became less
				//curr->ballance could be -2 or -1 or 0
				if (curr->balance == 0) delta0 = 1;// 1 -> 0
				else 
				{
					if (curr->balance == -2) curr = fix_left_subtree(curr, delta0);// -1 -> -2
					else delta0 = 0;//0 -> -1
				}
				return curr;
			}
			return curr;
		}
		

		static avl_tree_node * find(avl_tree_node *curr, const char * s)
		{
			if (curr == nullptr) return nullptr;
			int res = strcmp(s, curr->el->get_name());
			if (res > 0) return find(curr->right, s);
			if (res < 0) return find(curr->left, s);
			return curr; // names are equal
		}


		list2_node * find_in_tree(command * test)
		{
			list2_node *result = nullptr;
			const char *s = test->get_name();
			avl_tree_node *where = find(root, s);
			if (where == nullptr) return nullptr;
			if (where->head == nullptr)
			{
				if (test->apply_ph_and_gr(*(where->el)))//////////NEED TO CHANGE
				{
					result = where->el;
					result->set_next_select(nullptr);
				}
				return result;
			}
			else
			{
				list_node *curr = where->head;
				list2_node *end = nullptr;
				for(; curr; curr = curr->next_)
				{
					if (test->apply_ph_and_gr(*(curr->el)))///////////NEED TO CHANGE
					{
						if (result == nullptr)
						{
							result = curr->el;
							end = result;
						}
						else
						{
							end->set_next_select(curr->el);
							end = curr->el;
						}
					}
					
				}
				if (end != nullptr) end->set_next_select(nullptr);
				return result;
			}
		}

		
		static void print_subtree (avl_tree_node * curr, int level, int r, FILE *fp = stdout)
		{
			if (curr == nullptr || level > r)
				return;
			int spaces = level * 2;
			for (int i = 0; i < spaces; i++)
				printf (" ");
			curr->print (fp);
			print_subtree (curr->left, level + 1, r, fp);
			print_subtree (curr->right, level + 1, r, fp);
		}
		friend class list2;
};









# endif // avl_tree
