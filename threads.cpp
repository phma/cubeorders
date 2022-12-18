/******************************************************/
/*                                                    */
/* threads.cpp - multithreading                       */
/*                                                    */
/******************************************************/
/* Copyright 2020-2022 Pierre Abbat.
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
#include <queue>
#include <set>
#include <cassert>
#include <climits>
#include <atomic>
#include <cmath>
#include <iostream>
#include "threads.h"
#include "random.h"

using namespace std;
namespace cr=std::chrono;

mutex actMutex;
mutex startMutex;
mutex anyThreadMutex;
map<int,mutex> pointBufferMutex,cubeMutex;
shared_mutex threadStatusMutex;
mutex tileDoneMutex;
mutex classTotalMutex;

atomic<int> threadCommand;
vector<thread> threads;
vector<int> threadStatus; // Bit 8 indicates whether the thread is sleeping.
vector<double> sleepTime;
queue<ThreadAction> actQueue,resQueue;
int currentAction;
map<thread::id,int> threadNums;

cr::steady_clock clk;

double busyFraction()
{
  int i,numBusy=0;
  threadStatusMutex.lock_shared();
  for (i=0;i<threadStatus.size();i++)
    if ((threadStatus[i]&256)==0)
      numBusy++;
  threadStatusMutex.unlock_shared();
  return (double)numBusy/i;
}

void startThreads(int n)
{
  int i,m;
  threadCommand=TH_RUN;
  sleepTime.resize(n);
  m=n*3;
  threadNums[this_thread::get_id()]=-1;
  for (i=0;i<n;i++)
  {
    threads.push_back(thread(OrderThread(),i));
    sleepTime[i]=i+1;
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

void joinThreads()
{
  int i;
  for (i=0;i<threads.size();i++)
    threads[i].join();
}

ThreadAction dequeueAction()
{
  ThreadAction ret;
  ret.opcode=0;
  actMutex.lock();
  if (actQueue.size())
  {
    ret=actQueue.front();
    actQueue.pop();
    if (ret.opcode)
      currentAction=ret.opcode;
  }
  actMutex.unlock();
  return ret;
}

void enqueueAction(ThreadAction a)
{
  actMutex.lock();
  actQueue.push(a);
  actMutex.unlock();
}

bool actionQueueEmpty()
{
  return actQueue.size()==0;
}

bool actionQueueFull()
{
  return actQueue.size()>=3*threadStatus.size();
}

ThreadAction dequeueResult()
{
  ThreadAction ret;
  ret.opcode=0;
  actMutex.lock();
  if (resQueue.size())
  {
    ret=resQueue.front();
    resQueue.pop();
  }
  actMutex.unlock();
  return ret;
}

void enqueueResult(ThreadAction a)
{
  actMutex.lock();
  resQueue.push(a);
  actMutex.unlock();
}

bool resultQueueEmpty()
{
  return resQueue.size()==0;
}

void sleep(int thread)
{
  sleepTime[thread]*=1.0625;
  if (sleepTime[thread]>1e5)
    sleepTime[thread]*=0.9375;
  threadStatusMutex.lock();
  threadStatus[thread]|=256;
  threadStatusMutex.unlock();
  this_thread::sleep_for(chrono::microseconds(lrint(sleepTime[thread])));
  threadStatusMutex.lock();
  threadStatus[thread]&=255;
  threadStatusMutex.unlock();
}

void sleepDead(int thread)
// Sleep to get out of deadlock.
{
  sleepTime[thread]*=1.0625;
  if (sleepTime[thread]>1e5)
    sleepTime[thread]*=0.9375;
  threadStatusMutex.lock();
  threadStatus[thread]|=256;
  threadStatusMutex.unlock();
  this_thread::sleep_for(chrono::microseconds(lrint(sleepTime[thread])));
  threadStatusMutex.lock();
  threadStatus[thread]&=255;
  threadStatusMutex.unlock();
}

void unsleep(int thread)
{
  sleepTime[thread]*=0.9375;
  if (sleepTime[thread]<1)
    sleepTime[thread]*=1.125;
  if (sleepTime[thread]<0 || std::isnan(sleepTime[thread]))
    sleepTime[thread]=1;
}

double maxSleepTime()
{
  int i;
  double max=0;
  for (i=0;i<sleepTime.size();i++)
    if (sleepTime[i]>max)
      max=sleepTime[i];
  return max;
}

void setThreadCommand(int newStatus)
{
  threadCommand=newStatus;
  //cout<<statusNames[newStatus]<<endl;
}

int getThreadCommand()
{
  return threadCommand;
}

int getThreadStatus()
/* Returns aaaaaaaaaabbbbbbbbbbcccccccccc where
 * aaaaaaaaaa is the status all threads should be in,
 * bbbbbbbbbb is 0 if all threads are in the same state, and
 * cccccccccc is the state the threads are in.
 * If all threads are in the commanded state, but some may be asleep and others awake,
 * getThreadStatus()&0x3ffbfeff is a multiple of 1048577.
 */
{
  int i,oneStatus,minStatus=-1,maxStatus=0;
  threadStatusMutex.lock_shared();
  for (i=0;i<threadStatus.size();i++)
  {
    oneStatus=threadStatus[i];
    maxStatus|=oneStatus;
    minStatus&=oneStatus;
  }
  threadStatusMutex.unlock_shared();
  return (threadCommand<<20)|((minStatus^maxStatus)<<10)|(minStatus&0x3ff);
}

void waitForThreads(int newStatus)
// Waits until all threads are in the commanded status.
{
  int i,n;
  threadCommand=newStatus;
  do
  {
    threadStatusMutex.lock_shared();
    for (i=n=0;i<threadStatus.size();i++)
      if ((threadStatus[i]&255)!=threadCommand)
	n++;
    threadStatusMutex.unlock_shared();
    this_thread::sleep_for(chrono::milliseconds(n));
  } while (n);
}

void waitForQueueEmpty()
// Waits until the action queue is empty and all threads have completed their actions.
{
  int i,n;
  do
  {
    n=actQueue.size();
    threadStatusMutex.lock_shared();
    for (i=0;i<threadStatus.size();i++)
      if (threadStatus[i]<256)
	n++;
    threadStatusMutex.unlock_shared();
    //if (freeRam()<lowRam/4)
      //cout<<"aoeu\n";
    cout<<n<<"    \r";
    cout.flush();
    this_thread::sleep_for(chrono::milliseconds(30));
  } while (n);
}

int thisThread()
{
  return threadNums[this_thread::get_id()];
}

int nThreads()
// Returns number of worker threads, not counting main thread.
{
  return threadStatus.size();
}

void OrderThread::operator()(int thread)
{
  long long h=0,i=0,j=0,n,nPoints=0,nChunks;
  long long blknum;
  ThreadAction act;
  startMutex.lock();
  if (threadStatus.size()!=thread)
  {
    cout<<"Starting thread "<<threadStatus.size()<<", was passed "<<thread<<endl;
    thread=threadStatus.size();
  }
  threadStatus.push_back(0);
  threadNums[this_thread::get_id()]=thread;
  startMutex.unlock();
  while (threadCommand!=TH_STOP)
  {
    if (threadCommand==TH_RUN)
    {
      threadStatusMutex.lock();
      threadStatus[thread]=TH_RUN;
      threadStatusMutex.unlock();
      act=dequeueAction();
      if (act.opcode)
      {
	act.order=makeOrder(act.tuple);
	enqueueResult(act);
	unsleep(thread);
      }
      else
	sleep(thread);
    }
  }
  threadStatusMutex.lock();
  threadStatus[thread]=TH_STOP;
  threadStatusMutex.unlock();
}
