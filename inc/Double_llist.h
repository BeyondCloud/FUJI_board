#ifndef _DOUBLE_LLIST_H_
#define _DOUBLE_LLIST_H_
#include <iostream>
template <class T>
class Double_llist
{
    public:
        struct node_t
        {
            T obj;
            node_t* next;
            node_t* prev;
        };

<<<<<<< HEAD
        Double_llist(){head_ptr = NULL;tail_ptr = NULL;list_size=0;};
=======
        Double_llist():head_ptr(0){ list_size = 0;};
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
        void push_back(T const &obj);
        bool go_next();
        bool go_prev();
        bool go_tail();
        bool go_head();
        void remove(node_t* to_remove);
<<<<<<< HEAD

=======
        void remove_N_next();
        void remove_N_prev();
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
        const T& front();
        const T& back();
        T& cur();

        void set_cur(T &set_to);

        node_t* cur_ptr(){return current;};
<<<<<<< HEAD
        node_t* head(){if (!isEmpty())return head_ptr;};
        node_t* tail(){if (!isEmpty())return tail_ptr;};
        bool isEmpty();
        int size();
        node_t* first_touch;
        node_t* off_ptr;
=======
        node_t* head(){if (!isEmpty()){return head_ptr;}};
        node_t* tail(){if (!isEmpty()){return tail_ptr;}};
        bool isEmpty();
        int size();
        node_t* first_touch;
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
    private:
        int list_size;
        node_t* head_ptr;
        node_t* tail_ptr;
        node_t* current;
};
template <class T>
void Double_llist<T>::push_back(T const &obj)
{
    node_t *node = new node_t();
    node->obj = obj;
    if(!head_ptr)
    {
        head_ptr = node;
        tail_ptr = node;
<<<<<<< HEAD
        node->next = NULL;
=======
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
    }
    else
    {
        tail_ptr->prev = node;
        node->next = tail_ptr;
        tail_ptr = node;
    }
<<<<<<< HEAD
    node->prev = NULL;

=======
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
    list_size++;
}
template <class T>
void Double_llist<T>::set_cur(T &set_to)
{
    current->obj = set_to;
}
<<<<<<< HEAD

=======
template <class T>
void Double_llist<T>::remove_N_next()
{
    node_t* to_remove = current;
    if(current->next != 0)
        current = current->next;
    remove(to_remove);
}
template <class T>
void Double_llist<T>::remove_N_prev()
{
    node_t* to_remove = current;
    if(current->prev != 0)
        current = current->prev;
    remove(to_remove);
}
>>>>>>> e1fd690205cfc75d95dfb53506f84d9a4d2ad698
template <class T>
void Double_llist<T>::remove(node_t *to_remove)
{
    if(to_remove == head_ptr)
        head_ptr = head_ptr->prev;
    if(to_remove == tail_ptr)
        tail_ptr = tail_ptr->next;
    if(to_remove->next)
        to_remove->next->prev = to_remove->prev;
    if(to_remove->prev)
        to_remove->prev->next = to_remove->next;
    delete to_remove;
    list_size--;
}
template <class T>
const T& Double_llist<T>::front(){
    if ( tail_ptr == 0 )
    {
        std::cerr <<"access obj error: front(head) obj is NULL\n";
        return -1;
    }
    return head_ptr->obj;
}
template <class T>
const T& Double_llist<T>::back(){
    if ( tail_ptr == 0 )
    {
        std::cerr <<"access obj error: back(tail) obj is NULL\n";
        return -1;
    }
    return tail_ptr->obj;
}
template <class T>
T& Double_llist<T>::cur(){
    if ( current == 0 )
    {
        std::cerr <<"access obj error: current obj is NULL\n";
    }
    return current->obj;
}
template <class T>
bool Double_llist<T>::isEmpty(){
    return head_ptr == 0;
}
template <class T>
int Double_llist<T>::size(){
    return list_size;
}
template <class T>
bool Double_llist<T>::go_next(){
    if(current->next == 0)
        return false;
    current = current->next;
        return true;
}
template <class T>
bool Double_llist<T>::go_prev(){
    if(current->prev == 0)
        return false;
    current = current->prev;
        return true;
}
template <class T>
bool Double_llist<T>::go_tail(){
    if ( tail_ptr == 0 )
        return false;
    current = tail_ptr;
    return true;
}
template <class T>
bool Double_llist<T>::go_head(){
    if (head_ptr == 0 )
        return false;
    current = head_ptr;
    return true;
}
#endif // _DOUBLE_LLIST_H_
