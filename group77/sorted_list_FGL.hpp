#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

#include <mutex>
#include <iostream>

/* struct for list nodes */
template<typename T>
struct node {
	std::mutex lock;
	T value;
	node<T>* next;
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {
	node<T>* first = nullptr;

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list() = default;
		sorted_list(const sorted_list<T>& other) = default;
		sorted_list(sorted_list<T>&& other) = default;
		sorted_list<T>& operator=(const sorted_list<T>& other) = default;
		sorted_list<T>& operator=(sorted_list<T>&& other) = default;
		~sorted_list() {
			while(first != nullptr) {
				remove(first->value);
			}
		}
		/* insert v into the list */
		void insert(T v) {
			/* construct new node */
			node<T>* current = new node<T>();
			current->value = v;

			/* first find position */
			node<T>* pred = nullptr;
			if (first != nullptr) {first->lock.lock();}
			node<T>* succ = first;
			if (succ != nullptr && succ->next != nullptr) {
				pred = succ;
				succ = succ-> next;
				succ->lock.lock();
			}
			while(succ != nullptr && succ->value < v) {
				if (pred != nullptr) {pred->lock.unlock();}
				pred = succ;
				succ = succ->next;
				if (succ != nullptr){succ->lock.lock();}
			}
			
			/* insert new node between pred and succ */
			current->next = succ;
			if(pred == nullptr) {
				first = current;
			} else {
				pred->next = current;
			}
			if (pred != nullptr) {pred->lock.unlock();}
			if (succ != nullptr){succ->lock.unlock();}
		}

		void remove(T v) {
			/* first find position */
			node<T>* pred = nullptr;
			if (first != nullptr) {first->lock.lock();}
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				if (pred != nullptr) {pred->lock.unlock();}
				pred = current;
				current = current->next;
				if (current != nullptr){current->lock.lock();}
			}
			if(current == nullptr || current->value != v) {
				/* v not found */
				if (pred != nullptr) {pred->lock.unlock();}
				if (current != nullptr){current->lock.unlock();}
				return;
			}
			/* remove current */
			if(pred == nullptr) {
				first = current->next;
			} else {
				pred->next = current->next;
			}
			if (pred != nullptr) {pred->lock.unlock();}
			if (current != nullptr){current->lock.unlock();}
			delete current;
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;
			/* first go to value v */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current = current->next;
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;
				current = current->next;
			}
			return cnt;
		}
};

#endif // lacpp_sorted_list_hpp