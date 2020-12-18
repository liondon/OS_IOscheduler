#ifndef SCHEDULER_H
#define SCHEDULER_H

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
  virtual void moveHead(int &, int);

private:
};

void Scheduler::moveHead(int &head, int target)
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
//////////////////////////// C L O O K //////////////////////////
class CLOOK : public Scheduler
{
public:
  void insertIOQ(Request *) override;
  Request *getNextReq() override;

private:
  multimap<int, Request *> IOQ;
  // could've use <set> to save space, using <map> here to simplify the code...
};

Request *CLOOK::getNextReq()
{
  if (DEBUG)
  {
    cout << "//////// DEBUG: IOQ is now... ////////" << endl;
    for (auto p : IOQ)
    {
      cout << p.second->arrivalTime << ": " << p.second->target << endl;
    }
    cout << "//////////////////////////////////////" << endl;
  }

  if (!IOQ.empty())
  {
    auto iter = IOQ.lower_bound(head);

    // check if there's another req
    if (iter == IOQ.end())
    {
      // if no, wrapup
      iter = IOQ.begin();
    }
    return IOQ.extract(iter).mapped();
  }
  return nullptr;
}

void CLOOK::insertIOQ(Request *req)
{
  IOQ.emplace(req->target, req);
  return;
}
///////////////////////////////////////////////////////////////
//////////////////////////// L O O K //////////////////////////
class LOOK : public Scheduler
{
public:
  void insertIOQ(Request *) override;
  Request *getNextReq() override;
  LOOK() : IOQ(), seekGreater(1) {}

private:
  multimap<int, Request *> IOQ;
  // could've use <set> to save space, using <map> here to simplify the code...
  bool seekGreater;
};

Request *LOOK::getNextReq()
{
  if (DEBUG)
  {
    cout << "//////// DEBUG: IOQ is now... ////////" << endl;
    for (auto p : IOQ)
    {
      cout << p.second->arrivalTime << ": " << p.second->target << endl;
    }
    cout << "//////////////////////////////////////" << endl;
  }

  if (!IOQ.empty())
  {
    auto iter = IOQ.lower_bound(head);

    // check if there exist req targeted at head: log(n)
    if (iter != IOQ.end() && iter->first == head)
    {
      return IOQ.extract(iter).mapped();
    }

    if (seekGreater)
    {
      // check if there's another req at the seek direction
      if (iter == IOQ.end())
      {
        // if no, change direction
        seekGreater = !seekGreater;
        // when there's req with same target, FCFS.
        iter = IOQ.find((--iter)->first);
      }
      return IOQ.extract(iter).mapped();
    }

    if (!seekGreater)
    {
      // check if there's another req at the seek direction
      if (iter == IOQ.begin())
      {
        // if no, change direction
        seekGreater = !seekGreater;
      }
      else
      {
        // when there's req with same target, FCFS.
        iter = IOQ.find((--iter)->first);
      }
      return IOQ.extract(iter).mapped();
    }
  }
  return nullptr;
}

void LOOK::insertIOQ(Request *req)
{
  IOQ.emplace(req->target, req);
  return;
}
///////////////////////////////////////////////////////////////
//////////////////////////// S S T F //////////////////////////
class SSTF : public Scheduler
{
public:
  void insertIOQ(Request *) override;
  Request *getNextReq() override;

private:
  multimap<int, Request *> IOQ;
  // could've use <set> to save space, using <map> here to simplify the code...
};

Request *SSTF::getNextReq()
{
  if (DEBUG)
  {
    cout << "//////// DEBUG: IOQ is now... ////////" << endl;
    for (auto p : IOQ)
    {
      cout << p.second->arrivalTime << ": " << p.second->target << endl;
    }
    cout << "//////////////////////////////////////" << endl;
  }

  if (!IOQ.empty())
  {
    auto upper = IOQ.lower_bound(head);
    int up = numeric_limits<int>::max(), low = numeric_limits<int>::max();
    auto lower = upper;

    if (upper != IOQ.end())
    {
      // get the one after the head
      up = upper->first - head;
    }
    if (lower != IOQ.begin())
    {
      // get the one before the head
      low = head - (--lower)->first;
    }
    // TODO: < or <= ? same direction?
    if (up >= low)
    {
      // when there's req with same target, FCFS.
      lower = IOQ.find(lower->first);
      return IOQ.extract(lower).mapped();
    }
    return IOQ.extract(upper).mapped();
  }
  return nullptr;
}

void SSTF::insertIOQ(Request *req)
{
  IOQ.emplace(req->target, req);
  return;
}
///////////////////////////////////////////////////////////////
//////////////////////////// F I F O //////////////////////////
class FIFO : public Scheduler
{
public:
  void insertIOQ(Request *) override;
  Request *getNextReq() override;

private:
  deque<Request *> IOQ;
};

Request *FIFO::getNextReq()
{
  if (!IOQ.empty())
  {
    Request *req = IOQ.front();
    IOQ.pop_front();
    return req; // TODO: may occur undifined behavior...
  }
  return nullptr;
}

void FIFO::insertIOQ(Request *req)
{
  IOQ.push_back(req);
  return;
}
///////////////////////////////////////////////////////////////

#endif