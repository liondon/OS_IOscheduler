#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
using namespace std;

#include "Request.h"
#include "Scheduler.h"

void setupConfig(int, char **, char *&, Scheduler *&);
vector<Request *> *createRequests(string);
// TODO: the implementation should be put to .cpp

void setupConfig(int argc, char **argv,
                 char *&inputPath, Scheduler *&sched)
{
  // parsing the command line and set up config
  char algo = 0;
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "s:vqf")) != -1)
    switch (c)
    {
    case 's':
      sscanf(optarg, "%c", &algo);
      break;
    case '?':
      if (optopt == 's')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option '-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
      return;
    default:
      abort();
    }
  if (DEBUG)
  {
    printf("algo = %c", algo);
  }

  // set up scheduler
  switch (algo)
  {
  case 'i':
    sched = new FIFO();
    break;
  case 'j':
    sched = new SSTF();
    break;
  case 's':
    sched = new LOOK();
    break;
  case 'c':
    sched = new CLOOK();
    break;
  case 'f':
    sched = new FLOOK();
    break;
  default:
    return;
  }

  // set up inputPath
  inputPath = argv[optind++];
  if (DEBUG)
  {
    printf("input file path: %s\n", inputPath);
  }

  return;
}

vector<Request *> *createRequests(string inputPath)
{
  vector<Request *> *requests = new vector<Request *>();
  ifstream inputfile;
  regex delimiter("[\\s]+"); // Delimiters are spaces (\s) and/or commas
  string str;

  inputfile.open(inputPath);
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
      requests->push_back(req);
    }
  }
  inputfile.close();

  if (DEBUG)
  {
    cout << "/////// DEBUG: Requests parsed: ///////" << endl;
    for (auto req : *requests)
    {
      cout << req->arrivalTime << " " << req->target << endl;
    }
    cout << "//////////////////////////////////////" << endl;
  }
  return requests;
}
#endif