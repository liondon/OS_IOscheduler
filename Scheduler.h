#ifndef SCHEDULER
#define SCHEDULER

#include <iostream>
#include <deque>
#include <map>

using namespace std;

#include "Request.h"

class Scheduler
{
public:
  virtual void insertIOQ(Request *) = 0;
  virtual Request *getNextReq() = 0;
  virtual void moveHead(int &, int) = 0;
  //using vector + idx is probably more space-optimized?

private:
};
//////////////////////////// S S T F //////////////////////////
class SSTF : public Scheduler
{
public:
  void insertIOQ(Request *);
  Request *getNextReq();
  void moveHead(int &, int);

private:
  multimap<int, Request *> IOQ;
  // could've use <set> to save space, using <map> here to simplify the code...
};

Request *SSTF::getNextReq()
{
  cout << "DEBUG: IOQ is now..." << endl;
  for (auto p : IOQ)
  {
    cout << p.second->arrivalTime << ": " << p.second->target << endl;
  }

  if (!IOQ.empty())
  {
    // check if there exist req targeted at head: log(n)
    auto iter = IOQ.find(head);
    if (iter != IOQ.end())
    {
      // ele with key=head already exist!
      return IOQ.extract(iter).mapped();
    }
    else
    {
      // helper ele with key=head inserted!
      const auto iter_dum = IOQ.insert(make_pair(head, nullptr));
      iter = iter_dum;
      int low = numeric_limits<int>::max(), up = numeric_limits<int>::max();
      Request *req = nullptr;
      if (++iter != IOQ.end())
      {
        // get the one after the head
        up = (iter)->first - head;
        req = iter->second;
      }
      iter--;
      if (iter != IOQ.begin())
      {
        // get the one before the head
        low = head - (--iter)->first;
      }
      if (up >= low) // TODO: < or <= ? same direction?
      {
        iter = IOQ.find(iter->first);
        req = iter->second;
      }
      else
      {
        iter = iter_dum;
        iter++;
      }

      // delete the dummy node & the popped node: log(n)
      IOQ.erase(iter);
      IOQ.erase(iter_dum);
      return req;
    }
  }
  return nullptr;
}

void SSTF::insertIOQ(Request *req)
{
  IOQ.emplace(req->target, req);
  return;
}

void SSTF::moveHead(int &head, int target)
{
  if (target > head)
  {
    head++;
  }
  else
  {
    head--;
  }
  return;
}
//////////////////////////////////////////////////////////////////

//////////////////////////// F I F O //////////////////////////
class FIFO : public Scheduler
{
public:
  void insertIOQ(Request *);
  Request *getNextReq();
  void moveHead(int &, int);

private:
  deque<Request *> IOQ;
};

Request *FIFO::getNextReq()
{
  if (!IOQ.empty())
  {
    Request *req = IOQ.front();
    IOQ.pop_front();
    return req;
  }
  return nullptr;
}

void FIFO::insertIOQ(Request *req)
{
  IOQ.push_back(req);
  return;
}

void FIFO::moveHead(int &head, int target)
{
  if (target > head)
  {
    head++;
  }
  else
  {
    head--;
  }
  return;
}
//////////////////////////////////////////////////////////////////

#endif