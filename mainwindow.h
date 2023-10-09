#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazegenerator.h"
#include "mazeview.h"

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
#endif // MAINWINDOW_H
