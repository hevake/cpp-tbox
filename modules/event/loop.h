#ifndef TBOX_EVENT_LOOP_H
#define TBOX_EVENT_LOOP_H

#include <functional>
#include <chrono>
#include <string>
#include <vector>

#include "forward.h"
#include "stat.h"

namespace tbox {
namespace event {

class Loop {
  public:
    //! 创建默认类型的事件循环
    static Loop* New();
    //! 创建指定类型的事件循环
    static Loop* New(const std::string &engine_type);
    //! 获取引擎列表
    static std::vector<std::string> Engines();

    enum class Mode {
        kOnce,      //!< 仅执行一次
        kForever    //!< 一直执行
    };

    //! 执行事件循环
    virtual void runLoop(Mode mode = Mode::kForever) = 0;
    //! 退出事件循环
    virtual void exitLoop(const std::chrono::milliseconds &wait_time = std::chrono::milliseconds::zero()) = 0;

    //! 是否与Loop在同一个线程内
    virtual bool isInLoopThread() = 0;
    //! Loop是否正在运行
    virtual bool isRunning() const = 0;

    //! 委托延后执行动作
    using Func = std::function<void()>;
    virtual void runInLoop(const Func &func) = 0;
    virtual void runNext(const Func &func) = 0;
    virtual void run(const Func &func) = 0;
    /**
     * runInLoop(), runNext(), run() 区别
     *
     * runInLoop()
     *   功能：注入下一轮将执行的函数，有加锁操作，支持跨线程，跨Loop间调用；
     *   场景：常用于不同Loop之间委派任务或其它线程向Loop线程妥派任务。
     *
     * runNext()
     *   功能：注入本回调完成后立即执行的函数，无加锁操作，不支持跨线程与跨Loop间调用；
     *   场景：常用于不方便在本函数中执行的操作，比如释放对象自身。
     *   注意：仅Loop线程中调用，禁止跨线程操作
     *
     * runInLoop() 能替代 runNext()，但 runNext() 比 runInLoop() 更高效。
     *
     * run()
     *   功能：自动选择 runNext() 或是 runInLoop()。
     *         当与Loop在同一线程时，选择 runNext()，否则选择 runInLoop()。
     *   场景：当不知道该怎么选择，选它就对了。
     *
     * 使用建议：
     *   明确是Loop线程内的用 runNext(), 明确不是Loop线程内的用 runInLoop()。
     *   不清楚的直接用 run()。
     */

    //! 创建事件
    virtual FdEvent* newFdEvent() = 0;
    virtual TimerEvent* newTimerEvent() = 0;
    virtual SignalEvent* newSignalEvent() = 0;

    //! 统计操作
    virtual void setStatEnable(bool enable) = 0;
    virtual bool isStatEnabled() const = 0;
    virtual Stat getStat() const = 0;
    virtual void resetStat() = 0;

  public:
    virtual ~Loop() { }
};

}
}

#endif //TBOX_EVENT_LOOP_H
