#ifndef _DICTMANAGER_H_
#define _DICTMANAGER_H_

#define DICT_MAX_OPEN 3

#include "iDict.h"
#include "TaskManager.h"
#include "MutexLock.h"

#include <map>

using namespace std;

class DictManager;

class LookupTask: public Task
{
public:
      LookupTask(const string& input, int id, DictManager* dmgr);
      ~LookupTask() {}
      virtual void doWork();
      virtual void abort();
private:
      iDict* m_dict;
      DictManager* m_dmgr;
      int m_id;
      string m_input;
};

class LoadDictTask: public Task
{
public:
      LoadDictTask():Task(0, false) {};
      ~LoadDictTask() {}
      virtual void doWork();
};

class DictManager
{
friend class LookupTask;
friend class LoadDictTask;

public:
    static DictManager& getReference();
    DictManager();
    ~DictManager();
    void initialization();
	void lookup(const string& input);
    int getIndexList(IndexList& indexList, int start, int end);
	void onClick(int index, iIndexItem* item);

    void setSrcLan(const string& lan);
    void setDetLan(const string& lan);
    
private:
    bool match(const string& srcLan, const string& detLan);
    void onAddLookupResult(string& input, int which, iDictItem& item);
	void loadDict(bool more=false);
    iDict* createHandleByDict(const string dictpath);
    iDict* createHandleByIdenitfier(const string identi);

    struct DictOpen{
        iDict *dict;
        Task  *task;
        int   id;
    };
    
    DictOpen m_dictOpen[DICT_MAX_OPEN];
    bool m_bReload;
    int m_dictTotal;
    string m_input;
    int m_indexListStart;
    IndexList  *m_indexList;

    MutexCriticalSection m_cs;
};

#endif
