#ifndef ASH_TASK_H_INCLUDED
#define ASH_TASK_H_INCLUDED

#include <functional>
#include <stdexcept>

namespace ash
{
    template <class T>
    class task;
}

template <class T>
class ash::task
{
private:
    T *process = nullptr;
    int estimated_time;
    int priority;

public:
    task();
    task(T &process, int estimated_time, int priority = 1);
    task(T *process, int estimated_time, int priority = 1);
    task(const task &other);
    ~task();

    T &get_process();
    const T &get_process() const;
    int get_estimated_time() const;
    int get_priority() const;
    bool is_finished() const;

    void run(const std::function<void(T &process)> &run_process, int time_limit);
};

template <class T>
ash::task<T>::task()
{
    estimated_time = 0;
    priority = 1;
}

template <class T>
ash::task<T>::task(T &process, int estimated_time, int priority)
{
    this->process = &process;
    this->estimated_time = estimated_time;
    this->priority = priority;
}

template <class T>
ash::task<T>::task(T *process, int estimated_time, int priority)
{
    this->process = process;
    this->estimated_time = estimated_time;
    this->priority = priority;
}

template <class T>
ash::task<T>::task(const task &other)
{
    process = new T(*other.process);
    estimated_time = other.estimated_time;
    priority = other.priority;
}

template <class T>
ash::task<T>::~task()
{
    delete process;
}

template <class T>
T &ash::task<T>::get_process()
{
    return *process;
}

template <class T>
const T &ash::task<T>::get_process() const
{
    return *process;
}

template <class T>
int ash::task<T>::get_estimated_time() const
{
    return estimated_time;
}

template <class T>
int ash::task<T>::get_priority() const
{
    return priority;
}

template <class T>
bool ash::task<T>::is_finished() const
{
    return estimated_time <= 0;
}

template <class T>
void ash::task<T>::run(const std::function<void(T &process)> &run_process, int time_limit)
{
    if (estimated_time <= 0)
    {
        throw std::exception("Timed out!");
    }

    run_process(process);
    estimated_time -= time_limit;
}

#endif