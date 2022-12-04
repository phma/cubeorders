/******************************************************/
/*                                                    */
/* cubeorders.cpp - hypercube corner orders           */
/*                                                    */
/******************************************************/

#include <iostream>
#include <cmath>
#include <quadlods.h>
#include "order.h"
#define MINCOUNT 10
using namespace std;

bool ready(map<string,uint64_t> &histo)
{
  map<string,uint64_t>::iterator i;
  bool ret=histo.size()>0;
  for (i=histo.begin();ret && i!=histo.end();++i)
    if (i->second<MINCOUNT)
      ret=false;
  return ret;
}

int main(int argc, char *argv[])
{
  int i,j;
  uint64_t count=MINCOUNT;
  string order;
  map<string,uint64_t> histo;
  init(5);
  while (count)
  {
    order=gen();
    histo[order]++;
    if (histo[order]>MINCOUNT)
    {
      count--;
      if (count==0 && !ready(histo))
	count=histo.size();
    }
    else
      count=histo.size();
  }
  cout<<histo.size()<<endl;
  return 0;
}
