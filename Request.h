#ifndef REQUEST_H
#define REQUEST_H

class Request
{
public:
  Request(int, int);
  int id, arrivalTime, target, startTime, endTime;

private:
  inline static int i;
};

Request::Request(int a, int t)
    : id(i++), arrivalTime(a), target(t), startTime(0), endTime(0)
{
}

#endif