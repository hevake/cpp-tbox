#ifndef TBOX_STATE_MACHINE_H_20220320
#define TBOX_STATE_MACHINE_H_20220320

#include <functional>

namespace tbox {
namespace util {

//! HFSM，多层级有限状态机
class StateMachine {
  public:
    using StateID = int;   //! StateID 为 0 与 1 的两个状态为特定状态
                           //! StateID = 0 的状态为终止状态，用户可以不用定义
                           //! StateID = 1 的状态为默认的初始状态。也可以通过 setInitState() 重新指定
    using EventID = int;   //! EventID = 0 表示任意事件，仅在 addRoute() 时使用

    struct Event {
      EventID id = 0;
      void *extra = nullptr;

      Event() { }

      template <typename ET>
      Event(ET e) : id(static_cast<EventID>(e)) { }

      template <typename ET, typename DT>
      Event(ET e, DT *p) : id(static_cast<EventID>(e)), extra(p) { }
    };

    using ActionFunc = std::function<void(Event)>;
    using GuardFunc  = std::function<bool(Event)>;

    using StateChangedCallback = std::function<void(StateID/*from_state_id*/, StateID/*to_state_id*/, Event)>;

  public:
    StateMachine();
    ~StateMachine();

  public:
    /**
     * \brief   创建一个状态
     *
     * \param   state_id        状态ID号，StateID = 1 的状态默认为初始状态
     * \param   enter_action    进入该状态时的动作，nullptr表示无动作
     * \param   exit_action     退出该状态时的动作，nullptr表示无动作
     *
     * \return  bool    成功与否，重复创建会失败
     */
    bool newState(StateID state_id, const ActionFunc &enter_action, const ActionFunc &exit_action);

    template <typename S>
    bool newState(S state_id, const ActionFunc &enter_action, const ActionFunc &exit_action) {
        return newState(static_cast<StateID>(state_id), enter_action, exit_action);
    }

    /**
     * \brief   添加状态转换路由
     *
     * \param   from_state_id   源状态
     * \param   event_id        事件，0表示任意事件
     * \param   to_state_id     目标状态
     * \param   guard           判定条件，nullptr表示不进行判断
     * \param   action          转换中执行的动作，nullptr表示无动作
     *
     * \return  bool    成功与否
     *                  from_state_id，to_state_id所指状态不存在时会失败
     */
    bool addRoute(StateID from_state_id, EventID event_id, StateID to_state_id, const GuardFunc &guard, const ActionFunc &action);

    template <typename S, typename E>
    bool addRoute(S from_state_id, E event_id, S to_state_id, const GuardFunc &guard, const ActionFunc &action) {
        return addRoute(static_cast<StateID>(from_state_id),
                        static_cast<EventID>(event_id),
                        static_cast<StateID>(to_state_id),
                        guard, action);
    }

    /**
     * \brief   设置起始与终止状态
     *
     * \param   init_state_id   起始状态
     */
    void setInitState(StateID init_state_id);

    template <typename S>
    void setInitState(S init_state_id) {
        return setInitState(static_cast<StateID>(init_state_id));
    }

    //! 设置子状态机
    bool setSubStateMachine(StateID state_id, StateMachine *wp_sub_sm);

    template <typename S>
    bool setSubStateMachine(S state_id, StateMachine *wp_sub_sm) {
        return setSubStateMachine(static_cast<StateID>(state_id), wp_sub_sm);
    }

    //! 设置状态变更回调
    void setStateChangedCallback(const StateChangedCallback &cb);

    //! 启动状态机
    bool start();

    //! 停止状态机
    void stop();

    /**
     * \brief   运行状态机
     *
     * \param   event_id    事件
     *
     * \return  bool    状态是否变换
     */
    bool run(Event event);

    /**
     * \brief   获取当前状态ID
     *
     * \return  >0 当前状态ID
     * \return  =0 状态机未启动
     */
    StateID currentState() const;

    template <typename S>
    S currentState() const {
        return static_cast<S>(currentState());
    }

    bool isTerminated() const;

  private:
    class Impl;
    Impl *impl_;
};

}
}

#endif //TBOX_STATE_MACHINE_H_20220320