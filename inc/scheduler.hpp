#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

#include "mnode.hpp"
// #include "log.hpp"
// #include "returntype.hpp"
#include <thread>
#include <fstream>
#include <cstring>
#include <mutex>
#include <sys/time.h> // 后面再做优化。

#define MAX_NODE_NUM 100
#define MAX_NODE_TYPE_NUM 10
#define MAINTHREAD_EXECUTE_TIME 100000
#define NOTIFY_SIGNAL_TIME 1000
#define FEEDBACK_DATA_TIME 10 // 


namespace demo{
class Node{
public:
    mNode data;
    Node* next;
    Node() = default;
    ~Node() = default;
};

class Scheduler{
public:
   int initList(int* input_arr);
   int getNodeType(int* input_arr);
   int addNode(Node* current_node, Node* additional_node);
   void clearList();
   void sameNodeExecute(Node* arg_node);
   int logToFile(const char* data, const char* file);
   int mainThread(int* input_arr);
   void childThread();
private:
    int node_type_arr[MAX_NODE_TYPE_NUM]; // 后面再将所有数据类型严格化；
    Node* node_list_arr[MAX_NODE_NUM];
    int node_type_num;
    int system_time; // 主线程运行的时间；
    int last_time; // 主线程上一次通知子线程的时间；
    int current_time; // 当前的系统时间；
    int start_time; // 系统启动时间；
    int notify_signal; // 主线程通知子线程运的信号；
    int feedback_signal; // 子线程反馈信号
    char record_10ms[65535];
    char record_100ms[65535];
    std::mutex mtx;
};
}
#endif