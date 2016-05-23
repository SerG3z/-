/*
Потокобезопасный неблокируемый стек.
Реализовать (в виде класса или в виде набора функций) без использования блокировок
потокобезопасный стек. Стек должен поддерживать операции добавления и удаления
элементов. Необходимо разработать тестовую программу, в которой создаётся p тестовых
параллельных потоков, и каждый поток выполняет n операций (вид операций выбирается
случайным образом) со стеком для случайно сгенерированных элементов. Проблему АВА
при реализации стека решать любым способом.
Необходимо оценить пропускную способность очереди.
*/
#include <cstdlib>
#include <thread>
#include <vector>
#include <sys/time.h>
#include <cassert>
using namespace std;
#define MAX_THREADS 16
#include "lockfree2.h"

struct LockFreeElement2: public LockFreeStack2::Node
{
    int data;
};

boost::atomic<bool> running;

template<class Stack, class Element>
void Worker(Stack& st, Element* elems, int numElements, int* numOps, int threadId)
{
    unsigned int seed = rand();
    vector<Element*> mine;
    int ops = 0;
    for(int i=0; i<numElements; i++)
    {
        mine.push_back(&elems[i]);
        elems[i].data = 0;
    }
    while(!running.load(boost::memory_order_acquire)){}
    while(running.load(boost::memory_order_acquire))
    {
        Element* elem;
        switch(rand_r(&seed)&1)
        {
            case 0:
                if(mine.size())
                {
                    elem = mine.back();
                    assert(elem->data == 0);
                    elem->data = 1;
                    mine.pop_back();
                    st.Push(elem);
                }
                ops++;
                break;
            case 1:
                elem = static_cast<Element*>(st.Pop(threadId));
                if(elem != nullptr)
                {
                    assert(elem->data == 1);
                    elem->data = 0;
                    mine.push_back(elem);
                }
                ops++;
                break;
        }
    }
    *numOps = ops;
}

template<class Stack, class Element>
double Test(int nthreads)
{
    const int num_elements = 20;
    const int test_time = 5;
    const int test_iterations = 5;
    const int elem_per_thread = num_elements / nthreads;
    long long ops = 0;

    for(int it = 0; it < test_iterations; it++)
    {
        Stack st;
        Element* elements = new Element[num_elements];
        
        thread threads[MAX_THREADS];
        int numOps[MAX_THREADS] = {};
        
        for(int i = 0; i < nthreads; i++)
        {
            threads[i] = thread(Worker<Stack, Element>, ref(st), elements + i*elem_per_thread, elem_per_thread, &numOps[i], i);
        }

        running.store(true, boost::memory_order_release);
        sleep(test_time);
        running.store(false, boost::memory_order_release);

        for(int i = 0; i < nthreads; i++)
        {
            threads[i].join();
            ops += numOps[i];
        }
        
        delete[] elements;
    }
    return (double)ops / (test_time*test_iterations);
}

int main()
{
    for(int i=1; i<=MAX_THREADS; i++)
    {
        double lockFree2Time    = Test<LockFreeStack2, LockFreeElement2>(i);
    	printf("%d threads, LockFree: %d/sec, \n", i, (int)lockFree2Time);
	}
    return 0;
}