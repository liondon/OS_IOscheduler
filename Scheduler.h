#ifndef SCHEDULER
#define SCHEDULER

#include <iostream>
#include <deque>
using namespace std;

#include "Request.h"

class Scheduler
{
public:
  virtual void insertIOQ(Request *) = 0;
  virtual Request *getNextReq() = 0;
  virtual void moveHead(int &, int) = 0;
  deque<Request *> IOQ;
  //using vector + idx is probably more space-optimized?

private:
};

class FIFO : public Scheduler
{
public:
  void insertIOQ(Request *);
  Request *getNextReq();
  void moveHead(int &, int);

private:
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
    // cout << "head++" << endl;
  }
  else
  {
    head--;
    // cout << "head--" << endl;
  }
  return;
}

#endif