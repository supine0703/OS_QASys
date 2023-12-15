#include "showwidget.h"
#include "buffque.h"
#include "producers.h"
#include "pagereplace.h"
#include "randomproducer.h"
#include <QPushButton>
#include <QSlider>
#include <QThread>
#include <QEventLoop>
#include <QDialog>

ShowWidget::ShowWidget(int which, int bufSize, int memSize, QWidget *parent)
    : QWidget{parent}
    , basic(QPoint(0, 0))
    , base(((memSize >> 4) + 2 + !(!(memSize & 0xf))) * 250, 870)
{
    this->resize(base);
    MemBlock::ReSet();
    RandomProducer::ReSet();
    que = new BuffQue(bufSize, this);
    prders = new Producers(4, que, this);
    switch (which)
    {
    case 0:
        csmers = new FIFOConsumer(memSize, que, this);
        break;
    case 1:
        csmers = new LRUConsumer(memSize, que, this);
        break;
    case 2:
        csmers = new LFUConsumer(memSize, que, this);
        break;
    case 3:
        csmers = new NRUConsumer(memSize, que, this);
    }

    s1 = new QPushButton("Producers DO", this);
    s2 = new QPushButton("Consumers DO", this);
    s1->setCheckable(true);
    s2->setCheckable(true);
    connect(s1, &QPushButton::clicked, this, [this](bool k) {
        if (k)
            prders->Start();
        else
            prders->Stop();
    });
    connect(s2, &QPushButton::clicked, this, [this](bool k) {
        if (k)
            csmers->Start();
        else
            csmers->Stop();
    });

    globalSpeed = new QSlider(this);
    consumeSpeed = new QSlider(this);
    globalSpeed->resize(25, 100);
    consumeSpeed->resize(25, 100);
    connect(globalSpeed, &QSlider::sliderMoved, &ShowLabel::SetSpeed);
    connect(consumeSpeed, &QSlider::sliderMoved, &MemBlock::SetSpeed);
//    connect(globalSpeed, &QSlider::sliderMoved, this, [](int v) {qDebug() << v << "g";});
//    connect(consumeSpeed, &QSlider::sliderMoved, this, [](int v) {qDebug() << v << "c";});
    globalSpeed->setValue(5);
    consumeSpeed->setValue(45);
    globalSpeed->setInvertedAppearance(true);
    consumeSpeed->setInvertedAppearance(true);
    ShowLabel::SetSpeed(globalSpeed->value());
    MemBlock::SetSpeed(consumeSpeed->value());
}

void ShowWidget::resizeEvent(QResizeEvent *event)
{
    basic = QPoint((this->width() - base.width()), (this->height() - base.height())) / 2;
    MoveTo();
}

void ShowWidget::MoveTo()
{
    que->MoveTo(QPoint(335, 30) + basic);
    prders->MoveTo(QPoint(10, 152) + basic);
    csmers->MemoryMoveTo(QPoint(500, 40) + basic);
    csmers->MemDataMoveTo(QPoint(10, 494) + basic);
    s1->move(QPoint(10, 30) + basic);
    s2->move(QPoint(10, 70) + basic);
    globalSpeed->move(QPoint(215, 30) + basic);
    consumeSpeed->move(QPoint(250, 30) + basic);
}

