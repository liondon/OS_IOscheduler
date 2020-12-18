#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
using namespace std;

#include "Request.h"

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