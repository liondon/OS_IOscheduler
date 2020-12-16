#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
using namespace std;

// #include "Scheduler.h"

int main()
{
  int currTime = 0;
  vector<pair<int, int>> arrivals;
  int idx = 0;
  // Scheduler *sched;

  ifstream inputfile;
  inputfile.open("./94_lab4_assign/input0");
  // Delimiters are spaces (\s) and/or commas
  regex delimiter("[\\s]+");
  string str;

  while (getline(inputfile, str))
  {
    if (inputfile.is_open())
    {
      if (*str.begin() == '#')
      {
        // all lines starting with '#' must be ignored
        continue;
      }
      vector<string> tokens(sregex_token_iterator(str.begin(), str.end(), delimiter, -1), {});
      arrivals.push_back(make_pair(stoi(tokens[0]), stoi(tokens[1])));
    }
  }
  // for (auto p : arrivals)
  // {
  //   cout << p.first << " " << p.second << endl;
  // }

  // a loop increments simulation time by one and checks whether any action is to be taken.
  // while (true)
  // {
  //   // 1) If a new I/O arrived to the system at this current time → add request to IO-queue ?
  //   // if (currTime == arrivals[idx].first)
  //   // {
  //   //   sched->insert(arrivals[idx].second);
  //   //   idx++;
  //   // }

  //   // 2) If an IO is active and completed at this time → Compute relevant info and store in IO request for final summary

  //   // 3) If an IO is active but did not yet complete → Move the head by one sector/track/unit in the direction it is going (to simulate seek)
  //   // 4) If no IO request active now (after (2)) but IO requests are pending → Fetch the next request and start the new IO.
  //   // 5) If no IO request is active now and no IO requests pending → exit simulation

  //   // 6) Increment time by 1
  //   currTime++;
  // }

  return 0;
}