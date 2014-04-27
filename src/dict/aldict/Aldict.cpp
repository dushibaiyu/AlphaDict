#include "Aldict.h"

Aldict::Aldict(const string& dictname)
{
	m_doc.loadDict(dictname);
}

bool Aldict::load(const string& dictname)
{
    return m_doc.loadDict(dictname);
}

iDictItem Aldict::lookup(const string& word)
{
	iDictItem i;
	struct aldict_dataitem d;
    address_t loc = m_doc.lookup(word, &d);
    i.addr = loc;
    printf("Aldict:lookup (%s), %u\n", word.c_str(), loc);
    if (loc != ALD_INVALID_ADDR) {
        if (d.len_phon > 0) {
            i.phonetic = string((const char*)d.ptr_phon, (int)d.len_phon);
            free(d.ptr_phon);
        }
        if (d.len_expl > 0) {
            i.expl = string((const char*)d.ptr_expl, (int)d.len_expl);
            printf("Aldict:lookup result: len_word: (%d,%s)\n",
                   d.len_expl, d.ptr_expl);
        
            free(d.ptr_expl);
        }
	    free(d.ptr_word);
    }
	return i;
}

string Aldict::identifier()
{
    return "aldict";
}

int Aldict::getIndexList(IndexList& indexList, int start, int end)
{
    return m_doc.getIndexList(indexList, start, end);
}

iDictItem Aldict::onClick(int index, iIndexItem* item)
{
    struct aldict_dataitem d = m_doc.dataitem(item->addr);
    printf("Aldict:onClick (%d, %u)\n", index, item->addr);
    iDictItem i("AlphaDict");
    i.addr = item->addr;
    if (d.len_phon > 0) {
        i.phonetic = string((const char*)d.ptr_phon, (int)d.len_phon);
        free(d.ptr_phon);
    }
    if (d.len_expl > 0) {
        i.expl = string((const char*)d.ptr_expl, (int)d.len_expl);
        printf("Aldict:lookup result: len_word: (%d,%s)\n",
               d.len_expl, d.ptr_expl);
        free(d.ptr_expl);
    }

	free(d.ptr_word);
    return i;
}

bool Aldict::support(const string& dictname)
{
    return m_doc.support(dictname);
}

void Aldict::getLanguage(string& from, string& to)
{
    string s((char *)(m_doc.m_header.src_lan));
    string d((char *)(m_doc.m_header.det_lan));
    
    from  = s;
    to = d;
    printf("{canLookup} (%s, %s)\n", s.c_str(), d.c_str());    
}
