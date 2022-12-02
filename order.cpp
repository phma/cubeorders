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

string gen()
{
  uint64_t i,j,k,count=0;
  int64_t diff,lastdiff=1;
  vector<mpq_class> elem,sums;
  map<mpq_class,uint64_t> sorted;
  map<mpq_class,uint64_t>::iterator it;
  vector<uint32_t> seq;
  string ret;
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
    seq.push_back(it->second);
  for (i=1;i<seq.size()/2;i++)
  {
    diff=(int64_t)seq[i]-seq[i-1];
    if (diff==lastdiff)
      count++;
    else
    {
      if (count)
	ret+=to_string(lastdiff)+"("+to_string(count)+")";
      count=1;
      lastdiff=diff;
    }
  }
  if (count)
    ret+=to_string(diff)+"("+to_string(count)+")";
  return ret;
}
