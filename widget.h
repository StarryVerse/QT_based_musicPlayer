#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_folder_clicked();

    void on_play_clicked();

    void on_next_clicked();

    void on_previous_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);


    //void on_volumeSlider_valueChanged(int value);
    void slider_volume_changed();

    void on_volume_clicked();

    void on_volumeSlider_sliderMoved(int position);

private:
    Ui::Widget *ui;
    QList<QUrl> playList;   //playList
    QAudioOutput* audioOutput;
    QMediaPlayer* mediaPlayer;
    int curRow;
};
#endif // WIDGET_H
