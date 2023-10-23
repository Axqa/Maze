#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazegenerator.h"
#include "mazeview.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include "pathfinder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MazeGenerator *mg, QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent* event) override;

private:
    void StartMazeTimer();
    void ResetMaze();

public slots:
    void OnGenerate();
    void StepChange(int step);
    void OnMazeTimerStep();
    void PauseTimer();
    void ToFindPage();
    void ToGenPage();

private:
    MazeGenerator *mg;
    MazeView *mv;
    PathFinder *pf;
    Ui::MainWindow *ui;
    bool bindSizes = false;
    int n_col, n_row;

    int cur_step = 0;

    QTimer maze_timer;
    QElapsedTimer fps_timer;
    int timer_msec = 50;

    Mode mode = Mode::GENERATE;

    std::vector<qint64> time_for_frames;
    int kTimeFramesSize = 20;
    int curTimeFrameIdx = 0;
};

class MyLineEdit : public QSpinBox
{
    Q_OBJECT
public:
    MyLineEdit(QWidget *parent = nullptr);
    void focusInEvent(QFocusEvent *) override;

};

class BindLabel : public QLabel
{
    Q_OBJECT
private:
    bool binded = false;
    QPixmap bind_img, unbind_img;
public:
    BindLabel(QWidget *parent = nullptr);

private:
    void mousePressEvent(QMouseEvent *ev) override;
signals:
    void changeBind(bool isBinded);
};

class PlayButton : public QPushButton
{
    Q_OBJECT
private:
    QPixmap play_img, pause_img;
    bool is_playing = false;
public:
    PlayButton(QWidget *parent=nullptr);

private:
    void onClick();

public slots:
    void Play();
    void Pause();

signals:
    void playing();
    void paused();
};

#endif // MAINWINDOW_H
