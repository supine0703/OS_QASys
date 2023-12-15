#ifndef BUFFQUE_H
#define BUFFQUE_H

#include <QObject>
#include <QMutex>
#include "showlabel.h"

class BuffQue : public QObject
{
    Q_OBJECT
    BuffQue(const BuffQue&) = delete;
    BuffQue& operator=(const BuffQue&) = delete;
public:
    explicit BuffQue(int queSize, QWidget *parent=nullptr);
    bool Push(ShowLabel *page);
    bool Pop(ShowLabel *&page);
    void MoveTo(const QPoint& pos);

private:
    QLabel *back;
    QPoint basicPos;
    QVector<ShowLabel*> buffer;
    int head = 0, tail = 0; // head output, tail input
    int count = 0, mvCount = 0;
    bool ibusy = false, obusy = false;
    QMutex mutex;

signals:
    void push_finished();
    void pop_finished();
    void is_not_full();
};

#endif // BUFFQUE_H
