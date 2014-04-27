
#include "DictManager.h"
#include "Application.h"
#include "Configure.h"
#include "aldict/Aldict.h"
#include "alphadict.h"
#include "MessageQueue.h"

#include "Util.h"

LookupTask::LookupTask(const string& input, int id, DictManager* dmgr)
:Task(0, false)
{
    m_dmgr = dmgr;
    m_dict = dmgr->m_dictOpen[id].dict;
    m_id = id;
    m_input = input;
}
 
void LookupTask::doWork()
{
    iDictItem item = m_dict->lookup(m_input);
    item.dictIdentifier = m_dict->identifier();
    if (!isAbort())
        m_dmgr->onAddLookupResult(m_input, m_id, item);
}

void LookupTask::abort()
{
    Task::abort();
    m_id = -1;
}

void LoadDictTask::doWork()
{
    DictManager::getReference().loadDict();
    g_uiMessageQ.push(MSG_UPDATE_INDEXLIST, 0);
}

DictManager& DictManager::getReference()
{
    static DictManager mgr;
    return mgr;
}

DictManager::DictManager()
:m_input(""),m_dictTotal(0),m_indexListStart(-1),m_bReload(false)
{   
    for (int i=0; i<DICT_MAX_OPEN; i++) {
        m_dictOpen[i].task = NULL;
        m_dictOpen[i].dict = NULL;
        m_dictOpen[i].id = -1;
    }

    m_indexList = new IndexList();
}

DictManager::~DictManager()
{
}

void DictManager::initialization()
{
    Configure *config = g_application.m_configure;

    // Check new dictionary
    vector<struct DictNode>& nodeVec = config->m_dictNodes;
    for (int i=0; i<nodeVec.size(); i++) {
        if (nodeVec[i].open == "") {
            nodeVec[i].open = "none";
            iDict *dict;
            if ((dict = createHandleByDict(nodeVec[i].path)) != NULL) {
                nodeVec[i].open = dict->identifier();
                dict->getLanguage(nodeVec[i].srclan, nodeVec[i].detlan);
            }
            config->writeDictItem(i);
        }        
    }
    TaskManager::getInstance()->addTask(new LoadDictTask(), 0);
}

/* It takes lots of memory for loading a dictionay, actually it depends on 
 * the size of dictionay. Usually, we load only one, but allow user to enable 
 * more dictionaies.
 *
 * when change language comboxs, should loadDict again.
 */
void DictManager::loadDict(bool more)
{
    MutexLock lock(m_cs);
    vector<struct DictNode>& nodeVec = g_application.m_configure->m_dictNodes;
    vector<struct DictNode>::iterator iter;

    // drop 'more' dictionary
    for (int i=1; i<DICT_MAX_OPEN && m_dictOpen[i].dict!=NULL; i++) {
        delete m_dictOpen[i].dict;
        m_dictOpen[i].dict = NULL;
    }
    
    if (m_dictOpen[0].dict != NULL) {
        int dictId = m_dictOpen[0].id;
        if (match(nodeVec[dictId].srclan, nodeVec[dictId].detlan)) {
            m_dictTotal = 1;
            return;
        }
        delete m_dictOpen[0].dict;
        m_dictOpen[0].dict = NULL;
    }
    m_dictTotal = 0;
    for (iter=nodeVec.begin(); iter<nodeVec.end(); ++iter) {
        if (match(iter->srclan, iter->detlan)) {
            iDict *dict = createHandleByIdenitfier(iter->open);
            if (dict) {
                dict->load(iter->path);
                m_dictOpen[m_dictTotal++].dict = dict;
            }
            return;
        }
    }
}

void DictManager::setSrcLan(const string& lan)
{    
    if (g_application.m_configure->m_srcLan != lan) {
        g_application.m_configure->m_srcLan = lan;
        m_bReload = true;
    }
}

void DictManager::setDetLan(const string& lan)
{
    if (g_application.m_configure->m_detLan != lan) {
        g_application.m_configure->m_detLan = lan;
        m_bReload = true;
    }
}

void DictManager::lookup(const string& input)
{
    MutexLock lock(m_cs);

    if (m_bReload){
        m_bReload = false;
        loadDict();
        m_indexListStart = -1;        
        g_uiMessageQ.push(MSG_UPDATE_INDEXLIST, 0);
    }

    m_input = input;
    for (int i=0; i<m_dictTotal; i++) {
        if (m_dictOpen[i].task != NULL)
            m_dictOpen[i].task->abort();
         
        m_dictOpen[i].task = new LookupTask(input, i, this);
        TaskManager::getInstance()->addTask(m_dictOpen[i].task, 0);
    }
}

void DictManager::onAddLookupResult(string& input, int which, iDictItem& item)
{
    MutexLock lock(m_cs);

    if (which != -1 && input == m_input) {
	    m_dictOpen[which].task = NULL;
		iDictItem* arg1 = new iDictItem();
		*arg1 = item;
		g_uiMessageQ.push(MSG_SET_DICTITEM, -1, (void *)arg1); /* UI should delete arg1 */
        printf("{onAddLookupResult} %u\n", Util::getTimeMS());
    } else {
        // be cancelled.
    }
}

int DictManager::getIndexList(IndexList& indexList, int start, int end)
{
    iDict* dict = m_dictOpen[0].dict;
    if (!dict)
        return 0;
    return dict->getIndexList(indexList, start, end);
}

void DictManager::onClick(int index, iIndexItem* item)
{
    iDictItem* arg1 = new iDictItem();
    *arg1 = m_dictOpen[0].dict->onClick(index, item);
    g_uiMessageQ.push(MSG_SET_DICTITEM, -1, (void *)arg1);    
}

/* Check if this dictionary supports accroding lanauage */
bool DictManager::match(const string& srcLan, const string& detLan)
{
    Configure *config = g_application.m_configure;

    if (srcLan == "any") {
        if (detLan == "any")
            return true;
        if (detLan == config->m_detLan)
            return true;
        return false;
    } else {
        if (srcLan != config->m_srcLan)
            return false;
        if (detLan == "any")
            return true;
        if (detLan == config->m_detLan)
            return true;
        return false;
    }
}

iDict* DictManager::createHandleByDict(const string dictpath)
{
    iDict *aldict = new Aldict();
    if (aldict->support(dictpath))
        return aldict;
    delete aldict;
    return NULL;
}

iDict* DictManager::createHandleByIdenitfier(const string identi)
{
    iDict *aldict = new Aldict();
    if (aldict->identifier() == identi)
        return aldict;
    delete aldict;
    return NULL;
}
