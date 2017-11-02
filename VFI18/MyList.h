#pragma once

#include "WiseFile.h"
#include <vector>
#include <memory>

typedef std::shared_ptr<CWiseFile> spWiseFile;
typedef std::vector<spWiseFile> MyVector;
typedef MyVector::iterator MyListIterator;

class MyList
{
private:
	MyVector list;

public:
	MyList();
	~MyList();

	void RemoveAll();
	size_t GetCount();
	bool isEmpty();
	spWiseFile RemoveHead();
	spWiseFile GetItemAt(size_t i);
	bool Remove(spWiseFile p);
	void AddTail(spWiseFile p);
	void AddHead(spWiseFile p);
	void Grow(size_t i);
};

