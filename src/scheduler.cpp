#include "scheduler.hpp"

namespace demo{
int Scheduler::initList(int* input_arr)
{
    if(input_arr == nullptr || node_type_arr == nullptr || node_list_arr == nullptr)
    {
        ERROR << "Error initializing data!" << REND;
        return FAIL;
    }
    if(node_type_num < 1)
    {
        ERROR << "there is no linked list that needs to be initialized." << REND;
        return FAIL;
    }
    if(node_type_num > MAX_NODE_TYPE_NUM)
    {
        ERROR << "beyond capacity." << REND;
        return FAIL;
    }
    for(int i = node_type_num - 1; i >= 0; --i)
    {
        Node* m_node_tmp_head = (Node*)malloc(sizeof(Node)); // 可以使用关键字new；
        if(m_node_tmp_head == nullptr)
            return FAIL;
        m_node_tmp_head -> next = nullptr;
        node_list_arr[node_type_arr[i]] = m_node_tmp_head;
    }
    for(int node_num = 0; node_num < MAX_NODE_NUM; ++node_num)
    {
        if(input_arr[node_num] != 0)
        {
            mNode temp_mnode(input_arr[node_num]);
            Node* temp_node = (Node*)malloc(sizeof(Node)); // 可以使用关键字new；
            temp_node -> data = temp_mnode;
            temp_node -> next = nullptr;
            if(temp_node == nullptr)
            {
                ERROR << "description failed to create a linked list node." << REND;
                return FAIL;
            }
            addNode(node_list_arr[input_arr[node_num]], temp_node);
        }
        else
            break;
    }
    return SUCCESS;
}

int Scheduler::addNode(Node* current_node, Node* additional_node)
{
    if(current_node == nullptr || additional_node == nullptr)
    {
        ERROR << "the linked list itself has no nodes." << REND;
        return FAIL;
    }
    Node* temp_node = current_node;
    while(temp_node -> next != nullptr)
    {
        temp_node = temp_node -> next;
    }
    additional_node -> data.mnode_id = temp_node -> data.mnode_id + 1;
    temp_node -> next = additional_node;
    return SUCCESS;
}

void Scheduler::clearList()
{
    if(node_list_arr == nullptr || node_type_arr == nullptr || node_type_num == 0)
    {
        ERROR << "no linked list nodes need to be cleared." << REND;
        return;
    }
    for(int i = 0; i < node_type_num; ++i)
    {
        Node* temp_node = node_list_arr[node_type_arr[i]];
        while(temp_node != nullptr)
        {
            Node* clear_node = temp_node;
            temp_node = temp_node -> next;
            free(clear_node);
        }
    }
    INFO << "the list array was cleared successfully." << REND;
}

int Scheduler::getNodeType(int* input_arr)
{
    int count = 0;
    int temp_node_type[MAX_NODE_TYPE_NUM] = {0};
    int node_type_num = 0;

    while(input_arr[count] != 0)
    {
        if(count > MAX_NODE_NUM || input_arr[count] > MAX_NODE_TYPE_NUM)
            return FAIL;
        temp_node_type[input_arr[count++]] = 1;
    }

    for(int i = 0; i < sizeof(temp_node_type)/sizeof(int); ++i)
    {
        if(temp_node_type[i] != 0)
        {
            node_type_arr[node_type_num++] = i;
        }
    }

    return node_type_num;
}

void Scheduler::sameNodeExecute(Node* arg_node)
{
    Node* temp_node = arg_node;
    while(temp_node != nullptr)
    {
        char temp_data[1024];
        std::unique_lock<std::mutex> lock(mtx);

        int function_start_time = system_time;
        temp_node -> data.mNodeFunction();

        int function_end_time = system_time;
        int exec_time = function_end_time - function_start_time;
        temp_node -> data.execute_time = exec_time >= 0 ? exec_time : (1000000 + exec_time);
        int now_main_time = function_start_time;

        sprintf(temp_data, "%dus\t: %dms mnode_id: %d execute_time: %ld us count:%d\n",
                now_main_time, temp_node -> data.interval_ms, temp_node -> data.mnode_id,
                temp_node -> data.execute_time, temp_node -> data.execute_count);
        strcat(record_10ms, temp_data);
        lock.unlock();
        temp_node = temp_node -> next;
    }
}

int Scheduler::logToFile(const char* data, const char* file)
{
    std::ofstream outputFile(file, std::ios::out | std::ios::binary);

    if (outputFile.is_open()){
        outputFile.write(data, strlen(data));
        INFO << "the data was successfully written to the file." << REND;
        outputFile.close();
        return SUCCESS;
    } else {
        ERROR << "unable to open file." << REND;
        return FAIL;
    }
}

int Scheduler::mainThread(int* input_arr)
{
    node_type_num = getNodeType(input_arr);
    if(node_type_num < 1)
    {
        ERROR << "please enter nodes that meets the criterial." << REND;
        return FAIL;
    }
    if(initList(input_arr) < 0)
    {
        ERROR << "failed to initialize the linked list." << REND;
        return FAIL;
    }
    INFO << "linked list array created successfully" << REND;

    struct timeval daytime;

    gettimeofday(&daytime, nullptr);
    start_time = daytime.tv_usec;
    long int start_time_sec = daytime.tv_sec;

    std::thread child_thread(&Scheduler::childThread, this);
    while(system_time < MAINTHREAD_EXECUTE_TIME)
    {
        
        if(system_time - last_time >= NOTIFY_SIGNAL_TIME || system_time == 0)
        {
            std::lock_guard<std::mutex> lock1(mtx);
            notify_signal = 1;
            last_time = system_time;
        }
        if(feedback_signal == 1)
        {
            std::lock_guard<std::mutex> lock(mtx);
            feedback_signal = 0;
            INFO << record_10ms << REND;
            strcat(record_100ms, record_10ms);
            memset(record_10ms, 0, sizeof(record_10ms));
        }
        struct timeval now_time;
        gettimeofday(&now_time, nullptr);
        current_time = now_time.tv_usec;
        long int now_time_sec = now_time.tv_sec;
        system_time = current_time - start_time;

        if(system_time < 0 || (now_time_sec != start_time_sec))
            system_time += 1000000 * (now_time_sec -  start_time_sec);
    }
    INFO << "wait for the child thread to end." << REND;
    child_thread.join();

    clearList();
    logToFile(record_100ms, "data.txt");

    return SUCCESS;
}

void Scheduler::childThread()
{
    int count = 0;
    while(count < MAINTHREAD_EXECUTE_TIME/NOTIFY_SIGNAL_TIME)
    {
        if(notify_signal == 1)
        {
            count++;
            for(int i = 0; i < node_type_num; ++i)
            {
                int sched_interval_time = node_type_arr[i];
                if(count % sched_interval_time == 0)
                    sameNodeExecute(node_list_arr[sched_interval_time] -> next);
            }
            if(count % FEEDBACK_DATA_TIME == 0)
            {
                std::lock_guard<std::mutex> lock(mtx);
                feedback_signal = 1;
            }
            notify_signal = 0;
        }
    }
}
}