#pragma once
#include <iostream>
#include <algorithm>
constexpr int DYNAMIC_ARR_INITIAL_SIZE = 1;

template<class T>
class dynamic_array {
private:
	T* arr;
	size_t reserved_size;
	size_t item_count;

	/// <summary>
	/// How much to resize array on resize, 2 -> reserved space will be doubled each resize.
	/// Result size is always floored, setting resize_factor < 1 will decrease size in resize function, which sometimes can be usefull
	/// </summary>
	float resize_factor = 2;
	void resize();
	void resize(const float& resize_factor);
	void resize_to_fit(const int& fit); //resizes array, so it can fit given size, and then resizes to reserve size
	void resize_to_fit(const int& fit, const float& resize_factor); //resizes array, so it can fit given size, and then resizes to reserve size using given resize_factor

public:
	dynamic_array(): arr(new T[DYNAMIC_ARR_INITIAL_SIZE]), reserved_size(DYNAMIC_ARR_INITIAL_SIZE), item_count(0) {} //no arg ctor
	dynamic_array(const size_t& initial_size): arr(new T[initial_size]), reserved_size(initial_size), item_count(0){} //initial size ctor
	~dynamic_array(); //destructor

	//copy operators
	dynamic_array(const dynamic_array& other);
	dynamic_array<T>& operator=(const dynamic_array& other);

	//subscript operators -> gets items from our array
	T& operator[](size_t idx);
	const T& operator[] (size_t idx) const;

	void add_back(T item);
	void add_front(T item);
	void remove(const size_t& index);
	void remove_back();
	void remove_front();

	void reverse(); //reverses the array
	void append(const dynamic_array<T>& other);
	void append_front(const dynamic_array<T>& other);

	T& back() const; //gets item from the back
	T& front() const; //gets item from the front

	size_t size() const;
	bool is_empty() const;

	//add more functionalities if needed
};

template<class T>
inline void dynamic_array<T>::resize()
{
	if (resize_factor <= 0)
		throw std::runtime_error("Resize factor must be greater than zero!");

	if (reserved_size * resize_factor < item_count)
		throw std::runtime_error("dynamic_array::resize() -> cannot resize, size after is less than item_count");

	T* copy = new T[item_count];
	std::copy(arr, arr + item_count, copy);

	reserved_size *= resize_factor; //floor is applied automatically
	delete[] arr; //remove resources from the array

	arr = new T[reserved_size];
	std::copy(copy, copy + item_count, arr);

	delete[] copy; //free memory after copy
}

template<class T>
inline void dynamic_array<T>::resize(const float& resize_factor)
{
	if (resize_factor <= 0)
		throw std::runtime_error("dynamic_array::resize() -> resize_factor in dynamic array must be greater than zero!");

	//copy the resize factor 
	float r_factor_copy = this.resize_factor;
	this->resize_factor = resize_factor;

	resize(); //resize with new resize factor
	this->resize_factor = r_factor_copy; //restore old resize_factor
}

template<class T>
inline void dynamic_array<T>::resize_to_fit(const int& fit)
{
	if (fit <= 0)
		throw std::runtime_error("dynamic_array::resize_to_fit -> fit must be greater than zero!");

	if(fit * resize_factor < item_count)
		throw std::runtime_error("dynamic_array::resize_to_fit() -> cannot resize, size after is less than item_count");

	T* copy = new T[item_count];
	std::copy(arr, arr + item_count, copy);

	reserved_size = fit * resize_factor;
	delete[] arr;

	arr = new T[reserved_size];
	std::copy(copy, copy + item_count, arr);

	delete[] copy; //free memory after copy
}

template<class T>
inline void dynamic_array<T>::resize_to_fit(const int& fit, const float& resize_factor)
{
	if (resize_factor <= 0)
		throw std::runtime_error("dynamic_array::resize_to_fit() -> resize_factor in dynamic array must be greater than zero!");

	//copy the resize factor 
	float r_factor_copy = this.resize_factor;
	this->resize_factor = resize_factor;

	resize_to_fit(fit); //resize with new resize factor
	this->resize_factor = r_factor_copy; //restore old resize_factor
}


template<class T>
inline dynamic_array<T>::~dynamic_array()
{
	delete[] arr;
}

template<class T>
inline dynamic_array<T>::dynamic_array(const dynamic_array& other): item_count(other.item_count), reserved_size(other.reserved_size), arr(new T[other.reserved_size])
{
	std::copy(other.arr, other.arr + other.item_count, arr); //copy elements from other array to our array
}

template<class T>
inline dynamic_array<T>& dynamic_array<T>::operator=(const dynamic_array<T>& other)
{
	if (this == &other)
		return *this; //prevent self-assignment

	delete[] arr;
	item_count = other.item_count;
	reserved_size = other.reserved_size;
	arr = new T[reserved_size];

	std::copy(other.arr, other.arr + other.item_count, arr); //copy elements from other array to our array
	return *this;
}

template<class T>
inline void dynamic_array<T>::add_back(T item)
{
	if (item_count >= reserved_size)
		resize();

	arr[item_count++] = item;
}

template<class T>
inline void dynamic_array<T>::add_front(T item)
{
	if (item_count >= reserved_size)
		resize();

	//shift all elements 1 position to right
	std::move(arr, arr + item_count, arr + 1);

	arr[0] = item;
	item_count++;
}

template<class T>
inline void dynamic_array<T>::remove(const size_t& index)
{
	if (index >= item_count)
		throw std::out_of_range("dynamic_array::remove() -> index is out of range");

	std::move(arr + index + 1, arr + item_count, arr + index); //move all elements right of the removed item 1 position to the left
	item_count--;
}

template<class T>
inline void dynamic_array<T>::remove_back()
{
	if (item_count <= 0)
		throw std::out_of_range("dynamic_array::remove_back() -> array is empty");

	item_count--;
}

template<class T>
inline void dynamic_array<T>::remove_front()
{
	if(this->item_count <= 0)
		throw std::out_of_range("dynamic_array::remove_front() -> array is empty");

	this->remove(0);
}

template<class T>
inline void dynamic_array<T>::reverse()
{
	std::reverse(arr, arr + item_count);
}

template<class T>
inline void dynamic_array<T>::append(const dynamic_array<T>& other)
{
	if (item_count + other.item_count >= reserved_size)
		resize_to_fit(item_count + other.item_count); //we use resize to fit, cause usual resize might resize too little memory
	std::copy(other.arr, other.arr + other.item_count, arr + item_count); //append at the end
	item_count += other.item_count;
}

template<class T>
inline void dynamic_array<T>::append_front(const dynamic_array<T>& other)
{
	if (item_count + other.item_count >= reserved_size)
		resize_to_fit(item_count + other.item_count); //we use resize to fit, cause usual resize might resize too little memory

	std::move(arr, arr + item_count, arr + other.item_count); //make space for everything
	std::copy(other.arr, other.arr + other.item_count, arr); //append at the beginning
	item_count += other.item_count;
}

template<class T>
inline T& dynamic_array<T>::back() const
{
	if (item_count <= 0)
		throw std::out_of_range("dynamic_array::back() -> array is empty");

	return arr[item_count - 1];
}

template<class T>
inline T& dynamic_array<T>::front() const
{
	if (item_count <= 0)
		throw std::out_of_range("dynamic_array::back() -> array is empty");

	return arr[0];
}

template<class T>
inline size_t dynamic_array<T>::size() const
{
	return item_count;
}

template<class T>
inline bool dynamic_array<T>::is_empty() const
{
	return item_count<=0;
}

template<class T>
inline T& dynamic_array<T>::operator[](size_t idx)
{
	if (idx >= item_count)
		throw std::out_of_range("dynamic_array::operator[] -> index is out of range!");

	return arr[idx];
}

template<class T>
inline const T& dynamic_array<T>::operator[] (size_t idx) const
{
	if (idx >= item_count)
		throw std::out_of_range("dynamic_array::operator[] -> index is out of range!");

	return arr[idx];
}
