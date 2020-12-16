#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
using namespace std;

#include "Scheduler.h"
#include "Request.h"

int main()
{
  int currTime = 0;
  vector<Request *> arrivals;
  int idx = 0;
  Scheduler *sched = new FIFO();
  int currIOReq = 0, head = 0;

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
      Request *req = new Request(stoi(tokens[0]), stoi(tokens[1]));
      arrivals.push_back(req);
    }
  }
  for (auto req : arrivals)
  {
    cout << req->arrivalTime << " " << req->target << endl;
  }

  // // a loop increments simulation time by one and checks whether any action is to be taken.
  // while (true)
  // {
  //   // 1) If a new I/O arrived to the system at current time
  //   if (currTime == arrivals[idx]->arrivalTime)
  //   {
  //     // add request to IO-queue
  //     sched->insertIOQ(arrivals[idx]);
  //     idx++;
  //   }

  //   // 2) If an IO is active and completed at this time
  //   if (currIOReq == head)
  //   {
  //     // Compute relevant info and store in IO request for final summary
  //   }

  //   // 3) If an IO is active but did not yet complete → Move the head by one sector/track/unit in the direction it is going (to simulate seek)

  //   // 4) If no IO request active now (after (2)) but IO requests are pending
  //   if (!currIOReq && !sched->IOQ.empty())
  //   {
  //     // Fetch the next request
  //     currIOReq = sched->getNextReq();

  //     // start the new IO
  //   }

  //   // 5) If no IO request is active now and no IO requests pending
  //   if (!currIOReq && sched->IOQ.empty())
  //   {
  //     // exit simulation
  //     break;
  //   }

  //   // 6) Increment time by 1
  //   currTime++;
  // }

  return 0;
}