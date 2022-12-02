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

int main(int argc, char *argv[])
{
  int i,j;
  string order;
  map<string,uint64_t> histo;
  init(5);
  for (i=0;i<100000;i++)
  {
    order=gen();
    histo[order]++;
  }
  cout<<histo.size()<<endl;
  return 0;
}
