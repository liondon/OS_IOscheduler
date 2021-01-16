#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

int head = 0;

// use this to control logging for now...
#define DEBUG 0

#include "Helpers.h"
#include "Scheduler.h"
#include "Request.h"

int main(int argc, char **argv)
{
  // config
  Scheduler *sched = nullptr;
  char *inputPath;

  // simulation control
  vector<Request *> *requests;
  int reqIdx = 0;
  Request *currIOReq = nullptr;

  // statistics
  int currTime = 0, total_move = 0,
      total_turnaround = 0, total_waittime = 0, max_waittime = 0;
  double avg_turnaround = 0, avg_waittime = 0;

  // setup config from command arguments
  setupConfig(argc, argv, inputPath, sched);

  // input Parser: parsing input and get all the requests
  requests = createRequests(inputPath);

  // Simulator: a loop increments simulation time by one and
  // checks whether any action is to be taken.
  // 5) If no IO request is active now and no IO requests pending -> exit
  while (currIOReq != nullptr || reqIdx != requests->size())
  {
    // 1) If a new I/O arrived to the system at current time -> add to IOQ
    if (reqIdx != requests->size() && currTime == (*requests)[reqIdx]->arrivalTime)
    {
      Request *req = (*requests)[reqIdx++];
      if (DEBUG)
      {
        cout << currTime << ": "
             << " arrivalRequest=" << req->arrivalTime << ": " << req->target
             << endl;
      }

      // add request to IO_queue
      sched->insertIOQ(req);
    }

    // 2) If an IO is active and completed at this time -> req completion
    if (currIOReq != nullptr && head == currIOReq->target)
    {
      if (DEBUG)
      {
        cout << currTime << ": "
             << " head=" << head
             << " target=" << currIOReq->target
             << endl;
      }

      // Compute relevant info and store in IO request for final summary
      currIOReq->endTime = currTime;
      if (DEBUG)
      {
        cout << setw(5) << currIOReq->id << ": "
             << setw(5) << currIOReq->arrivalTime << " "
             << setw(5) << currIOReq->startTime << " "
             << setw(5) << currIOReq->endTime << endl;
      }
      currIOReq = nullptr;
    }

    // 3) If an IO is active but did not yet complete -> move head
    if (currIOReq != nullptr)
    {
      if (DEBUG)
      {
        cout << currTime << ": "
             << " head=" << head
             << " target=" << currIOReq->target
             << endl;
      }
      // Move the head by one sector/track/unit in the direction it is going (to simulate seek)
      if (currIOReq->target > head)
      {
        head++;
      }
      else
      {
        head--;
      }
      total_move++;
    }

    // 4) If no IO request active now but IO requests are pending -> get next req
    if (currIOReq == nullptr)
    {
      if (DEBUG)
      {
        cout << currTime << ": "
             << " head=" << head
             << endl;
      }

      // Fetch the next request
      currIOReq = sched->getNextReq();
      if (currIOReq != nullptr)
      {
        if (DEBUG)
        {
          cout << "next request get! "
               << " head=" << head
               << " target=" << currIOReq->target
               << endl;
        }

        // start the new IO
        currIOReq->startTime = currTime;
        continue;
      }
    }
    // 6) Increment time by 1
    currTime++;
  }

  // print per request summary
  for (size_t i = 0; i < requests->size(); i++)
  {
    // printf("%5d: %5d %5d %5d\n"...);
    total_turnaround += ((*requests)[i]->endTime - (*requests)[i]->arrivalTime);
    int waittime = ((*requests)[i]->startTime - (*requests)[i]->arrivalTime);
    max_waittime = max(max_waittime, waittime);
    total_waittime += waittime;
    cout << setw(5) << i << ": "
         << setw(5) << (*requests)[i]->arrivalTime << " "
         << setw(5) << (*requests)[i]->startTime << " "
         << setw(5) << (*requests)[i]->endTime << endl;
  }
  avg_turnaround = static_cast<double>(total_turnaround) / requests->size();
  avg_waittime = static_cast<double>(total_waittime) / requests->size();

  // print SUM statistics
  // printf("SUM: %d %d %.2lf %.2lf %d\n"...);
  cout << "SUM: "
       << setw(1) << --currTime << " "
       << setw(1) << total_move << " "
       << fixed << setprecision(2)
       << avg_turnaround << " "
       << avg_waittime << " "
       << setw(1) << max_waittime << endl;

  return 0;
}