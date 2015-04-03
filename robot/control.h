#ifndef CONTROL_H
#define CONTROL_H
#include <QTimer>
#include <QObject>
#include "model.h"

class Model;
class View;
class Control : public QObject
{
    Q_OBJECT
public:
    Control();
    ~Control();
    float internalMsecs;
    bool RotateMasterAbs(float angle, float speed=-1, bool immediate=false){
        return actionAbs(model->MASTER_ROTATE, angle, speed, immediate);
    }

    bool RotateMaster(float angle, float speed=-1, bool immediate=false){
        return action(model->MASTER_ROTATE, angle, speed, immediate);
    }

    bool RotateAssistantAbs(float angle, float speed=-1, bool immediate=false){
        return actionAbs(model->ASSISTANT_ROTATE, angle, speed, immediate);
    }

    bool RotateAssistant(float angle, float speed=-1, bool immediate=false){
        return action(model->ASSISTANT_ROTATE, angle, speed, immediate);
    }

    bool RotateBarAbs(float angle, float speed=-1, bool immediate=false){
        return actionAbs(model->BAR_ROTATE, angle, speed, immediate);
    }

    bool RotateBar(float angle, float speed=-1, bool immediate=false){
        return action(model->BAR_ROTATE, angle, speed, immediate);
    }

    bool MoveBarAbs(float down, float speed=-1, bool immediate=false){
        return actionAbs(model->BAR_MOVE, -down, speed, immediate);
    }

    bool MoveBar(float down, float speed=-1, bool immediate=false){
        return action(model->BAR_MOVE, -down, speed, immediate);
    }
    void Stop();
    bool actionAbs(Model::ActionType actionType, float angle, float speed=-1, bool immediate=false);
    bool action(Model::ActionType actionType, float angle, float speed=-1, bool immediate=false);
    void setModel(Model *model) {
        this->model = model;
    }
    void setView(View *view) { this->view = view; }
    void setFinishedSignal(bool open)
    {
        actionFinishedOpened = open;
    }

protected slots:
    void onPaintTick();
private:
    Model *model;
    View *view;
    QTimer paintTick;
    bool actionFinishedOpened;
signals:
    void actionFinished();
};

#endif // CONTROL_H
