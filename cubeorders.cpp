/******************************************************/
/*                                                    */
/* cubeorders.cpp - hypercube corner orders           */
/*                                                    */
/******************************************************/

#include <iostream>
#include <cmath>
#include <quadlods.h>
#include <boost/program_options.hpp>
#include "order.h"
#define MINCOUNT 10
using namespace std;
namespace po=boost::program_options;

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
  map<string,uint64_t>::iterator i;
  uint64_t count=MINCOUNT;
  string order,minorder,maxorder;
  map<string,uint64_t> histo;
  uint64_t minbar=0,maxbar=0;
  time_t now,then;
  mpz_class totalOrders;
  bool validArgs,validCmd=true;
  int j,n=0;
  po::options_description generic("Options");
  po::options_description hidden("Hidden options");
  po::options_description cmdline_options;
  po::positional_options_description p;
  po::variables_map vm;
  hidden.add_options()
    ("dimensions",po::value<int>(&n),"Dimensions");
  p.add("dimensions",1);
  cmdline_options.add(generic).add(hidden);
  try
  {
    po::store(po::command_line_parser(argc,argv).options(cmdline_options).positional(p).run(),vm);
    po::notify(vm);
  }
  catch (exception &e)
  {
    cerr<<e.what()<<endl;
    validCmd=false;
  }
  init(n);
  while (count)
  {
    order=gen();
    histo[order]++;
    minbar++;
    if (histo[order]>MINCOUNT)
    {
      count--;
      if (count==0 && !ready(histo))
	count=histo.size();
    }
    else
      count=histo.size();
    now=time(nullptr);
    if (now!=then)
    {
      cout<<histo.size()<<' '<<count<<"  \r";
      cout.flush();
      then=now;
    }
  }
  for (i=histo.begin();i!=histo.end();++i)
  {
    count+=i->second;
    if (i->second<minbar)
    {
      minbar=i->second;
      minorder=i->first;
    }
    if (i->second>maxbar)
    {
      maxbar=i->second;
      maxorder=i->first;
    }
  }
  cout<<count<<" total trials\n";
  cout<<"Histogram bars range:\n";
  cout<<minbar<<' '<<minorder<<endl;
  cout<<maxbar<<' '<<maxorder<<endl;
  cout<<histo.size()<<" orders, not counting signs and permutations\n";
  totalOrders=histo.size();
  for (j=1;j<=n;j++)
    totalOrders*=2*j;
  cout<<totalOrders<<" orders, counting signs and permutations\n";
  return 0;
}
