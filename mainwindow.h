#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
class Robot;
class Control;
class View;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
public slots:
    void onReached();
private:
    Ui::MainWindow *ui;
    View *view;
    Control *control;
    Robot *robot;
};

#endif // MAINWINDOW_H
