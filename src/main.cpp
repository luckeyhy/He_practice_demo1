#include "scheduler.hpp"

using namespace demo;

int main(int argc, char** argv)
{
    int input_arr[MAX_NODE_NUM] = {1, 1, 2, 5, 5};
    memset(input_arr + 5, 0, (MAX_NODE_NUM - 5) * sizeof(int));

    Scheduler* mscheduler = new Scheduler();
    if(mscheduler -> mainThread(input_arr) < 0)
        return FAIL;
    return SUCCESS;
}