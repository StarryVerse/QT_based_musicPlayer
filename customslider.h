#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H
#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

//自定义滑动条以实现单击后的任意时间点的跳转
class CustomSlider : public QSlider
{
    Q_OBJECT
public:
    CustomSlider(QWidget *parent = 0) : QSlider(parent)
    {
    }
protected:
    void mousePressEvent(QMouseEvent *ev);//重写QSlider的mousePressEvent事件
signals:
    void costomSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
};

#endif // CUSTOMSLIDER_H
