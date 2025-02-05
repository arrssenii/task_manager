#ifndef TASK_HPP
#define TASK_HPP

#include <string>

struct Task {
    int id;
    std::string title;
    std::string description;
    bool completed;
};

#endif // TASK_HPP
