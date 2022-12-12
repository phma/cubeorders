/******************************************************/
/*                                                    */
/* cubeorders.cpp - hypercube corner orders           */
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

#include <iostream>
#include <cmath>
#include <quadlods.h>
#include <boost/program_options.hpp>
#include "config.h"
#include "order.h"
#include "threads.h"
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
  int nthreads;
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
    validArgs=vm.count("dimensions")>0 && n>=0;
  }
  catch (exception &e)
  {
    cerr<<e.what()<<endl;
    validCmd=false;
  }
  if (validArgs && validCmd)
  {
    init(n);
    cout<<"Cubeorders version "<<VERSION<<" © "<<COPY_YEAR<<" Pierre Abbat\n"<<
      "Distributed under GPL v3 or later.\n"<<
      "This is free software with no warranty.\n";
    nthreads=thread::hardware_concurrency();
    startThreads(nthreads);
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
    setThreadCommand(TH_STOP);
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
    waitForThreads(TH_STOP);
    joinThreads();
  }
  else
  {
    cerr<<"Usage: cubeorders n\n";
    cerr<<"where n (number of dimensions) is a nonnegative integer.\n";
  }
  return 0;
}
