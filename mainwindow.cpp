#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robot/robot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    robot = new Robot();
    view = robot->getView();
    control = robot->getControl();
    this->setCentralWidget(view);

    connect(control, SIGNAL(actionFinished()),
            this, SLOT(onReached()));
    control->setFinishedSignal(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool immediate = false;
    if(event->modifiers() == Qt::ShiftModifier) immediate = true;
    switch (event->key()) {
    case Qt::Key_Left:
        view->rotateView(0,-10,0);
        break;
    case Qt::Key_Down:
        view->rotateView(10,0,0);
        break;
    case Qt::Key_Up:
        view->rotateView(-10,0,0);
        break;
    case Qt::Key_Right:
        view->rotateView(0,10,0);
        break;
    case Qt::Key_Q:
        control->RotateMaster(-40, 100, immediate);
        break;
    case Qt::Key_A:
        control->RotateMaster(40, 100, immediate);
        break;
    case Qt::Key_W:
        control->RotateAssistant(-40, 100, immediate);
        break;
    case Qt::Key_S:
        control->RotateAssistant(40, 100, immediate);
        break;
    case Qt::Key_E:
        control->RotateBar(-40, 100, immediate);
        break;
    case Qt::Key_D:
        control->RotateBar(40, 100 ,immediate);
        break;
    case Qt::Key_R:
        control->MoveBar(-40, 100, immediate);
        break;
    case Qt::Key_F:
        control->MoveBar(40, 100, immediate);
        break;
    case Qt::Key_U:
        control->RotateMasterAbs(-90, 50, immediate);
        break;
    case Qt::Key_J:
        control->RotateMasterAbs(90, 50, immediate);
        break;
    case Qt::Key_I:
        control->RotateAssistantAbs(-90, 50, immediate);
        break;
    case Qt::Key_K:
        control->RotateAssistantAbs(90, 50, immediate);
        break;
    case Qt::Key_O:
        control->RotateBarAbs(-90, 50, immediate);
        break;
    case Qt::Key_L:
        control->RotateBarAbs(90, 50, immediate);
        break;
    case Qt::Key_P:
        control->MoveBarAbs(40, 50, immediate);
        break;
    case Qt::Key_Semicolon:
        control->MoveBarAbs(0, 50, immediate);
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::onReached()
{
    qDebug()<<"Reached!";
}
