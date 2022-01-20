#ifndef TBOX_EVENTX_TIMERS_H_20220119
#define TBOX_EVENTX_TIMERS_H_20220119

#include <string>
#include <functional>
#include <chrono>

#include <tbox/base/cabinet.hpp>
#include <tbox/base/defines.h>

#include <tbox/event/forward.h>

namespace tbox::eventx {

//! 定时任务管理器
//! 让开发者轻松创建定时任务而不必关心定时器的生命期
class Timers {
  public:
    using Token = cabinet::Token;
    using Callback = std::function<void(const Token &)>;
    using Milliseconds = std::chrono::milliseconds;
    using TimePoint = std::chrono::system_clock::time_point;

  public:
    explicit Timers(event::Loop *wp_loop);
    virtual ~Timers();

    NONCOPYABLE(Timers);
    IMMOVABLE(Timers);

  public:
    Token doEvery(const Milliseconds &m_sec, const Callback &cb);
    Token doAfter(const Milliseconds &m_sec, const Callback &cb);
    Token doAt(const TimePoint &time_point, const Callback &cb);

    bool cancel(const Token &token);

    void cleanup(); //!< WARN: don't invoke in loop

  private:
    class Impl;
    Impl *impl_ = nullptr;
};

}

#endif //TBOX_EVENTX_TIMERS_H_20220119