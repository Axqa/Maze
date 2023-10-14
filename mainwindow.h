#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mazegenerator.h"
#include "mazeview.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MazeGenerator *mg, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnGenerate();
    void StepChange(int step);
private slots:
    void on_most_left_btn_clicked();

private:
    MazeGenerator *mg;
    MazeView *mv;
    Ui::MainWindow *ui;
    bool bindSizes = false;
    int n_col, n_row;

    int cur_step = 0;
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

#endif // MAINWINDOW_H
