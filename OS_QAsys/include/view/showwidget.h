#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include "buffque.h"
#include <QWidget>

#include <QThread>

class Consumers;
class Producers;
class QPushButton;
class QSlider;

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(int which, int bufSize, int memSize, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void MoveTo();

    QSize base;
    QPoint basic;
    QPushButton *s1;
    QPushButton *s2;
    QSlider *globalSpeed;
    QSlider *consumeSpeed;

    BuffQue *que;
    Consumers *csmers;
    Producers *prders;

signals:

};

#endif // SHOWWIDGET_H
