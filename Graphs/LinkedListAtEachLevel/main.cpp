#include <iostream>
#include <vector>
#include <list>

using namespace std;

class TreeNode
{
public:
  TreeNode* left;
  TreeNode* right;
  int value;
};

vector< list<TreeNode*> > lists;

void createLinkedList (TreeNode* root, int level) 
{
  if (root == NULL) // base case 
    return;
  cout << "Exploring root at level " 
	  << level 
	  << " with value " 
	  << root->value << endl;

  if (lists.size () <= level)
  {
    list<TreeNode* > alist;
    lists.push_back(alist);
  }
  cout << "The list size is " 
	  << lists.size() 
	  << endl; 

  lists[level].push_back(root);

  cout << "Calling root left" << endl;

  createLinkedList (root->left, level+1);
  
  cout << "Calling root right" << endl;
  createLinkedList (root->right, level+1);
}

void getLinkedLists (TreeNode* root) 
{
  int level = 0;
  createLinkedList (root, level); 
}

void createTree (TreeNode*& root){

  root = new TreeNode();

  //level 1
  root->left = new TreeNode();
  root->right = new TreeNode();
  root->value = 1;

  //level 2 
  root->left->left = new TreeNode();
  root->left->right = new TreeNode();
  root->left->value =11;
  root->right->left = new TreeNode();
  root->right->right = new TreeNode();
  root->right->value =12;

  //level 3  
  root->left->left->left = new TreeNode();
  root->left->left->right = new TreeNode();
  root->left->left->value = 111;
  root->left->right->left = new TreeNode();
  root->left->right->right = new TreeNode();
  root->left->right->value = 112;

  root->right->left->left = new TreeNode();
  root->right->left->right = new TreeNode();
  root->right->left->value = 121;
  root->right->right->left = new TreeNode();
  root->right->right->right = new TreeNode();
  root->right->right->value = 122;
}

void printLevels()
{
  for (int i = 0;i < lists.size();i++)
    {
      list<TreeNode*>::iterator iter;
      cout << "Level " << i << endl;
      cout << "====================" << endl;
      for (iter = lists[i].begin();iter != lists[i].end();iter++)
      {
	cout << (*iter)->value << endl;
      } 
    }
}

int main (int argc, char* argv[])
{
  TreeNode* root;
  createTree(root);
  
  getLinkedLists(root);

  printLevels();
  //need to iterate and delete the whole tree ...
  delete root;

}
