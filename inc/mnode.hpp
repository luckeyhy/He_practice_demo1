#ifndef __MNODE_HPP__
#define __MNODE_HPP__

#include <chrono>
#include <iostream>
#include "log.hpp"
#include "returntype.hpp"

namespace demo{
class mNode{
public:
    mNode() = default;
    ~mNode() = default;
    mNode(int arg_interval_ms) : interval_ms(arg_interval_ms), execute_count(0), mnode_id(0), execute_time(0) {}
    inline void mNodeFunction()
    {
        execute_count++;
        int i = 0;
        while(i < interval_ms * 600)
        {
            i++;
        }
    }
public:
    int interval_ms;
    int execute_count;
    int mnode_id;
    int execute_time;
};
}
#endif