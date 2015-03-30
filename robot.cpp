#include "robot.h"
#include "model.h"
#include "view.h"
#include "control.h"

Robot::Robot()
{
    model = new Model();
    view = new View(model);

    control = new Control();
    control->setModel(model);
    control->setView(view);
}

Robot::~Robot()
{
    delete control;
    delete view;
    delete model;
}

