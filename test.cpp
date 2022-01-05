/**
 *  CSE 100 PA2 C++ Auto-Complete
 *  Authors: Gina Ratto, Shruti Pandey, Carlos Mattoso
 *  Description: Testing file to see if our
 *                dictionaries work correctly
 */

#include<iostream>
#include<string>
#include<algorithm>
#include<set>
#include<cstdlib>
#include<cassert>
#include "util.h"
#include "DictionaryTrie.h"
#include "DictionaryBST.h"
#include "DictionaryHashtable.h"
#include "SuffixArray.h"
#define LETTERS 26
using namespace std;


int main()
{

    cout << endl << "Testing SPELL CHECKER..." << endl;
    cout <<         "========================" << endl;

    DictionaryTrie scTrie;
    scTrie.insert("stop"    , 9001);

 
 
  // scTrie.deleteAll(scTrie.root); 
 
// scTrie.printAll(); 




	  
    return 0;
}
