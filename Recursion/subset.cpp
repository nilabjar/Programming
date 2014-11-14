#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

vector< vector<int> >
findsets(vector<int> members, int index);

int main(int argc, char* argv[])
{
  vector<int> theset;

  for(int i = 1;i < argc;i++)
  {
    int j = atoi(argv[i]);
    theset.push_back(j);
  }

  vector< vector<int> > allsets = findsets (theset, theset.size()-1);

  for (int i=0;i < allsets.size();i++)
  {
    cout << "\n" << endl;
    cout << "Set number " << i << endl;
    for (int j=0;j < allsets[i].size();j++)
    {
      cout << allsets[i][j] << ", ";
    }
    cout << "\n" << endl;
  }
}

vector< vector<int> >
findsets(vector<int> members, int index) {

  if (index < 0)
  {
    // return the null set ...
    vector< vector<int> > newSet;
    vector<int> emptySet;
    newSet.push_back(emptySet);
    return newSet;
  }

  vector< vector<int> > subsets = findsets(members, index-1);

  vector< vector<int> > newSet;

  for (int i=0;i < subsets.size();i++)
  {
    newSet.push_back(subsets[i]);
    subsets[i].push_back(members[index]);
    newSet.push_back(subsets[i]);
  }

  return newSet;
}


