#include"ThreadPool.h"

/*
class ThreadPool
{
    private:
        std::vector<std::thread> threads_; //线程池中的线程 
        std::queue<std::function<void()>> taskqueue_;//任务队列
        std::mutex mutex_;//任务队列同步的互斥锁
        std::condition_variable condition_;//任务队列同步的条件变量
        std::atomic_bool stop_;//在析构函数中 把stop的值设置为1， 退出全部线程

    public:
        //在构造函数中将启动threadnum个线程
        ThreadPool(size_t threadnum);
        //把任务添加到队列中
        void addtask(std::function<void()> task);
        //在析构函数中将停止线程
        ~ThreadPool();
};
*/
 //在构造函数中将启动threadnum个线程
ThreadPool::ThreadPool(size_t threadnum, const std::string& threadtype):stop_(false), threadtype_(threadtype)
{
    //指定线程数量
    for(size_t i = 0; i < threadnum; i++)
    {
        threads_.emplace_back([this]()
        {
            printf("create %s threads(%d)\n", threadtype_.c_str(), syscall(SYS_gettid));//显示线程id
            while(stop_ == false)
            {
                std::function<void()> task;;//用于存放出队的元素

                //////////////////////////////////////////////////
                {
                    std::unique_lock<std::mutex> lock(this->mutex_); //锁作用域
                    //等待生产者的条件变量 一开始所有任务都阻塞在这里 来一个任务 取走一个线程
                    condition_.wait(lock, [this]()
                    {
                        return ((this->stop_) || !this->taskqueue_.empty()); //线程池停止或者任务队列不为空 可以获取锁之后向下执行
                    });
                    //在线程池停止之前 如果任务队列中还有任务 执行完再退出
                    if(stop_ && this->taskqueue_.empty()) return;
                    //出队一个任务
                    task = std::move(taskqueue_.front());
                    taskqueue_.pop();
                }
                /////////////////////////////////////////////////////////////

                printf("%s(%d) execute task.\n", threadtype_.c_str(),syscall(SYS_gettid));
                task();//执行任务
            }
        });
    }
}
//把任务添加到队列中
void ThreadPool::addtask(std::function<void()> task)
{
    ///////////////////////////////////
    {
        std::unique_lock<std::mutex> lock(mutex_);
        taskqueue_.push(task);
    }
    ////////////////////////////////////

    condition_.notify_one();//有任务添加 唤醒一个线程
}
//在析构函数中将停止线程
ThreadPool::~ThreadPool()
{
    stop_ = true;

    condition_.notify_all();//唤醒全部线程 退出

    for(std::thread &th : threads_) 
    {
        th.join();
    }
}

