#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include "blocker.h"

MainWindow::MainWindow(MazeGenerator *mg, QWidget *parent)
    : QMainWindow(parent)
    , mg(mg)
    , ui(new Ui::MainWindow)
{
    Ui_MainWindow();
    ui->setupUi(this);

    mv = new MazeView(mg, ui->widget);

    n_col = mg->n_col;
    n_row = mg->n_row;

    auto layout = new QVBoxLayout();
    ui->widget->layout()->addWidget(mv);

    connect(ui->gen_btn, SIGNAL(clicked()), mg, SLOT(Generate()));
    connect(ui->n_col_le, &QSpinBox::textChanged, [&](){mg->SetColCount(ui->n_col_le->text().toInt());});
    connect(ui->n_row_le, &QSpinBox::textChanged, [&](){mg->SetRowCount(ui->n_row_le->text().toInt());});
    connect(ui->bind_lbl, &BindLabel::changeBind, mg, &MazeGenerator::SetBindSizes);
    connect(ui->bind_lbl, &BindLabel::changeBind, [&](bool bind){bindSizes = bind;});
    connect(ui->n_col_le, &QSpinBox::textChanged, [&](const QString &str){if (bindSizes) ui->n_row_le->setValue(mg->n_row);});
    connect(ui->n_row_le, &QSpinBox::textChanged, [&](const QString &str){if (bindSizes) ui->n_col_le->setValue(mg->n_col);});
    connect(ui->step_sl, &QSlider::valueChanged, this, &MainWindow::StepChange);
    connect(ui->cur_step_sb, &QSpinBox::textChanged, [&](const QString &str) {StepChange(str.toInt());});
    connect(mg, &MazeGenerator::generated, this, &MainWindow::OnGenerate);
    connect(ui->dfs_rb, &QRadioButton::pressed, [&](){mg->strategy = GenStrategy::DFS;});
    connect(ui->bfs_rb, &QRadioButton::pressed, [&](){mg->strategy = GenStrategy::BFS;});
    connect(ui->rnd_rb, &QRadioButton::pressed, [&](){mg->strategy = GenStrategy::RANDOM;});

    connect(ui->right_btn, &QPushButton::clicked, [&](){StepChange(cur_step+1);});
    connect(ui->most_right_btn, &QPushButton::clicked, [&](){StepChange(mg->steps.size());});
    connect(ui->left_btn, &QPushButton::clicked, [&](){StepChange(cur_step-1);});
    connect(ui->most_left_btn, &QPushButton::clicked, [&](){StepChange(0);});
//    ui->widget->setStyleSheet("background-color: red");

//    connect(ui->n_col_le, &QLineEdit::e)
    ui->gen_btn->setFocus();
    OnGenerate();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mv;
}

void MainWindow::OnGenerate()
{
    int size = mg->steps.size();
    whileBlocking(ui->cur_step_sb)->setRange(0, size);
    whileBlocking(ui->cur_step_sb)->setValue(size);
    whileBlocking(ui->max_steps_lbl)->setText(QString::number(size));

    whileBlocking(ui->step_sl)->setRange(0, size);
    whileBlocking(ui->step_sl)->setValue(size);

    cur_step = size;
    mv->setMaxStep(size);
    mv->setCurStep(size);

    ui->right_btn->setEnabled(false);
    ui->most_right_btn->setEnabled(false);
}

void MainWindow::StepChange(int step)
{
    if (step == 0) {
        ui->left_btn->setEnabled(false);
        ui->most_left_btn->setEnabled(false);
    } else {
        ui->left_btn->setEnabled(true);
        ui->most_left_btn->setEnabled(true);
    }
    if (step == mg->steps.size()) {
        ui->right_btn->setEnabled(false);
        ui->most_right_btn->setEnabled(false);
    } else {
        ui->right_btn->setEnabled(true);
        ui->most_right_btn->setEnabled(true);
    }
    whileBlocking(ui->cur_step_sb)->setValue(step);
    whileBlocking(ui->step_sl)->setValue(step);

    cur_step = step;
//    mv->setCurStep(step);

    mv->ToStep(step);
}



MyLineEdit::MyLineEdit(QWidget *parent)
    : QSpinBox(parent)
{
}

void MyLineEdit::focusInEvent(QFocusEvent *)
{
//    selectAll();
//    this->setSelection(0, this->text().size());
    QTimer::singleShot(0,this,SLOT(selectAll()));
//    qDebug() << "focused in" << selectedText();
}

BindLabel::BindLabel(QWidget *parent)
    : QLabel(parent)
{
    bind_img = QPixmap(":/assets/icons/chain_color.png").scaled(16,16,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    unbind_img = QPixmap(":/assets/icons/chain_black.png").scaled(16,16,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    setPixmap(unbind_img);
}

void BindLabel::mousePressEvent(QMouseEvent *ev)
{
    binded = !binded;
    if (binded) {
        this->setPixmap(bind_img);
    } else {
        this->setPixmap(unbind_img);
    }
    emit changeBind(binded);
}
