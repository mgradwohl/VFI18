#include "stdafx.h"
#include "MyList.h"

MyList::MyList()
{
}

MyList::~MyList()
{
	list.clear();
}

void MyList::RemoveAll()
{
	list.clear();
}

size_t MyList::GetCount()
{
	return list.size();
}

bool MyList::isEmpty()
{
	return list.empty();
}

spWiseFile MyList::RemoveHead()
{
	if (list.empty()) return nullptr;

	spWiseFile p = list.front();
	list.erase(list.begin());
	return p;
}

spWiseFile MyList::GetItemAt(size_t i)
{
	if (list.empty()) return nullptr;
	return list[i];
}

bool MyList::Remove(spWiseFile p)
{
	MyListIterator i = find(list.begin(), list.end(), p);
	if (i != list.end())
	{
		list.erase(i);
		return true;
	}

	return false;
}

void MyList::AddTail(spWiseFile p)
{
	list.push_back(p);
}

void MyList::AddHead(spWiseFile p)
{
	list.insert(list.begin(), p);
}

void MyList::Grow(size_t i)
{
	const size_t c = list.size();
	list.resize(i + c);
}
