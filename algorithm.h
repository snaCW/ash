#ifndef ASH_ALGORITHM_H_INCLUDED
#define ASH_ALGORITHM_H_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::string current;
    for (char c : str)
    {
        if (delimiter == c)
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);

    return result;
}

std::vector<std::string> split(const std::string &str, std::vector<char> delimiters)
{
    std::sort(delimiters.begin(), delimiters.end());
    
    std::vector<std::string> result;
    std::string current;
    for (char c : str)
    {
        auto it = std::lower_bound(delimiters.begin(), delimiters.end(), c);
        if (*it == c)
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);

    return result;
}

std::vector<std::string> split(const std::string &str, std::string delimiters)
{
    return split(str, std::vector<char>(delimiters.begin(), delimiters.end()));
}

void quick_split(const std::string &str, std::vector<char> delimiters, std::vector<std::string> &result, char begin, char end)
{
    std::vector<bool> is_delimiter(end - begin, false);
    for (char delimiter : delimiters)
    {
        if (delimiter < begin || delimiter >= end)
            throw std::invalid_argument("Delimiter was out of range:\nDelimiter: " + std::string{delimiter} + "\t Begin: " + begin + "\t End: " + end + '\n');
        is_delimiter[delimiter - begin] = true;
    }
    
    std::string current;
    for (char c : str)
    {
        if (c >= begin && c < end && is_delimiter[c - begin])
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);
}

/*
template <class T>
static void round_robin(ash::priority_queue<task<T>> &processing_queue, int time_limit, const std::function<void (T &element)> &run_process)
{
    for (auto it = processing_queue.rbeing(); it != processing_queue.rend(); ++it)
    {
        it->second->iterate_on_all(run_process);
    }
}

template <class T>
static void sort(ash::stack<T> &input)
{
    ash::stack<T> temp;

    while (!input.empty())
    {
        T value = input.top();
        input.pop();

        while (!temp.empty() && temp.top() < value)
        {
            input.push(temp.top());
            temp.pop();
        }

        temp.push(value);
    }

    input = temp;
}
*/

#endif