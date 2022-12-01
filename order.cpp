/******************************************************/
/*                                                    */
/* order.cpp - hypercube corner order                 */
/*                                                    */
/******************************************************/

#include <quadlods.h>
#include <map>
#include <algorithm>
#include "order.h"
using namespace std;

Quadlods quad;
uint64_t twon;

void init(int n)
// n should be less than 31
{
  quad.init(n,0);
  twon=(uint64_t)1<<n;
}

Order gen()
{
  uint64_t i,j,k;
  vector<mpq_class> elem,sums;
  map<mpq_class,uint64_t> sorted;
  map<mpq_class,uint64_t>::iterator it;
  Order ret;
  elem=quad.gen();
  sort(elem.begin(),elem.end());
  sums.push_back(0);
  for (i=1,k=0;i<twon;i++)
  {
    j=i&(i-1);
    if (j)
      sums.push_back(sums[j]+sums[i-j]);
    else
      sums.push_back(elem[k++]);
  }
  for (i=0;i<twon;i++)
    sorted[sums[i]]=i;
  for (it=sorted.begin();it!=sorted.end();++it)
    ret.seq.push_back(it->second);
  return ret;
}
