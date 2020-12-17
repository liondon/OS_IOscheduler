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
  //using vector + idx is probably more space-optimized?

private:
};


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
    // cout << "head++" << endl;
  }
  else
  {
    head--;
    // cout << "head--" << endl;
  }
  return;
}
//////////////////////////////////////////////////////////////////

#endif