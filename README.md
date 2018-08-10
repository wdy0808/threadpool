# threadpool
C++11实现线程池
使用c++11新增特性（头文件thread，mutex，future，chrono）实现线程池
## 项目结构
| | 作用 | 备注 |
| -------- | :---- | :----|
| ThreadPool | 对外暴露接口 | 任务增加取消、获取线程池数码、修改线程数目增长模式 |
| PoolCondition | 线程池控制管理 | 记录管理线程池状态、任务队列与线程桥梁 |
| TaskQueue | 线程安全的任务队列 | 任务优先级、队列为空时阻塞线程并记录时间 |
| ThreadsWorker | 线程管理模块 | 线程入口函数、依赖任务执行时阻塞进程、增加线程数目 |
| Task | 任务统一接口 | 虚基类、任务执行统一接口run() |
| CancelTask | 任务被取消 | Task子类、run函数为空表示任务被取消 |
| DependentTask | 依赖任务接口 | Task子类、在run()函数中调用ready()决定是否执行依赖任务 |
## 对外接口
| 函数名 | 函数类型 | 备注 |
| -------- | :---- | :----|
| ThreadPool() | 默认构造函数 | 线程增长方式为每次增加一个、最大线程数5、初始线程数1、任务队列大小为100 |
| ThreadPool(int threadnum, int starthread, int tasknum) | 构造函数 | 线程增长方式默认为线性增长、每次增长初始线程数的一半（最小为1）|
| ThreadPool(ThreadIncreaseWay way, int startthread, int maxthread, int maxtask, int addnum) | 构造函数 | |
| bool addTask(Task* task, bool ifblock) | 添加任务 | 参数`ifblock`决定队列满时是否阻塞 |
| stop() | 关闭线程池 | 无法继续添加任务、队列内任务执行完后推出 |
| shutdown() | 立即关闭线程池 | 清空任务队列、当前线程执行完任务后退出 |
| getWorkedThreadsNum() | 获取当前工作线程数目 | |
| setIncreaseMode(ThreadIncreaseWay way, int num = 1) | 设置线程增长模式 | 参数num为单次线程增加数目 |
| cancelTask(Task*) | 取消任务 | |
## 线程动态调整
### 增长时机
任务队列内任务数大于当前线程池运行线程数
### 线程动态增长方式
**线性增长：** 每次增加相同数目的线程直到达到最大值  
**指数增长：** 增长函数为：<a href="https://www.codecogs.com/eqnedit.php?latex=a2^{t}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?a2^{t}" title="a2^{t}" /></a>，其中a为用户制定初始值，t为增加次数（从0开始）。一旦有线程超时推出会恢复改增长值为1
### 线程动态释放参数
阻塞等待任务超过两分钟会自动结束
## 任务添加取消
### 添加任务
函数返回值为是否成功添加任务。非阻塞添加时队列已满会返回false，阻塞添加会阻塞线程知道队列有位置。
### 取消任务
函数返回值为是否成功取消任务。参数为空指针（执行完会释放资源），或者参数指向任务正在执行，返回取消任务失败。
否则释放参数指向资源，将参数指针重新指向新的CancelTask对象
## 任务实现  
**常规任务：** 继承类Task，重载函数void run()，实现该任务具体工作，默认优先级为10，可调用setPriority(int)设置，优先级越高，优先级越高，表明任务越重要  
**依赖任务：** 继承类DependentTask，重载函数void run()，需要在该函数内调用ready(bool)决定是否执行依赖任务，若不设置会有错误提示，默认不执行依赖任务。调用dependentTo(DependentTask*)函数设置该任务为某个任务的依赖任务。
