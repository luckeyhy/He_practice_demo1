# 实现一个调度的简易功能：

<img src="./功能梗概.jpg" style="zoom:80%;" />

##### 	实现两个对象：`node`和`scheduler`；

​	当程序初始化时，主线程静态添加上图中的六个node(可以设计为动态输入)，并创建子线程，主线程每隔1ms通知子线程一次，每10ms接收子线程发送回来的统计结果。直到100ms，主线程等待子线程退出，并将所有统计结果写入到文本文件中。