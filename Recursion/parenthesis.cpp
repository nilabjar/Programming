
using namespace std;

vector<string> sets;

int main (int argc, char* argv[])
{
  int num =   atoi(argv[1]);

  getAllSets (num);

}

void getAllSets(int num)
{
  string str;

  getSet (num, num, str, 0);
}

void getSet (int left, int right, string s, int position) {

  if (left == 0 && right == 0)
  {
    sets.push_back(s);
  }

  if (left > 0)
  {
    s[position] = "(";
    getSet (left-1, right, s, position+1);
  }

  if (right < left)
  {
    s[position] = ")";
    getSet (left, right-1, s, position+1);
  }
}
