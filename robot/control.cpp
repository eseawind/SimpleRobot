#include "control.h"
#include "model.h"
#include "view.h"

Control::Control():
    internalMsecs(100.0),
    actionFinishedOpened(false)
{
    paintTick.setInterval(internalMsecs);
    connect(&paintTick, SIGNAL(timeout()), this, SLOT(onPaintTick()));
}

Control::~Control()
{
}

bool Control::actionAbs(Model::ActionType actionType, float angle, float speed, bool immediate)
{
    bool ret = false;
    if(immediate){
        // 关闭时钟中断减少异步中断修改数据带来的冲突
        paintTick.stop();
        ret = model->setCurValue(actionType, angle);
        float cur = model->getCurValue(actionType);
        ret = model->setTargetValue(actionType, angle, speed);
        if(ret)
            view->update();
        // 重启时钟，完成其他部位的运动
        paintTick.start();
    }else{
        ret = model->setTargetValue(actionType, angle, speed);
        if(ret)
            paintTick.start();
    }
    return ret;
}

bool Control::action(Model::ActionType actionType, float angle, float speed, bool immediate)
{
    bool ret = false;
    if(immediate){
       // 关闭时钟中断减少异步中断修改数据带来的冲突
       paintTick.stop();
       ret = model->addCurValue(actionType, angle);
       float cur = model->getCurValue(actionType);
       ret = model->setTargetValue(actionType, cur+angle, speed);
       if(ret)
            view->update();
       // 重启时钟，完成其他部位的运动
       paintTick.stop();
    }else{
       float cur = model->getCurValue(actionType);
       ret = model->setTargetValue(actionType, cur+angle, speed);
       if(ret)
           paintTick.start();
    }
    return ret;
}

void Control::onPaintTick()
{
    bool reached = false;
    bool allReached = true;
    model->CalcCurValue(model->MASTER_ROTATE, internalMsecs/1000, &reached);
    allReached &= reached;
    model->CalcCurValue(model->ASSISTANT_ROTATE, internalMsecs/1000, &reached);
    allReached &= reached;
    model->CalcCurValue(model->BAR_ROTATE, internalMsecs/1000, &reached);
    allReached &= reached;
    model->CalcCurValue(model->BAR_MOVE, internalMsecs/1000, &reached);
    allReached &= reached;
    if(allReached) {
        qDebug()<<"master"<<model->getCurValue(model->MASTER_ROTATE);
        qDebug()<<"assistant"<<model->getCurValue(model->ASSISTANT_ROTATE);
        qDebug()<<"bar rotate"<<model->getCurValue(model->BAR_ROTATE);
        qDebug()<<"bar move"<<model->getCurValue(model->BAR_MOVE);
        paintTick.stop();
        emit actionFinished();
    }
    view->update();
}


