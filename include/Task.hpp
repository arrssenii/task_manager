#ifndef TASK_HPP
#define TASK_HPP

#include <string>

struct Task {
    int id;
    std::string title;
    std::string description;
    std::string deadline;
    std::string priority;
    std::string status;
};

#endif // TASK_HPP
