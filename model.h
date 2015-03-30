#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QMatrix4x4>
struct STLObject;

class Model : public QObject
{
    Q_OBJECT
    float speedFactor;
    int defaultSpeed;
public:
    Model();
    ~Model();
    struct RobotData{
        float cur[4], target[4];
        float speed[4];
        int directionFactor[4];
        int direction[4];
    };
    enum ActionType{MASTER_ROTATE=0, ASSISTANT_ROTATE=1, BAR_ROTATE=2, BAR_MOVE=3};
    enum ModelType{BASE=0, MASTER=1, ASSISTANT=2, BAR=3};
    const QVector<float> &getModelVerteces() const;
    const QVector<float> &getModelNormals() const;

    unsigned int getModelVertecesNum(ModelType modelType) const { return arrayV[modelType].size()/3; }
    void initValue()
    {
        for(int i=0; i<4; ++i){
            data.cur[i] = data.target[i] = 0;
            data.speed[i] = defaultSpeed;
            data.directionFactor[i] = 1;
            data.direction[i] = 0;
        }
        action_master.setToIdentity();
        action_assistant.setToIdentity();
        action_bar.setToIdentity();
        action_bar_move.setToIdentity();
    }

    float CalcCurValue(ActionType actionType, float spendSeconds, bool *reached)
    {
        if(data.cur[actionType]==data.target[actionType]){
            if(reached) *reached=true;
            return data.cur[actionType];
        }

        float target = data.cur[actionType] + data.speed[actionType] * spendSeconds * data.direction[actionType];
        if((data.direction[actionType]>0&&target>data.target[actionType]) ||
           (data.direction[actionType]<0&&target<data.target[actionType])){
            if(reached) *reached = true;
            data.cur[actionType] = data.target[actionType];
        }
        else{
            if(reached) *reached = false;
            data.cur[actionType] = target;
        }
        setCurValue(actionType, data.cur[actionType]);
        return data.cur[actionType];
    }

    float getCurValue(ActionType actionType)
    {
        return data.cur[actionType];
    }

    bool setCurValue(ActionType actionType, float value)
    {
        switch (actionType) {
        case MASTER_ROTATE:
            action_master.setToIdentity();
            action_master.rotate(value, 0 ,1, 0);
            break;
        case ASSISTANT_ROTATE:
            action_assistant.setToIdentity();
            action_assistant.rotate(value, 0, 1, 0);
            break;
        case BAR_ROTATE:
            action_bar.setToIdentity();
            action_bar.rotate(value, 0, 1, 0);
            break;
        case BAR_MOVE:
            action_bar_move.setToIdentity();
            action_bar_move.translate(0, value, 0);
            break;
        default:
            return false;
            break;
        }
        data.cur[actionType] = value;
        data.direction[actionType] = sign(data.target[actionType] - data.cur[actionType]);
        return true;
    }
    bool addCurValue(ActionType actionType, float value)
    {
        switch (actionType) {
        case MASTER_ROTATE:
            action_master.rotate(value, 0 ,1, 0);
            break;
        case ASSISTANT_ROTATE:
            action_assistant.rotate(value, 0, 1, 0);
            break;
        case BAR_ROTATE:
            action_bar.rotate(value, 0, 1, 0);
            break;
        case BAR_MOVE:
            action_bar_move.translate(0, value, 0);
            break;
        default:
            return false;
            break;
        }
        data.cur[actionType]  += value;
        data.direction[actionType] = sign(data.target[actionType] - data.cur[actionType]);
        return true;
    }
    bool setTargetValue(ActionType actionType, float angle, float speed=-1)
    {
        data.target[actionType] = angle;
        if(speed>0&&speed<=100){
            data.speed[actionType] = speed * speedFactor;
            data.direction[actionType] = sign(angle - data.cur[actionType]);
        }
        else if(speed==-1)
            data.direction[actionType] = sign(angle - data.cur[actionType]);
        else
            return false;
        return true;
    }

    bool setSpeed(ActionType actionType, float speed)
    {
        if(speed>0&&speed<=100)
            data.speed[actionType] = speed * speedFactor;
        else
            return false;
        return true;
    }


    QMatrix4x4 matrix_base, matrix_master, matrix_assistant, matrix_bar;
    QMatrix4x4 action_master, action_assistant, action_bar, action_bar_move;
    QMatrix4x4 center_base, center_master, center_assistant, center_bar;
private:
    int sign(float value) { return value>0?1:(value<0?-1:0); }
    void getVerteces(STLObject *obj, QVector<float> &vertecs, QVector<float> &normals);
    STLObject *object[4];
    QVector<float> arrayV[4], arrayN[4];
    QVector<float> verteces, normals;
   // bool ready;
    RobotData data;

};

#endif // MODEL_H
