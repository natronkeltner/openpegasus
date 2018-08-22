//%LICENSE////////////////////////////////////////////////////////////////
//
// Licensed to The Open Group (TOG) under one or more contributor license
// agreements.  Refer to the OpenPegasusNOTICE.txt file distributed with
// this work for additional information regarding copyright ownership.
// Each contributor licenses this file to you under the OpenPegasus Open
// Source License; you may not use this file except in compliance with the
// License.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/Common/PegasusAssert.h>
#include <Pegasus/Common/Mutex.h>
#include <Pegasus/Common/Thread.h>
#include <Pegasus/Common/AtomicInt.h>

#if defined(PEGASUS_OS_TYPE_UNIX)
# include <unistd.h>
# if defined(PEGASUS_HAS_SIGNALS)
#  include <sys/wait.h>
# endif
#endif

PEGASUS_USING_PEGASUS;
PEGASUS_USING_STD;

Boolean verbose = false;

static Mutex mutex;

const int TEST_COUNT = 100;
// This is set at a vary low ration because it appears that in reality
// the result is not very fair.  Depending on system, system loading,etc.
// and in particular use of VMs Success rations of as low as 5% have
// been measured. This assures that there is at least some usage of the
// second thread.
const int SUCCESS_RATIO = 3; // in percent

enum {
    STATE_PREPARE,
    STATE_PREPARE_LOCKED,
    STATE_READY,
    STATE_THREAD1,
    STATE_THREAD2
};

AtomicInt state;
float thread1Count = 0.0;
float thread2Count = 0.0;

/*
 * This test checks that Mutex:timed_lock() is fair and gives two competing
 * threads fair chance to get a mutex. See bug #9813 for details.
 * It checks so in sequence of TEST_COUNT attempts, checking
 * that at least SUCCESS_RATIO % of the locks are granted to either of threads.
 *
 * The sequence should be as follows:
 *   Main thread holds the mutex while creating T1 and T2 threads.
 *   T1 uses mutex.timed_lock() (and blocks).
 *   T2 uses muter.lock() (and blocks).
 *   The main thread makes sure both T1 and T2 are blocked (using yield()).
 *   The main thread releases the mutex.
 *   T1 or T2 gets the mutex and records the successful attempt.
 */

ThreadReturnType PEGASUS_THREAD_CDECL thread1(void* parm)
{
    // try to wait for the lock
    mutex.lock();
    // are we the first to get the lock?
    if (state.get() == STATE_READY)
    {
        if (verbose)
        {
            cout << "thread 1 got lock" << endl;
        }
        state.set(STATE_THREAD1);
        thread1Count++;
    }
    mutex.unlock();
    return 0;
}

ThreadReturnType PEGASUS_THREAD_CDECL thread2(void* parm)
{
    // try to wait for the lock
    if (mutex.timed_lock(10 * 1000)) {
        // are we the first to get the lock?
        if (state.get() == STATE_READY)
        {
            if (verbose)
            {
                cout << "thread 2 got lock" << endl;
            }
            state.set(STATE_THREAD2);
            thread2Count++;
        }
        mutex.unlock();
    }
    else
    {
        if (verbose)
        {
            cout << "thread 2 failed to get lock" << endl;
        }
    }
    return 0;
}


int main(int, char** argv)
{
    verbose = (getenv("PEGASUS_TEST_VERBOSE")) ? true : false;

    try
    {
        for (int i = 0; i < TEST_COUNT; i++)
        {
            mutex.lock();
            Thread *t1 = new Thread(thread1, 0, false );
            Thread *t2 = new Thread(thread2, 0, false );
            t1->run();
            t2->run();
            state.set(STATE_READY);

            // make sure both threads are waiting for the lock
            Threads::sleep(10);
            Threads::yield();
            Threads::sleep(10);
            Threads::yield();

            mutex.unlock();
            t1->join();
            t2->join();
        }
        cout << "Thread1 count: " << thread1Count << endl;
        cout << "Thread2 count: " << thread2Count << endl;
    }
    catch (Exception& e)
    {
        cerr << argv[0] << " Exception " << e.getMessage() << endl;
        exit(1);
    }

    float r1 = thread1Count*100/(thread1Count + thread2Count);
    float r2 = thread2Count*100/(thread1Count + thread2Count);
    if (r1 < SUCCESS_RATIO)
    {
        cout << argv[0] << " Test failed: thread1 was used less than "
                << SUCCESS_RATIO << "%" << endl;
        return 1;
    }
    if (r2 < SUCCESS_RATIO)
    {
        cout << argv[0] << " Test failed: thread2 was used less than "
                << SUCCESS_RATIO << "%" << endl;
        return 1;
    }

    cout << argv[0] << " +++++ passed all tests" << endl;
    return 0;
}
