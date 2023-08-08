#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "ArgumentManager.h"
using namespace std;

//queue to hold messages
struct Node{
  string data;
  Node *next;
};

class regularQueue {
private:
  Node* front;
  Node* rear;

public:
  regularQueue(){
    front = nullptr;
    rear = nullptr;
  }
  bool isEmpty(){
    return front == nullptr;
  }
  void enqueue(string _data){
    Node* temp = new Node;
    temp->data = _data;
    temp->next = nullptr;
    if(isEmpty()){
      front = temp;
      rear = temp;
    }
    else{
      rear->next = temp;
      rear = temp;
    }
  }
  void dequeue(){
    if(!isEmpty()){
      Node* temp = front;
      front = front->next;
      delete temp;
    }
  }
  Node* getFront(){
    if(!isEmpty())
      return front;
    return nullptr;
  }
  void print(){
    Node *cu = front;
    while(cu != nullptr){
      cout << cu->data << endl;
      cu = cu->next;
    }
  }
};

//priority queue to hold commands
struct node{
  string data;
  float prio;
  node *next;
};

class priorityQueue{
private:
  node *front;
  node *rear;

public:
  priorityQueue(){
    front = nullptr;
    rear = nullptr;
  }
  bool isEmpty(){
    return front == nullptr;
  }
  void enqueue(string _data, float _prio){
    node *temp = new node;
    temp->data = _data;
    temp->prio = _prio;
    temp->next = nullptr;

    if(isEmpty()){
      front = temp;
      rear = temp;
    }
    else{
      node *cu = front;
      node *prev = nullptr;
      if(temp->prio < cu->prio){
        temp->next = front;
        front = temp;
      }
      else{
        while(cu != nullptr && temp->prio >= cu->prio){
          prev = cu;
          cu = cu->next;
        }
        prev->next = temp;
        temp->next = cu;
        if (temp->next == nullptr)
          rear = temp;
      }
    }
  }
  void dequeue(){
    if(!isEmpty()){
      node *temp = front;
      front = front->next;
      delete temp;
    }
  }
  node* getFront(){
    if (!isEmpty())
      return front;
    return nullptr;
  }
  void print() {
    node *cu = front;
    while (cu != nullptr) {
      cout << cu->data << endl;
      cu = cu->next;
    }
  }
};

//binary search tree 
struct tnode {
	string data;
	tnode* left;
	tnode* right;
	tnode(string d) {
		data = d;
		left = nullptr;
		right = nullptr;
	}
};
class bsTree{
private:
  tnode* root;
public:
  bsTree(){ 
    root = nullptr; 
  }
  void add(string data){ 
    root = insert(root, data); 
  }
  tnode* getRoot(){ 
    return root; 
  }
	tnode* insert(tnode* t, string x) {
		if(t == nullptr)
			t = new tnode(x);
		else if (x.length() < t->data.length())
			t->left = insert(t->left, x);
		else if (x.length() > t->data.length())
			t->right = insert(t->right, x);
		else if (x.length() == t->data.length())
      t->data = x;
		return t;
	}
  void preorder(tnode *n, ofstream &outputFile){
    if(n == nullptr)
      return;
    outputFile << n->data << endl;
    preorder(n->left, outputFile);
    preorder(n->right, outputFile);
  }
  void postorder(tnode *n, ofstream &outputFile){
    if(n == nullptr)
      return;
    postorder(n->left, outputFile);
    postorder(n->right, outputFile);
    outputFile << n->data << endl;
  }
  void inorder(tnode *n, ofstream &outputFile){
    if(n == nullptr)
      return;
    inorder(n->left, outputFile);
    outputFile << n->data << endl;
    inorder(n->right, outputFile);
  }
};

int main(int argc, char* argv[]){
  ArgumentManager am(argc, argv);
  string input(am.get("input"));
  string output(am.get("output"));

  ifstream inputFile(input);
  ofstream outputFile(output);
  
  priorityQueue commands;
  regularQueue messages;
  bsTree bstree;
  
  string line;
  string traversalMethod = "";
  
  while (getline(inputFile, line)){
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    if(!line.empty()){
      if(line.at(0) == 'P' || line.at(0) == 'I')
        traversalMethod = line;
      else{
        string p = "";
        for(int i = line.find('(')+1; i<line.find(')'); i++)
          p +=line[i];
        float prio = stof(p);
        commands.enqueue(line.substr(0, line.find('(')), prio);
      }
    }
  }
  //commands.print();
  while(!commands.isEmpty()){
    if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "DECODE"){
      int openingBracket = commands.getFront()->data.find('[');
      int closingBracket = commands.getFront()->data.find(']');
      messages.enqueue(commands.getFront()->data.substr(openingBracket+1, closingBracket-8)); 
      commands.dequeue();
    }
    else if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "REMOVE"){
      string c = commands.getFront()->data.substr(commands.getFront()->data.find('[')+1, 1);
      char r = c[0];
      if(messages.getFront() != NULL){
        string temp = messages.getFront()->data;
        temp.erase(remove(temp.begin(), temp.end(), r), temp.end());
        messages.dequeue();
        messages.enqueue(temp);
      }
      commands.dequeue();
    }
    else if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "REPLACE"){
      string charToReplace = commands.getFront()->data.substr(commands.getFront()->data.find('[')+1, 1);
      string newChar = commands.getFront()->data.substr(commands.getFront()->data.find('[')+3, 1);
      if(messages.getFront() != NULL){
        string temp = messages.getFront()->data;
        replace(temp.begin(), temp.end(), charToReplace[0], newChar[0]);
        messages.dequeue();
        messages.enqueue(temp);
      }
      commands.dequeue();
    }
    else if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "ADD"){
      string addAfterThis = commands.getFront()->data.substr(commands.getFront()->data.find('[')+1, 1);
      string charToAdd = commands.getFront()->data.substr(commands.getFront()->data.find('[')+3, 1);
      if(messages.getFront() != NULL){
        string temp = messages.getFront()->data;
        for(int i=0; i<temp.size(); i++){
          if(temp[i] == addAfterThis[0])
            temp.insert(i+1, charToAdd);
        }
        messages.dequeue();
        messages.enqueue(temp);
      }
      commands.dequeue();
    }
    else if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "SWAP"){
      string key1 = commands.getFront()->data.substr(commands.getFront()->data.find('[')+1, 1);
      string key2 = commands.getFront()->data.substr(commands.getFront()->data.find('[')+3, 1);
      if(messages.getFront() != NULL){
        string message = messages.getFront()->data;
        for(int i = 0; i<message.size(); i++){
          if(message[i] == key1[0])
            message[i] = key2[0];
          else if(message[i] == key2[0])
            message[i] = key1[0];
        }
        messages.dequeue();
        messages.enqueue(message);
      }
      commands.dequeue();
    }
    else if(commands.getFront()->data.substr(0, commands.getFront()->data.find(':')) == "BST"){
      if(messages.getFront() != NULL){
        bstree.add(messages.getFront()->data);
        messages.dequeue();
      }
      commands.dequeue();
    }
  }
  if(traversalMethod == "Preorder")
    bstree.preorder(bstree.getRoot(), outputFile);
  else if(traversalMethod == "Postorder")
    bstree.postorder(bstree.getRoot(), outputFile);
  else if(traversalMethod == "Inorder")
    bstree.inorder(bstree.getRoot(), outputFile);
}