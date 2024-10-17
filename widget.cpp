#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());	//固定大小
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint ); //隐藏最大化按钮
    //load music
    //load multimedia
    //link mediaPlayer to audioOutput
    audioOutput = new QAudioOutput(this);

    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    //ui->volumeSlider->setValue(50);
    //初始隐藏volumeSlider
    ui->volumeSlider->hide();
    //get the duration of the current media
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this,[=](qint64 duration)
            {
                ui->totalLable->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60,2,10,QChar('0')));
                ui->timeSlider->setRange(0,duration);
    });

    //get the position of the current media
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this,[=](qint64 position)
            {
                ui->curLable->setText(QString("%1:%2").arg(position/1000/60,2,10,QChar('0')).arg(position/1000%60,2,10,QChar('0')));
                ui->timeSlider->setValue(position);
            });

    //to change the progress of the music via timeSlider
    connect(ui->timeSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    connect(ui->volumeSlider,&CustomSlider::costomSliderClicked,this,&Widget::slider_volume_changed);
    connect(ui->volumeSlider,&CustomSlider::sliderMoved,this,&Widget::slider_volume_changed);


}

Widget::~Widget()
{
    delete ui;
}

//打开文件夹
void Widget::on_folder_clicked()
{
    //open folder to select the directory where the music is located.
    auto path = QFileDialog::getExistingDirectory(this,"请选择音乐所在目录","../../music");
    qInfo()<<path;
    //get all the mp3 and wav files
    QDir dir(path);
    auto musicList = dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList;
    //show the musicList in the listWidget.
    ui->listWidget->addItems(musicList);

    ui->listWidget->setCurrentRow(0);
    //save
    for(auto file : musicList)
    {
        playList.append(QUrl::fromLocalFile(path + "/" + file));
    }
    qInfo() << playList;
}

//播放和暂停
void Widget::on_play_clicked()
{
    if(playList.empty())
    {
        return;
    }
    switch(mediaPlayer->playbackState())
    {
    case QMediaPlayer::PlaybackState::StoppedState: //stopped state
    {
        //如果无音乐播放，则播放当前行的音乐
        //play the selected music
        //get current row
        curRow = ui->listWidget->currentRow();
        //play the current music
        mediaPlayer->setSource(playList[curRow]);
        ui->play->setStyleSheet("QPushButton{icon: url(:/asserts/btn_playback_pause.png)}");
        mediaPlayer->play();
        break;
    }
    case QMediaPlayer::PlaybackState::PlayingState: //playing state
    {
        //如果正在播放，则暂停播放
        ui->play->setStyleSheet("QPushButton{icon: url(:/asserts/btn_playback_play.png)}");
        mediaPlayer->pause();
        break;
    }
    case QMediaPlayer::PlaybackState::PausedState:  //paused state
    {
        //如果是暂停状态，则继续播放音乐
        ui->play->setStyleSheet("QPushButton{icon: url(:/asserts/btn_playback_pause.png)}");
        mediaPlayer->play();
        break;
    }
    }
}

//下一首
void Widget::on_next_clicked()
{
    //to select the next row of the listWidget
    ++curRow;
    if(curRow >= playList.size())   //to avoid array bound
    {
        curRow = 0;
    }
    ui->listWidget->setCurrentRow(curRow);
    mediaPlayer->setSource(playList[curRow]);
    mediaPlayer->play();
}

//上一首
void Widget::on_previous_clicked()
{
    //to select the previous row of the listWidget
    --curRow;
    if(curRow < 0)   //to avoid array bound
    {
        curRow = playList.size()-1;
    }
    ui->listWidget->setCurrentRow(curRow);
    mediaPlayer->setSource(playList[curRow]);
    mediaPlayer->play();
}

void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curRow = index.row();
    mediaPlayer->setSource(playList[curRow]);
    mediaPlayer->play();
}



//音量控制Slider的槽函数
void Widget::slider_volume_changed()
{
    audioOutput->setVolume((float)ui->volumeSlider->value()/100);
}

void Widget::on_volume_clicked()
{
    if(ui->volumeSlider->isHidden()){
        ui->volumeSlider->setHidden(false);
    }
    else{
        ui->volumeSlider->setHidden(true);
    }
}



