#include <vector>
#include <iostream>

using namespace std;

vector<string> sets;
void getAllSets(int num);
void getSet (int left, int right, string s, int position);

int main (int argc, char* argv[])
{
  int num =   atoi(argv[1]);

  getAllSets (num);

  for (int i =0;i<sets.size();i++)
  {
    cout << sets[i] << endl; 
  }

}

void getAllSets(int num)
{
  string str;
  getSet (num, num, str, 0);
}

void getSet (int left, int right, string s, int position) {

  if (left < 0 || right < left)
    return;
  cout << "Inside getset " << s << endl; 
  cout << "right " << right << endl; 
  cout << "left " << left << endl; 
  if (left == 0 && right == 0)
  {
    cout << "Adding " << s << endl; 
    sets.push_back(s);
    return;
  }

  if (left > 0)
  {
    s.insert(position,1, '(');
    cout << "s is " << s << endl; 
    cout << "Adding left paren " << left << endl; 
    getSet (left-1, right, s, position);
  }

  if (right > left)
  {
    cout << "s is " << s << endl; 
    cout << "position is " << position << endl; 
    s.insert(position,1, ')');
    cout << "Adding right paren " <<  right << endl; 
    getSet (left, right-1, s, position+1);
  }
}
