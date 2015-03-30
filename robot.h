#ifndef ROBOT_H
#define ROBOT_H
/*
 * Robot 模块
 * MVC结构
 * 以效率角度未使用多线程锁，异步安全锁，但是从当前表现机械臂的运动基本正确
 * 后期如果出现问题再考虑加锁。
 */
#include <QWidget>

class Model;
class View;
class Control;

class Robot
{
public:
    Robot();
    ~Robot();
    View *getView() const { return view; }
    Control *getControl() const { return control; }
private:
    Model *model;
    View *view;
    Control *control;
};

#endif // ROBOT_H
