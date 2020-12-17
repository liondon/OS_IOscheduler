#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <vector>
using namespace std;

int head = 0;
#include "Scheduler.h"
#include "Request.h"

int main()
{
  int currTime = 0;
  vector<Request *> requests;
  int reqIdx = 0;
  Scheduler *sched = new SSTF();
  Request *currIOReq = nullptr;
  int i = 0;
  int total_move = 0;

  // parsing input and get all the requests
  ifstream inputfile;
  regex delimiter("[\\s]+"); // Delimiters are spaces (\s) and/or commas
  string str;

  inputfile.open("./94_lab4_assign/input2");
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
      requests.push_back(req);
    }
  }
  // for (auto req : requests)
  // {
  //   cout << req->arrivalTime << " " << req->target << endl;
  // }

  // a loop increments simulation time by one and
  // checks whether any action is to be taken.
  while (true)
  {
    // cout << "currTime=" << currTime << ": " << endl;
    // 1) If a new I/O arrived to the system at current time
    if (reqIdx != requests.size() && currTime == requests[reqIdx]->arrivalTime)
    {
      // cout << currTime << ": "
      //      << "arrivalRequest=" << requests[reqIdx]->arrivalTime
      //      << endl;
      // add request to IO - queue
      sched->insertIOQ(requests[reqIdx++]);
    }

    // 2) If an IO is active and completed at this time
    if (currIOReq != nullptr && head == currIOReq->target)
    {
      // cout << currTime << ": "
      //      << "head=" << head
      //      << "target=" << currIOReq->target
      //      << endl;
      // Compute relevant info and store in IO request for final summary
      currIOReq->endTime = currTime;
      // cout << setw(5) << i++ << ": "
      //      << setw(5) << currIOReq->arrivalTime << " "
      //      << setw(5) << currIOReq->startTime << " "
      //      << setw(5) << currIOReq->endTime << endl;
      currIOReq = nullptr;
    }

    // 3) If an IO is active but did not yet complete
    if (currIOReq != nullptr)
    {
      // cout << currTime << ": "
      //      << "head=" << head
      //      << "target=" << currIOReq->target
      //      << endl;
      // Move the head by one sector/track/unit in the direction it is going (to simulate seek)
      sched->moveHead(head, currIOReq->target);
      total_move++;
    }

    // 4) If no IO request active now (after (2)) but IO requests are pending
    if (currIOReq == nullptr)
    {
      // cout << currTime << ": "
      //      << "head=" << head
      //      << endl;
      // Fetch the next request
      currIOReq = sched->getNextReq();
      if (currIOReq != nullptr)
      {
        // cout << "next request get! "
        //      << "head=" << head
        //      << "target=" << currIOReq->target
        //      << endl;

        // start the new IO
        currIOReq->startTime = currTime;
        continue;
      }
      else
      {
        // 5) If no IO request is active now and no IO requests pending
        if (reqIdx == requests.size())
        {
          // exit simulation
          break;
        }
      }
    }

    // 6) Increment time by 1
    currTime++;
  }

  int total_turnaround = 0, total_waittime = 0, max_waittime = 0;
  double avg_turnaround = 0, avg_waittime = 0;
  for (size_t i = 0; i < requests.size(); i++)
  {
    // printf("%5d: %5d %5d %5d\n"...);
    total_turnaround += (requests[i]->endTime - requests[i]->arrivalTime);
    int waittime = (requests[i]->startTime - requests[i]->arrivalTime);
    max_waittime = max(max_waittime, waittime);
    total_waittime += waittime;
    cout << setw(5) << i << ": "
         << setw(5) << requests[i]->arrivalTime << " "
         << setw(5) << requests[i]->startTime << " "
         << setw(5) << requests[i]->endTime << endl;
  }
  avg_turnaround = static_cast<double>(total_turnaround) / requests.size();
  avg_waittime = static_cast<double>(total_waittime) / requests.size();

  // printf("SUM: %d %d %.2lf %.2lf %d\n"...);
  cout
      << "SUM: "
      << setw(1) << currTime << " "
      << setw(1) << total_move << " "
      << setw(2) << avg_turnaround << " "
      << setw(2) << avg_waittime << " "
      << setw(1) << max_waittime << endl;

  return 0;
}