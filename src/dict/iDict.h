#ifndef _IDICT_H_
#define _IDICT_H_
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>

#include "type.h"

using namespace std;

class iIndexItem {
public:
    iIndexItem():index(NULL),inxlen(0),addr(0),opaque(NULL)
	{
	}
    ~iIndexItem()
    {
        if (index)
	        free(index);
        if (opaque)
		    free(opaque);
	}
	wchar_t *index;
	int inxlen;
    address_t addr;
	void *opaque;
};

typedef vector<class iIndexItem*> IndexList;

class iDictItem
{
public:
	iDictItem(std::string identi=""):phonetic(""),expl(""),dictIdentifier(identi),addr(0)
    {
    }
    ~iDictItem(){}

    std::string dictIdentifier;
	std::string phonetic; /* utf-8 bytes */
	std::string expl;     /* utf-8 bytes */
	address_t addr;
};

class iDict {
public:
    /* Load a dictionary */
    virtual bool load(const string& dictname) = 0;

    /* Lookup a word or phrase */
	virtual iDictItem lookup(const string& word) = 0;

    /* Including 'start' but not incluing 'end', start from 0 */
	virtual int getIndexList(IndexList & indexList, int start, int end) = 0;

    /* For click on index item */
	virtual iDictItem  onClick(int index, iIndexItem* item) = 0;

    /* Identify the unique dict class name */
    virtual std::string identifier() = 0;

    /* Check if a dictionary is supported by this dict class  */
    virtual bool support(const string& dictname) = 0;

    /* 'any' means the dictionary don't specify it */
    virtual void getLanguage(string& from, string& to) = 0;
};

#endif
