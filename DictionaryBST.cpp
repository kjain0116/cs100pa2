#include "util.h"
#include "DictionaryBST.h"
#include <set>
#include <string>

using namespace std;

/* Create a new Dictionary that uses a BST back end */
DictionaryBST::DictionaryBST(){
    set<string> dictionary;
}

/* Insert a word into the dictionary. */
bool DictionaryBST::insert(std::string word)
{
 	pair<set<string>::iterator, bool> p;   
 	p = dictionary.insert(word);
 	return p.second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryBST::find(std::string word) const
{
 	set<string>::iterator it;   
 	it = dictionary.find(word);
 	if(it != dictionary.end()){
     return true;
    }
 	else{
     return false;
    }
}

/* Destructor */
DictionaryBST::~DictionaryBST(){
    // delete dictionary;
}
