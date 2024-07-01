#ifndef BUFFQUE_H
#define BUFFQUE_H

#include <QObject>
#include <QMutex>
#include "models/showlabel.h"

class BuffQue : public QObject
{
    Q_OBJECT
public:
    explicit BuffQue(int queSize, QWidget *parent = nullptr);
    bool Push(ShowLabel *page);
    bool Pop(ShowLabel *&page);
    void MoveTo(const QPoint& pos);

private:
    QLabel *back;           // background.
    QPoint basicPos;        // where buffer in widget.
    QVector<ShowLabel*> buffer;
    QMutex mutex;           // let push or pop one-off finish.
    int head = 0, tail = 0; // head output, tail input.
    int count = 0;          // count in buffer.
    bool ibusy = false, obusy = false; // is busy before showing animation finished.
    bool stpFlg = true;

public:
    static void SetSpeed(int rate);
private:
    static float speedRate;

signals:
    void push_finished();
    void pop_finished();
    void is_not_full();
    void is_not_empty();
};

#endif // BUFFQUE_H
