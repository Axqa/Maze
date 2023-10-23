#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include "blocker.h"
#include <QElapsedTimer>

MainWindow::MainWindow(MazeGenerator *mg, QWidget *parent)
    : QMainWindow(parent)
    , mg(mg)
    , ui(new Ui::MainWindow)
{
    Ui_MainWindow();
    ui->setupUi(this);
    pf = new AStarFinder(mg);
//    pf = new LinePather(mg);
//    pf = new PathFinder(mg);

    mv = new MazeView(mg, pf, ui->widget);


    n_col = mg->n_col;
    n_row = mg->n_row;

    ui->widget->layout()->addWidget(mv);

    connect(ui->gen_btn, SIGNAL(clicked()), mg, SLOT(Generate()));
    connect(ui->n_col_le, &QSpinBox::textChanged, [&](){mg->SetColCount(ui->n_col_le->text().toInt());});
    connect(ui->n_row_le, &QSpinBox::textChanged, [&](){mg->SetRowCount(ui->n_row_le->text().toInt());});
    connect(ui->bind_lbl, &BindLabel::changeBind, mg, &MazeGenerator::SetBindSizes);
    connect(ui->bind_lbl, &BindLabel::changeBind, [&](bool bind){bindSizes = bind;});
    connect(ui->n_col_le, &QSpinBox::textChanged, [&](){if (bindSizes) ui->n_row_le->setValue(mg->n_row);});
    connect(ui->n_row_le, &QSpinBox::textChanged, [&](){if (bindSizes) ui->n_col_le->setValue(mg->n_col);});
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

    connect(ui->play_btn, &PlayButton::playing, this, &MainWindow::StartMazeTimer);
    connect(ui->play_btn, &PlayButton::paused, this, &MainWindow::PauseTimer);
    connect(&maze_timer, &QTimer::timeout, this, &MainWindow::OnMazeTimerStep);

    connect(ui->play_speed_sb, &QSpinBox::textChanged, [&](const QString &str){timer_msec = str.toInt();});

    connect(ui->to_find_btn, &QPushButton::pressed, this, &MainWindow::ToFindPage);
    connect(ui->to_gen_btn, &QPushButton::pressed, this, &MainWindow::ToGenPage);

    connect(mv, &MazeView::pathCellClicked, pf, &PathFinder::SetPoint);
    connect(pf, &PathFinder::changed, mv, &MazeView::UpdatePathMode);

    connect(mg, &MazeGenerator::generated, pf, &PathFinder::ResetPoints);
    connect(ui->clear_btn, &QPushButton::pressed, this, &MainWindow::ResetMaze);

    connect(mv, &MazeView::sceneTimeChanged, [&](qint64 time){ui->scene_time_lbl->setText(QString::number(time/1000000.0,'f',2));});
    connect(pf, &PathFinder::pathTimeChanged, [&](qint64 time){ui->find_time_lbl->setText(QString::number(time/1000000.0,'f',2));});

    maze_timer.setSingleShot(true);
//    ui->widget->setStyleSheet("background-color: red");

//    connect(ui->n_col_le, &QLineEdit::e)
    ui->gen_btn->setFocus();
    OnGenerate();
    fps_timer.start();

    time_for_frames = std::vector<qint64>(kTimeFramesSize, 0);
    ui->scene_time_lbl->setText(QString::number(mv->scene_time/1000000.0,'f',2));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mv;
    delete pf;
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    QMainWindow::paintEvent(event);

    qint64 elapsedTime = fps_timer.restart(); //.nsecsElapsed();
    if (elapsedTime) {
        QString res;
        time_for_frames[curTimeFrameIdx] = elapsedTime;
        for (int i = 0; i < kTimeFramesSize; ++i) {
            res += QString::number(time_for_frames[(i + curTimeFrameIdx) % kTimeFramesSize]);
            res += " ";
        }
        curTimeFrameIdx = (curTimeFrameIdx + 1) % kTimeFramesSize;
        statusBar()->showMessage(res);
    }

}

void MainWindow::StartMazeTimer()
{
    if (cur_step == mg->steps.size()) {
        StepChange(0);
    }

    maze_timer.start(timer_msec);
}

void MainWindow::ResetMaze()
{
    mg->Reset();
    mv->Reload();
    OnGenerate();
    StepChange(0);
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

    ui->gen_time_lbl->setText(QString::number(mg->gen_time / 1000000.0,'f',2));
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

void MainWindow::OnMazeTimerStep()
{
    if (cur_step < mg->steps.size()) {
        StepChange(cur_step + 1);
    }

    if (cur_step >= mg->steps.size()) {
        whileBlocking(ui->play_btn)->Pause();
        return;
    }

    maze_timer.start(timer_msec);
}

void MainWindow::PauseTimer()
{
    if (maze_timer.isActive()) {
        maze_timer.stop();
    }
}

void MainWindow::ToFindPage()
{
    mode = Mode::PATH_FINDER;
    mv->setMode(mode);
    ui->right_stack->setCurrentIndex(1);
    ui->left_stack->setCurrentIndex(1);
}

void MainWindow::ToGenPage()
{
    mode = Mode::GENERATE;
    mv->setMode(mode);
    ui->right_stack->setCurrentIndex(0);
    ui->left_stack->setCurrentIndex(0);

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

PlayButton::PlayButton(QWidget *parent)
{
    play_img = QPixmap(":/assets/icons/play.png");
    pause_img = QPixmap(":/assets/icons/pause.png");

    connect(this, &QPushButton::pressed, this, &PlayButton::onClick);
}

void PlayButton::onClick()
{
    if (is_playing) {
        Pause();
    } else {
        Play();
    }
}

void PlayButton::Play()
{
    is_playing = true;
    setIcon(QIcon(pause_img));
    emit playing();
}

void PlayButton::Pause()
{
    is_playing = false;
    setIcon(QIcon(play_img));
    emit paused();

}
