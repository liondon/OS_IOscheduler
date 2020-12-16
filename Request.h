#ifndef REQUEST
#define REQUEST

class Request
{
public:
  Request(int, int);
  int arrivalTime, target, startTime, endTime;

private:
};

Request::Request(int a, int t)
    : arrivalTime(a), target(t), startTime(0), endTime(0)
{
}

#endif