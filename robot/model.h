#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QMatrix4x4>
#include <cassert>
struct STLObject;
/// 机器人数据模型
class Model : public QObject
{
    Q_OBJECT
    /// 机器人位置信息结构体
    struct RobotData{
        float cur[4], target[4];
        float speed[4];
        int directionFactor[4];
        int direction[4];
    };
public:
    /// A constructor
    Model();
    /// A destructor
    ~Model();
    /// 动作类型
    enum ActionType{MASTER_ROTATE=0, ASSISTANT_ROTATE=1, BAR_ROTATE=2, BAR_MOVE=3};
    /// 模型部位
    enum ModelType{BASE=0, MASTER=1, ASSISTANT=2, BAR=3};
    /// 获取模型顶点
    const QVector<float> &getModelVerteces() const;
    /// 获取模型法向量
    const QVector<float> &getModelNormals() const;
    /// 获取模型顶点数目
    unsigned int getModelVertecesNum(ModelType modelType) const { return arrayV[modelType].size()/3; }
    /// 重置机器人位置
    void resetValue()
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
    /// 计算指定部位当前位置
    float CalcCurValue(ActionType actionType, float spendSeconds, bool *reached)
    {
        if(data.cur[actionType]==data.target[actionType]){
            if(reached) *reached=true;
            return data.cur[actionType];
        }

        float target = data.cur[actionType] + data.speed[actionType] * spendSeconds * data.direction[actionType];
        assert(data.speed[actionType]*spendSeconds>=0);
        if((data.direction[actionType]>0&&target>data.target[actionType]) ||
           (data.direction[actionType]<0&&target<data.target[actionType])){
            if(reached) *reached = true;
            data.cur[actionType] = data.target[actionType];
        }else{
            if(reached) *reached = false;
            data.cur[actionType] = target;
        }
        setCurValue(actionType, data.cur[actionType]);
        return data.cur[actionType];
    }
    /// 获取当前位置
    float getCurValue(ActionType actionType)
    {
        return data.cur[actionType];
    }
    /// 设置当前位置
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
            assert(false);
            break;
        }
        data.cur[actionType] = value;
        data.direction[actionType] = sign(data.target[actionType] - data.cur[actionType]);
        return true;
    }
    /// 对当前位置添加指定的值
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
            assert(false);
            break;
        }
        data.cur[actionType]  += value;
        data.direction[actionType] = sign(data.target[actionType] - data.cur[actionType]);
        return true;
    }
    /// 设置目标位置
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
            assert(false);
        return true;
    }
    /// 设置运动速度
    /// @param actionType 动作类型
    /// @param speed 速度(1-100)
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
    float speedFactor;      ///< 速度因子，调节全局速度比例
    int defaultSpeed;       ///< 默认速度
    RobotData data;         ///< 机器人运动数据
    STLObject *object[4];   ///< STL数据
    QVector<float> arrayV[4], arrayN[4]; ///< STL顶点和向量
    QVector<float> verteces, normals;    ///< STL顶点和向量

};

#endif // MODEL_H
