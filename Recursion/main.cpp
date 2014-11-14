#include <iostream>
#include <stdlib.h>
#include <map>

using namespace std;
map<int, int> no_of_ways;

int countWays (int steps)
{
  if (steps < 0)
    return 0;
  else if ( steps == 0)
    return 1;

  if (no_of_ways[steps] != 0)
    return no_of_ways[steps];

  no_of_ways[steps] = countWays(steps - 3)
    + countWays(steps - 2)
    + countWays(steps - 1);

  return no_of_ways[steps];
}


int main (int argc, char* argv[])
{
  cout << "Counting the number of steps" << endl;

  if (argc < 2)
  {
    cout << "Please input the number of steps" << endl;
    exit (1);
  }

  int i_steps = atoi(argv[1]);

  int ways = countWays (i_steps);

  cout << "The number of ways are " << ways << endl;
}
