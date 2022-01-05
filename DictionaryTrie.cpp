#include "util.h"
#include "DictionaryTrie.h"
#include <vector>
#include <string>
#include <queue>
#include <functional>

using namespace std;

Node::Node() {
  isWord = false;
  word = string();
  frequency = 0;
  int i = 0;
  for(i = 0; i < 27; ++i) {
    children[i] = nullptr;
  }
}

unsigned int myComparator::operator() (Node* n1, Node* n2)
{
    return n1->frequency < n2->frequency;
}

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie(){
  root = new Node();
}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
 bool DictionaryTrie::insert(std::string word, unsigned int freq)
 {
   if(word == "") {
     return false;
   }
   bool isduplicate = false;
   if(find(word)==true) {
     isduplicate = true;
   }

       Node* curr = root;
  if(!root) {
   return false;
  }
   unsigned int i = 0;
   int index;
   for(i = 0; i < word.length(); ++i) {
     if(word[i] == 32) {
       index = 26;
     }
     else {
       index = word[i] - 'a';
     }
    if(index < 0 || index > 26) {
     return false;
    }

     if(!(curr->children[index])) {

       curr->children[index] = new Node(); // not found
     }
     curr = curr->children[index];
   }
   curr->isWord = true;

   if(isduplicate==true) {
     if(freq > curr->frequency) {
       curr->frequency = freq;
     }
     return false;
   }
   else {
     curr->word = word;
     curr->frequency = freq;
     return true;
   }
 }

 /* Return true if word is in the dictionary, and false otherwise */
 bool DictionaryTrie::find(std::string word) const
 {
  Node* curr = root;
  if(!root) {
   return false;
  }
  else {
   unsigned int i = 0;
   int index;
   for(i = 0; i < word.length(); ++i) {
     if(word[i] == 32) {
       index = 26;
     }
     else {
       index = word[i] - 'a';
     }
    if(index < 0 || index > 26) {
     return false;
    }
     if(!(curr->children[index])) {
       return false; // not found
     }
     curr = curr->children[index];
   }
   return (curr != NULL && curr->isWord);
  }
 }

/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
  std::vector<std::string> words;
  if(prefix == "") {
    return words;
  }
  // find starts here:
  Node* curr = root;
  if(!root) {
   cout << "Invalid Input. Please retry with correct input" << endl;
   return words;
  }
  else {
   unsigned int i = 0;
   int index;
   for(i = 0; i < prefix.length(); ++i) {
     if(prefix[i] == 32) {
       index = 26;
     }
     else {
       index = prefix[i] - 'a';
     }
    if(index < 0 || index > 26) {
      cout << "Invalid Input. Please retry with correct input" << endl;
      return words;
    }
     if(!(curr->children[index])) {
       cout << "Invalid Input. Please retry with correct input" << endl;
       return words; // not found
     }
     curr = curr->children[index];
   }
   // if the word is found, curr will != NULL and curr isWord is true
   if(curr != NULL) {
     auto foundWords = findWords(curr);
     Node* node;
     string wordToAdd;
     // check if num_completions is more than the number in priority queue
     int size = foundWords.size();
     int endLoop = 0;
     if(num_completions > size) {
       endLoop = size;
     }
     else {
       endLoop = num_completions;
     }
     for(i = 0; i < endLoop; ++i) { // FIXME
       // add words to vector
       node = foundWords.top();
       wordToAdd = node->word;
       words.push_back(wordToAdd);
       foundWords.pop();
     }
   }
  }
  return words;
}

/*
 * Return the most similar word of equal length to the query, based
 * on their Hamming distance.
 * In case of ties, return the word with the highest frequency (you may
 * assume there will always be one should a tie happen.)
 * In case there isn't any word of equal length to the query in the
 * trie, return an empty string.
 */
std::string DictionaryTrie::checkSpelling(std::string query)
{
  // if(!query) {
  //   return "";
  // }
  if(query == "") {
    return "";
  }
  if(find(query)) {
    return query;
  }
  else {
    /*
    1. Run BFS or DFS from the root
    2. Keep track of current best word and minimum Hamming distance
    3. At each node explored, compare Hamming distance with minimum
      Hamming distance and update values if necessary.
    */
    int minHamming;
    int currHamming;
    Node* correctNode;

    auto allWords = findWordsLength(query.length()); // queue of nodes
    // check if allWords has any words in it
    if(allWords.empty()) {
      return "";
    }
    minHamming = calcHamming(query, allWords.front()->word);
    correctNode = allWords.front();

    while(!allWords.empty()) {
      currHamming = calcHamming(query, allWords.front()->word);
      if(currHamming < minHamming){
        minHamming = currHamming;
        correctNode = allWords.front();
      }
      else if(currHamming == minHamming){
        if(allWords.front()->frequency > correctNode->frequency) {
          correctNode = allWords.front();
        }
      }
      allWords.pop();
    }
    return correctNode->word;
  }
  // return ""; // TODO: delete this
}

int DictionaryTrie::calcHamming(std::string w1, std::string w2) {
  int i = 0;
  int value = 0;
  int total = 0;
  for(i = 0; i < w1.length(); ++i) {
    value = w1[i] - w2[i];
    if(value < 0){
      value = value * -1;
    }
    total = total + value;
  }
  return total;
}

queue<Node*> DictionaryTrie::findWordsLength(int length) {
// void DictionaryTrie::findWordsLength(int length) {
  Node* curr = root;
  int i = 0;
  queue<Node*> q; // list of nodes
  queue<Node*> wordNodes; // nodes containing words
  q.push(curr);
  while(!q.empty()) {
    curr = q.front(); // just get's first in line; doesn't remove
    if(curr->isWord == true && curr->word.length() > length) {
      break;
    }
    if(curr->isWord == true && curr->word.length() == length) {
      wordNodes.push(curr); // enqueue
    }
    q.pop(); // actually removes word
    // for each child of curr; add it
    for(i = 0; i < 27; i++) {
      if((curr->children[i])) {
       q.push(curr->children[i]);
      }
    }
  }
  // int size = wordNodes.size();
  // for(i = 0; i < size; i++){
  //   cout << wordNodes.front()->word << endl;
  //   wordNodes.pop();
  // }
  return wordNodes;
}

// std::priority_queue<int, std::vector<int>, std::greater<int> > my_min_heap;
priority_queue<Node*, vector<Node*>, myComparator> DictionaryTrie::findWords(Node* start) {
  Node* curr = start;
  int i = 0;
  queue<Node*> q; // list of nodes
  priority_queue<Node*, vector<Node*>, myComparator> wordNodes; // nodes containing words
  q.push(curr);
  while(!q.empty()) {
    curr = q.front(); // just get's first in line; doesn't remove
    if(curr->isWord == true) {
      wordNodes.push(curr);
    }
    q.pop(); // actually removes word
    // for each child of curr; add it
    for(i = 0; i < 27; i++) {
      if((curr->children[i])) {
       q.push(curr->children[i]);
      }
    }
  }
  // int size = wordNodes.size();
  // for(i = 0; i < size; i++){
  //   cout << wordNodes.top()->word << endl;
  //   wordNodes.pop();
  // }
  return wordNodes;
}

void DictionaryTrie::deleteAll(Node* n)
{
  // for loop every pointer in child array in node
  // base case if n = NULL; delete(n) and return
  // if n is not NULL; recursively call line 1-> for loop every pointer in children
  if(n == NULL) {
   // delete(n); 
    return;
  }
  int i = 0;
  for(i = 0; i < 27; i ++) {
    if(n->children[i]) {
      deleteAll(n->children[i]);
    }
  }
  if(n!=NULL) { 
   delete(n);
  }

   
}
/*
if(n == NULL) {
  return;
}
deleteAll(n->left);
deleteAll(n->right);
delete(n);
*/

// method: printAllWords
void DictionaryTrie::printAll() {
// void DictionaryTrie::findWordsLength(int length) {
  Node* curr = root;
  if(curr == NULL) {
    return;
  }
  int i = 0;
  queue<Node*> q; // list of nodes
  queue<Node*> wordNodes; // nodes containing words
  q.push(curr);
  while(!q.empty()) {
    curr = q.front(); // just get's first in line; doesn't remove
    if(curr->isWord == true) {
      wordNodes.push(curr); // enqueue
    }
    q.pop(); // actually removes word
    // for each child of curr; add it
    for(i = 0; i < 27; i++) {
      if((curr->children[i])) {
       q.push(curr->children[i]);
      }
    }
  }
  int size = wordNodes.size();
  for(i = 0; i < size; i++){
    cout << wordNodes.front()->word << endl;
    wordNodes.pop();
  }
}


/* Destructor */
DictionaryTrie::~DictionaryTrie(){
  deleteAll(root);
}
