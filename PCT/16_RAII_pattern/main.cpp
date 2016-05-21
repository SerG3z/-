/*
 * 3.16 Реализация паттерна RAII для работы с потоками
 * Реализовать обёртку над стандартным классом std::thread в С++ в соответствии с паттерном
 * RAII (Resource Acquisition Is Initialization). Действие, выполняемое при завершении жизни
 * объекта обёртки, выбирается в конструкторе.
 * Написать тестовую программу, в которой создаётся p потоков с помощью созданной обёртки.
*/

#include <utility>
#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>

class ThreadWrapper
{

public:
    enum class Action
    {
        join,
        detach
    };

    ThreadWrapper(std::thread&& t, Action a):
        action{a}, th{std::move(t)}
    {}

    ~ThreadWrapper()
    {
        if(th.joinable()) {
            if (action == Action::join) {
                th.join();
            } else {
                th.detach();
            }
        }
    }

    std::thread &get()
    {
        return th;
    }

private:
    Action action;
    std::thread th;
};

void func()
{
    std::cout << "exit" << std::endl;
}

int main(void)
{
    static const size_t p = 10;

    std::vector<ThreadWrapper*> wrapper_threads;
    for (size_t i = 0; i < p; ++i) {
        wrapper_threads.emplace_back(new ThreadWrapper{std::thread(func), ThreadWrapper::Action::join});
    }

    sleep(1);

    while (!wrapper_threads.empty()) {
        delete wrapper_threads.back();
        wrapper_threads.pop_back();
    }

    return 0;
}
