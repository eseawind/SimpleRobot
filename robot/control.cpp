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
    paintTick.stop();
}

void Control::Stop()
{
    paintTick.stop();
    model->setTargetValue(Model::MASTER_ROTATE, model->getCurValue(Model::MASTER_ROTATE));
    model->setTargetValue(Model::ASSISTANT_ROTATE, model->getCurValue(Model::ASSISTANT_ROTATE));
    model->setTargetValue(Model::BAR_ROTATE, model->getCurValue(Model::BAR_ROTATE));
    model->setTargetValue(Model::BAR_MOVE, model->getCurValue(Model::BAR_MOVE));
}

// 需要先判断speed是否正确再进行下一步，否则直接返回false
bool Control::actionAbs(Model::ActionType actionType, float angle, float speed, bool immediate)
{
    if( !((speed>0&&speed<=100)||speed==-1) ) return false;
    if(immediate){
        // 关闭时钟中断减少异步中断修改数据带来的冲突
        paintTick.stop();
        model->setCurValue(actionType, angle);
        model->setTargetValue(actionType, angle, speed);
        view->update();
        // 重启时钟，完成其他部位的运动
        paintTick.start();
    }else{
        model->setTargetValue(actionType, angle, speed);
        paintTick.start();
    }
    return true;
}

bool Control::action(Model::ActionType actionType, float angle, float speed, bool immediate)
{
    if( !((speed>0&&speed<=100)||speed==-1) ) return false;
    if(immediate){
       // 关闭时钟中断减少异步中断修改数据带来的冲突
       paintTick.stop();
       model->addCurValue(actionType, angle);
       float cur = model->getCurValue(actionType);
       model->setTargetValue(actionType, cur+angle, speed);
       view->update();
       // 重启时钟，完成其他部位的运动
       paintTick.stop();
    }else{
       float cur = model->getCurValue(actionType);
       model->setTargetValue(actionType, cur+angle, speed);
       paintTick.start();
    }
    return true;
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


