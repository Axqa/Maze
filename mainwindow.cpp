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

    connect(ui->gen_bt, SIGNAL(clicked()), mg, SLOT(Generate()));
    connect(ui->n_col_le, &QLineEdit::editingFinished, mg, [&](){mg->SetColCount(ui->n_col_le->text().toInt());});
    connect(ui->n_row_le, &QLineEdit::editingFinished, mg, [&](){mg->SetRowCount(ui->n_row_le->text().toInt());});
//    ui->widget->setStyleSheet("background-color: red");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mv;
}

