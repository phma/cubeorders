/******************************************************/
/*                                                    */
/* cubeorders.cpp - hypercube corner orders           */
/*                                                    */
/******************************************************/

#include <iostream>
#include <cmath>
#include <quadlods.h>
#include "order.h"
using namespace std;

int main(int argc, char *argv[])
{
  int i,j;
  Order order;
  init(5);
  for (i=0;i<10;i++)
  {
    order=gen();
    for (j=0;j<order.seq.size();j++)
      cout<<order.seq[j]<<' ';
    cout<<endl;
  }
  return 0;
}
