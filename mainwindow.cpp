#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>

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

    connect(ui->gen_btn, SIGNAL(clicked()), mg, SLOT(Generate()));
    connect(ui->n_col_le, &QLineEdit::textChanged, [&](){mg->SetColCount(ui->n_col_le->text().toInt());});
    connect(ui->n_row_le, &QLineEdit::textChanged, [&](){mg->SetRowCount(ui->n_row_le->text().toInt());});
//    ui->widget->setStyleSheet("background-color: red");

//    connect(ui->n_col_le, &QLineEdit::e)
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mv;
}



MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void MyLineEdit::focusInEvent(QFocusEvent *)
{
//    selectAll();
//    this->setSelection(0, this->text().size());
    QTimer::singleShot(0,this,SLOT(selectAll()));
//    qDebug() << "focused in" << selectedText();
}
