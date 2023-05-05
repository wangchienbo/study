#include<iostream>
#include<thread>
#include <stdarg.h>
#include<list>
#include <sys/types.h>
#include <unistd.h>
#include<utility>
#include<errno.h>
#include<functional>
#include <memory>
#include <mutex>
#include <future> 
#define frame packaged_task<void()>
using namespace std;
mutex mtx_task;
mutex mtx_thread;
int mythreadfun();
class thread_pool;
//任务
class task{
    public:
    list<shared_ptr<frame> > task_list;
    int task_num;//未处理的任务数量
    //初始化
    task(){
        task_num=0;
    }
    //添加任务，任务以函数形式传递进来
    template<typename T,typename ..._Args>//模版
    int add_task(T &&fun,_Args &&..._args){//函数名为fun，参数是_args...
        mtx_task.lock();//防止不安全访问，锁住
        typedef __bind<T, _Args...> type;//把函数名和函数成员组合成type
        function<decltype(fun(_args...))()> task_temp=type(_VSTD::forward<T>(fun), _VSTD::forward<_Args>(_args)...);//组合，decltyoe获取函数的类型，并且利用fun和_args进行填充
        auto task_ptr=make_shared<packaged_task<decltype(fun(_args...))()> >(task_temp);//把函数打包并设置智能指针指向他
        task_list.push_front(task_ptr);//指针入队
        task_num++;//任务数量加一
        mtx_task.unlock();//开锁
        return 1;
    }
    
};
task task_; 
class thread_pool{
    public:
    int thread_num;//线程数量
    int max_thread_num;//线程的最大数量
    list<thread *>threads;//线程列表
    //初始化
    thread_pool(){
        thread_num=0;
        max_thread_num=8;
    }
    //添加线程
    int add_creat(){
        thread *t1=(new thread(mythreadfun));//创建线程
        t1->detach();//从当前线程脱离
        mtx_thread.lock();//锁住
        threads.push_front(t1);//线程增加
        thread_num++;
        mtx_thread.unlock();//解锁
        return 1;
    }
    //检查是否需要添加线程
    void is_add_thread(){
        mtx_task.lock();//锁住
        if(task_.task_num>thread_num*5&&thread_num<max_thread_num) add_creat();//满足条件添加线程
        mtx_task.unlock();//解锁
    }
    //终止线程
    template<typename T>
    int stop(const T& fun){
        thread temp;
        mtx_task.lock();
        threads.remove(&fun);
        thread_num--;
        mtx_task.unlock();
        return 1;
    }
    //终止线程
    int stop(int fun){
        thread temp;
            auto it=threads.begin();
            for(int i=fun-1;i>0;i--)
            {
                it++;
            }
            mtx_task.lock();
            pthread_cancel((*(*it)).native_handle());
            threads.remove(*it);
            thread_num--;
            mtx_task.unlock();
        return 1;
    }

};
thread_pool pool;
//线程处理函数
int mythreadfun(){
    int flag=1;//标志是否有任务需要处理
    while(flag==1){
        flag=0;
        shared_ptr<frame> task_temp;//定义任务
        mtx_task.lock();
        if(task_.task_num!=0){//检查是否还有任务
            task_temp=task_.task_list.front();//获取任务
            task_.task_list.pop_front();//任务出队
            task_.task_num--;//数量减一
            flag=1;
        }
        mtx_task.unlock();//解锁
        if(flag==1)(*task_temp)(); //执行任务
        pool.is_add_thread();//判断是否需要添加线程
    }
    return 1;
}
void print(int a){
    cout<<getgid()<<" "<<(a+100)<<endl;
}
int main(){
    for(int i=0;i<100000;i++){
        task_.add_task(print,i);//添加任务
        pool.is_add_thread();//判断是否需要添加线程
    }
    sleep(5);//主进程睡眠
    return 0;
}
