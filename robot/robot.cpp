#include "robot.h"
#include "model.h"
#include "view.h"
#include "control.h"
// 创建资源
Robot::Robot()
{
    model = new Model();
    view = new View(model);

    control = new Control();
    control->setModel(model);
    control->setView(view);
}
// 反向删除资源
Robot::~Robot()
{
    delete control;
    delete view;
    delete model;
}

