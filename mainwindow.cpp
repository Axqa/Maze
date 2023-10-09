#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MazeGenerator *mg, QWidget *parent)
    : QMainWindow(parent)
    , mg(mg)
    , ui(new Ui::MainWindow)
{
    Ui_MainWindow();
    ui->setupUi(this);

    mv = new MazeView(mg, ui->widget);

    auto layout = new QVBoxLayout();
    ui->widget->layout()->addWidget(mv);

//    ui->widget->setStyleSheet("background-color: red");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mv;
}

