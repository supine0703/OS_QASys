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

#define GLOBAL_SPEED 1
#define BUFFQUE_SPEED 49
#define CONSUME_SPEED 35

ShowWidget::ShowWidget(int which, int bufSize, int memSize, QWidget *parent)
    : QWidget{parent}
    , view1Size(((memSize >> 4) + 2 + !(!(memSize & 0xf))) * 250, 830)
    , view1(new QWidget(this))
{
    this->resize(view1Size);

    // number reset
    MemBlock::ReSet();
    RandomProducer::ReSet();

    // new space for modules to show
    QPushButton *pDo = new QPushButton("Producers DO", view1);
    QPushButton *cDo = new QPushButton("Consumers DO", view1);
    QSlider *globalSpeed = new QSlider(view1);
    QSlider *buffQueSpeed = new QSlider(view1);
    QSlider *consumeSpeed = new QSlider(view1);
    QLabel *globalSpeedValue = new QLabel("0", view1);
    QLabel *buffQueSpeedValue = new QLabel("0", view1);
    QLabel *consumeSpeedValue = new QLabel("0", view1);
    BuffQue *buffer = new BuffQue(bufSize, view1);
    Producers *prders = new Producers(4, buffer, view1);
    Consumers *csmers;
    switch (which)
    {
    case 0: {
        csmers = new FIFOConsumer(memSize, buffer, view1);
        break;
    }
    case 1: {
        csmers = new LRUConsumer(memSize, buffer, view1);
        break;
    }
    case 2: {
        csmers = new LFUConsumer(memSize, buffer, view1);
        break;
    }
    case 3: {
        csmers = new NRUConsumer(memSize, buffer, view1);
        break;
    }
    default: {
        Q_ASSERT_X(false, "class ShowWidget", "Consumers new false");
        csmers = new FIFOConsumer(memSize, buffer, view1); // to resist the warning
    }
    }

    // modules move to
    pDo->move(QPoint(10, 10) + basic);
    cDo->move(QPoint(10, 50) + basic);
    globalSpeed->move(QPoint(185, 40) + basic);
    buffQueSpeed->move(QPoint(220, 40) + basic);
    consumeSpeed->move(QPoint(255, 40) + basic);
    globalSpeedValue->move(QPoint(185, 10) + basic);
    buffQueSpeedValue->move(QPoint(220, 10) + basic);
    consumeSpeedValue->move(QPoint(255, 10) + basic);
    buffer->MoveTo(QPoint(335, 10) + basic);
    prders->MoveTo(QPoint(10, 132) + basic);
    csmers->MemoryMoveTo(QPoint(500, 20) + basic);
    csmers->MemDataMoveTo(QPoint(10, 474) + basic);

    // modules setting
    pDo->setCheckable(true);
    cDo->setCheckable(true);

    auto initSpeedSlider = [this](QSlider *slider, QLabel *valueLabel) {
        slider->resize(25, 80);
        slider->setInvertedAppearance(true);
        valueLabel->resize(25, 20);
        valueLabel->setAlignment(Qt::AlignCenter);
    };

    initSpeedSlider(globalSpeed, globalSpeedValue);
    initSpeedSlider(buffQueSpeed, buffQueSpeedValue);
    initSpeedSlider(consumeSpeed, consumeSpeedValue);

    // connect signals
    connect(pDo, &QPushButton::clicked, this, [prders](bool key) {
        if (key)
            prders->Start();
        else
            prders->Stop();
    });
    connect(cDo, &QPushButton::clicked, this, [csmers](bool key) {
        if (key)
            csmers->Start();
        else
            csmers->Stop();
    });
    connect(globalSpeed, &QSlider::valueChanged, this, [globalSpeedValue](int value) {
        ShowLabel::SetSpeed(value);
        globalSpeedValue->setText(QString::number(99 - value));
    });
    connect(buffQueSpeed, &QSlider::valueChanged, this, [buffQueSpeedValue](int value) {
        BuffQue::SetSpeed(value);
        buffQueSpeedValue->setText(QString::number(99 - value));
    });
    connect(consumeSpeed, &QSlider::valueChanged, this, [consumeSpeedValue](int value) {
        MemBlock::SetSpeed(value);
        consumeSpeedValue->setText(QString::number(99 - value));
    });
    // something need do after connect
    globalSpeed->setValue(GLOBAL_SPEED);
    consumeSpeed->setValue(CONSUME_SPEED);
    buffQueSpeed->setValue(BUFFQUE_SPEED);
}

void ShowWidget::resizeEvent(QResizeEvent *event)
{
    view1->move(
        (this->width() - view1Size.width()) >> 1,
        (this->height() - view1Size.height()) >> 1);
}


