/******************************************************/
/*                                                    */
/* order.cpp - hypercube corner order                 */
/*                                                    */
/******************************************************/
/* Copyright 2022 Pierre Abbat.
 * This file is part of Cubeorders.
 *
 * Cubeorders is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cubeorders is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cubeorders. If not, see <http://www.gnu.org/licenses/>.
 */

#include <map>
#include <algorithm>
#include "order.h"
using namespace std;

Quadlods quadd;
uint64_t twon;

void init(int n)
// n should be less than 31
{
  quadd.init(n,0);
  twon=(uint64_t)1<<n;
}

vector<mpq_class> gen()
{
  return quadd.gen();
}

string makeOrder(vector<mpq_class> elem)
{
  uint64_t i,j,k,count=0;
  int64_t diff,lastdiff=1;
  vector<mpq_class> sums;
  map<mpq_class,uint64_t> sorted;
  map<mpq_class,uint64_t>::iterator it;
  vector<uint32_t> seq;
  string ret;
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
    sorted[sums[i]]=i; // Most time is spent here.
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
