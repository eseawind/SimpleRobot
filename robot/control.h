#ifndef CONTROL_H
#define CONTROL_H
#include <QTimer>
#include <QObject>
#include "model.h"

class Model;
class View;
/// 控制类
class Control : public QObject
{
    Q_OBJECT
public:
    /// A constructor
    Control();
    /// A destructor
    ~Control();
    float internalMsecs;        ///< 定时刷新时间
    /// 旋转主臂到指定角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateMasterAbs(float angle, float speed=-1, bool immediate=false)
    {
        return actionAbs(model->MASTER_ROTATE, angle, speed, immediate);
    }
    /// 旋转主臂指定大小的角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateMaster(float angle, float speed=-1, bool immediate=false)
    {
        return action(model->MASTER_ROTATE, angle, speed, immediate);
    }
    /// 旋转副臂到指定角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateAssistantAbs(float angle, float speed=-1, bool immediate=false)
    {
        return actionAbs(model->ASSISTANT_ROTATE, angle, speed, immediate);
    }
    /// 旋转副臂指定大小的角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateAssistant(float angle, float speed=-1, bool immediate=false)
    {
        return action(model->ASSISTANT_ROTATE, angle, speed, immediate);
    }
    /// 旋转工具杆到指定角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateBarAbs(float angle, float speed=-1, bool immediate=false)
    {
        return actionAbs(model->BAR_ROTATE, angle, speed, immediate);
    }
    /// 旋转工具杆指定大小的角度
    /// @param angle 角度
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool RotateBar(float angle, float speed=-1, bool immediate=false)
    {
        return action(model->BAR_ROTATE, angle, speed, immediate);
    }
    /// 移动工具杆到指定位置
    /// @param down 位置
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool MoveBarAbs(float down, float speed=-1, bool immediate=false)
    {
        return actionAbs(model->BAR_MOVE, -down, speed, immediate);
    }
    /// 移动工具杆指定大小的位移
    /// @param down 位移
    /// @param speed 速度（-1 or 1-100）
    /// @param immediate 是否立即到达
    bool MoveBar(float down, float speed=-1, bool immediate=false)
    {
        return action(model->BAR_MOVE, -down, speed, immediate);
    }
    /// 停止运动
    void Stop();
    /// 设置数据模型
    /// @param model 数据模型指针
    void setModel(Model *model) {
        this->model = model;
    }
    /// 设置3D视图
    /// @param view 3D视图指针
    void setView(View *view) { this->view = view; }
    /// 开关结束信号
    /// @param open 打开结束信号?
    void setFinishedSignal(bool open)
    {
        actionFinishedOpened = open;
    }

protected slots:
    /// 定时器中断槽
    void onPaintTick();
private:
    /// 绝对运动抽象
    bool actionAbs(Model::ActionType actionType, float angle, float speed=-1, bool immediate=false);
    /// 相对运动抽象
    bool action(Model::ActionType actionType, float angle, float speed=-1, bool immediate=false);

    Model *model;               ///< 数据模型指针
    View *view;                 ///< 3D视图指针
    QTimer paintTick;           ///< 刷新定时器
    bool actionFinishedOpened;  ///< 是否打开结束信号?
signals:
    void actionFinished();      ///< 动作完成信号
};

#endif // CONTROL_H
