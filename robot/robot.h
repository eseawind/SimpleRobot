#ifndef ROBOT_H
#define ROBOT_H
/*
 * Robot 模块
 * MVC结构
 * 以效率角度未使用多线程锁，异步安全锁，但是从当前表现机械臂的运动基本正确
 * 后期如果出现问题再考虑加锁。
 */
#include <QWidget>
#include "model.h"
#include "view.h"
#include "control.h"

class Model;
class View;
class Control;
/// 机器人类
class Robot
{
public:
    /// A constructor
    Robot();
    /// A destructor
    ~Robot();
    /// 获取视图
    View *getView() const { return view; }
    /// 获取控制器
    Control *getControl() const { return control; }
private:
    Model *model;
    View *view;
    Control *control;
};

#endif // ROBOT_H
