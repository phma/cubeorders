/******************************************************/
/*                                                    */
/* threads.h - multithreading                         */
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

#ifndef THREADS_H
#define THREADS_H

#ifdef __MINGW64__
#define MINGW_STDTHREAD_REDUNDANCY_WARNING
#include <../mingw-std-threads/mingw.thread.h>
#include <../mingw-std-threads/mingw.mutex.h>
#include <../mingw-std-threads/mingw.shared_mutex.h>
#else
#include <thread>
#include <mutex>
#include <shared_mutex>
#endif
#include <chrono>
#include <vector>
#include <array>
#include <map>

// These are used as both commands to the threads and status from the threads.
#define TH_RUN 1
#define TH_STOP 2
#define TH_ASLEEP 256

// These are used to tell threads to do things.
#define ACT_SUBSUM 1

struct ThreadAction
{
  int opcode;
  int param0;
  double param1;
  double param2;
  std::string filename;
  int flags;
  int result;
};

extern int currentAction;
extern std::chrono::steady_clock clk;

double busyFraction();
void startThreads(int n);
void joinThreads();
void enqueueAction(ThreadAction a);
ThreadAction dequeueResult();
bool actionQueueEmpty();
bool resultQueueEmpty();
void sleepDead(int thread);
void setThreadCommand(int newStatus);
int getThreadCommand();
int getThreadStatus();
void waitForThreads(int newStatus);
void waitForQueueEmpty();
int thisThread();
int nThreads();

class OrderThread
{
public:
  void operator()(int thread);
};

#endif
