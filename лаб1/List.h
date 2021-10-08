#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <sstream>
#include <exception>

template <typename T>
class List
{
	class Node
	{
	public:
		T* data;
		Node* next{ nullptr };
		Node() : data(), next{ nullptr }{};
		Node(T* _data) : data(_data), next(nullptr) {}
		Node(T* _data, Node* _next) : data(_data), next(_next) {};
		~Node() {}// delete data; delete next;
	};
	Node* _head = nullptr;
	size_t _size = 0;
	
public:
	bool isEmpty();
	size_t getSize();
	void pushBack(T* data);
	Node* getLast();
	void pushFront(T* data);
	void popFront();
	void popBack();
	void deleteNode(T* data);
	T* getObjByIndex(int idx);
	void deserialize(std::string str);
	std::string serialize();
	void deleteAt(int idx);
	~List()
	{
		std::cout << "DElete List" << std::endl;
		Node* tmp = _head;
		while (tmp)
		{
			popBack(tmp);
		}
		delete _head;
	}
};

template <typename T>
bool List<T>::isEmpty()
{
	return !(0 < _size);
}

template <typename T>
size_t List<T>::getSize()
{
	return _size;
}

template <typename T>
void List<T>::pushBack(T* data)
{
	Node* last = getLast();
	Node* tmp = new Node(data);
	if (last)
	{
		last->next = tmp;
	}
	else
	{
		_head = tmp;
	}
	_size++;
}

template <typename T>
typename List<T>::Node* List<T>::getLast()
{
	if (!_head)
	{
		return nullptr;
	}
	Node* tmp = _head;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	return tmp;
}

template <typename T>
void List<T>::pushFront(T* data)
{
	Node* tmp = new Node(data, _head);
	_head = tmp;
	_size++;
	delete tmp;
}

template <typename T>
void List<T>::popFront()
{
	if (isEmpty())
	{
		return;
	}
	Node* tmp = _head;
	_head = tmp->next;
	_size--;
	delete tmp;
}

template <typename T>
void List<T>::popBack()
{
	if (isEmpty())
	{
		return;
	}
	Node* last = getLast();
	if (last == _head)
	{
		popFront();
	}
	Node* tmp = _head;
	while (tmp->next != last)
	{
		tmp = tmp->next;
	}
	tmp->next = nullptr;
	_size--;
	delete last;
}

template <typename T>
void List<T>::deleteNode(T* data)
{
	if (isEmpty())
	{
		return;
	}
	else if (_head->data == data)
	{
		popFront();
		return;
	}
	else if (getLast()->data == data)
	{
		popBack();
		return;
	}
	else
	{
		Node* slow = _head;
		Node* fast = _head->next;
		while (fast && fast->data != data) {
			fast = fast->next;
			slow = slow->next;
		}
		if (!fast) {
			return;
		}
		slow->next = fast->next;
		_size--;
		delete fast;
	}

}

template <typename T>
T* List<T>::getObjByIndex(int idx)
{
	Node* tmp = _head;
	int i = 0;
	if (idx < 0 || idx >(int)_size)
	{
		return nullptr;
	}
	while (tmp && i != idx)
	{
		tmp = tmp->next;
		i++;
	}
	if (!tmp)
	{
		return nullptr;
	}
	return tmp->data;
}

template <typename T>
void List<T>::deserialize(std::string str)
{
	while (_head)
	{
		popFront();
	}
	std::stringstream ss(str);
	std::string _data;
	while (std::getline(ss, _data, ';'))
	{
		if (_data.find("|") == std::string::npos) { continue; }
		T* newRecord = new T();
		char* c = &_data[0];
		newRecord->Deserialize(c);
		pushBack(newRecord);
		//delete newRecord;
	}
}

template <typename T>
std::string List<T>::serialize()
{
	std::stringstream ss;
	if (_size > 0)
	{
		Node* tmp = _head;
		while (tmp)
		{
			ss << tmp->data->Serialize() << ";";
			tmp = tmp->next;
		}
	}
	return ss.str();
}

template <typename T>
void List<T>::deleteAt(int idx)
{
	T* toDel = getObjByIndex(idx);
	deleteNode(toDel);
}

