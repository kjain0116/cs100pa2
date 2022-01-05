#include "util.h"
#include "DictionaryHashtable.h"
#include <unordered_set>
#include <string>

using namespace std;

/* Create a new Dictionary that uses a Hashset back end */
DictionaryHashtable::DictionaryHashtable(){
  unordered_set<string> dictionary;
}

/* Insert a word into the dictionary. */
bool DictionaryHashtable::insert(std::string word)
{
  pair<unordered_set<string>::iterator, bool> p;
 	p = dictionary.insert(word);
 	return p.second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryHashtable::find(std::string word) const
{
    unordered_set<string>::const_iterator it;
    it = dictionary.find(word);
 	if(it != dictionary.end()){
     return true;
    }
 	else{
     return false;
    }
}

/* Destructor */
DictionaryHashtable::~DictionaryHashtable(){}
