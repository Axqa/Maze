#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazegenerator.h"
#include "mazeview.h"
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MazeGenerator *mg, QWidget *parent = nullptr);
    ~MainWindow();

private:
    MazeGenerator *mg;
    MazeView *mv;
    Ui::MainWindow *ui;
};

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget *parent);
    void focusInEvent(QFocusEvent *) override;

};


#endif // MAINWINDOW_H
