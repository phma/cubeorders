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
  string order;
  init(5);
  for (i=0;i<10;i++)
  {
    order=gen();
    cout<<order<<endl;
  }
  return 0;
}
