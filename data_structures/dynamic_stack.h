#pragma once
#include <algorithm>
#include <assert.h>
#include <stdexcept>
constexpr int DYNAMIC_STACK_INITIAL_SIZE = 1;

template<class T>
class dynamic_stack {
private:
	T* stack;
	size_t reserved_size;
	size_t item_count;
	long top_idx; //top idx is signed, if top idx is < 0, then stack is empty or corrupted

	/// <summary>
	/// How much to resize stack on resize, 2 -> reserved space will be doubled each resize.
	/// Result size is always floored, setting resize_factor < 1 will decrease size in resize function
	/// </summary>
	float resize_factor = 2;
	void resize();
	void resize(const float& resize_factor);

public:
	dynamic_stack() : stack(new T[DYNAMIC_STACK_INITIAL_SIZE]), reserved_size(DYNAMIC_STACK_INITIAL_SIZE), top_idx(-1), item_count(0) {} //no arg ctor
	dynamic_stack(const size_t& initial_size) : stack(new T[initial_size]), reserved_size(initial_size), top_idx(-1), item_count(0) {} //initial size ctor
	~dynamic_stack(); //destructor

	bool is_empty();

	void add_top(const T& item);
	void add_at_index(const size_t& idx, const T& value);
	T pop_top();
	T& top();

	//subscript operators -> gets items from stack, needed to print content of the stack
	T& operator[](size_t idx);
	const T& operator[] (size_t idx) const;

	size_t size();


	//add more functionalities if needed
};

template<class T>
inline void dynamic_stack<T>::resize()
{
	if(resize_factor <=0 )
		throw std::runtime_error("dynamic_stack::resize() -> cannot resize, resize factor must be greater than 0!");

	T* copy = new T[item_count];
	std::copy(stack, stack + item_count, copy);

	if (reserved_size * resize_factor < item_count)
		throw std::runtime_error("dynamic_stack::resize() -> cannot resize, size after is less than item_count");

	reserved_size *= resize_factor; //floor is applied automatically
	delete[] stack; //remove resources from the array

	stack = new T[reserved_size];
	std::copy(copy, copy + item_count, stack);

	delete[] copy; //free memory after copy
}

template<class T>
inline void dynamic_stack<T>::resize(const float& resize_factor)
{
	if (resize_factor <= 0)
		throw std::runtime_error("dynamic_stack::resize() -> resize_factor in dynamic stack must be greater than zero!");

	//copy the resize factor 
	float r_factor_copy = this.resize_factor;
	this->resize_factor = resize_factor;

	resize(); //resize with new resize factor
	this->resize_factor = r_factor_copy; //restore old resize_factor
}

template<class T>
inline dynamic_stack<T>::~dynamic_stack()
{
	delete[] stack;
}

template<class T>
inline bool dynamic_stack<T>::is_empty()
{
	return item_count <= 0;
}

template<class T>
inline void dynamic_stack<T>::add_top(const T& item)
{
	if (item_count >= reserved_size)
		resize();

	stack[item_count++] = item;
	top_idx++;
}

template<class T>
inline void dynamic_stack<T>::add_at_index(const size_t& idx, const T& value)
{
	if (idx > item_count)
		throw std::out_of_range("dynamic_stack::add_at_index() -> index is out of range");

	if (item_count >= reserved_size)
		resize();

	std::move(stack + idx, stack + item_count, stack + idx + 1); //move all elements right of the added item 1 position to the right

	stack[idx] = value;

	item_count++;
	top_idx++;
}

template<class T>
inline T dynamic_stack<T>::pop_top()
{
	if (item_count<=0)
		throw std::out_of_range("dynamic_stack::pop_top() -> stack is empty");

	//do not remove anything, item will be removed by overwriting it
	T item = stack[top_idx];
	item_count--;
	top_idx--;

	return item;
}

template<class T>
inline T& dynamic_stack<T>::top()
{
	if (is_empty())
		throw std::out_of_range("dynamic_stack::top() -> stack is empty");

	return stack[top_idx];
}

template<class T>
inline T& dynamic_stack<T>::operator[](size_t idx)
{
	if (idx > top_idx)
		throw std::out_of_range("dynamic_stack::operator[] -> index is out of range!");

	return stack[idx];
}

template<class T>
inline const T& dynamic_stack<T>::operator[](size_t idx) const
{
	return *this[idx];
}

template<class T>
inline size_t dynamic_stack<T>::size()
{
	return item_count;
}
