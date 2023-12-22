#ifndef RANDOMPRODUCER_H
#define RANDOMPRODUCER_H

#include <QObject>
#include "showlabel.h"
class BuffQue;

class RandomProducer : public QObject
{
    Q_OBJECT
public:
    explicit RandomProducer(QWidget *parent = nullptr);
    void MoveTo(const QPoint& pos);
    void Produce(QWidget *parent);
    bool SendInc(BuffQue *buffer); // send the instruction to buffer.

private:
    ShowLabel *name; // whihc this RandomProducer.
    ShowLabel *instruction = nullptr;
    QPoint producePos; // where produce to.
    bool have = false; // can't do.

public:
    static void ReSet(); // numGenerator reset 0.
private:
    static int Random();
    static int numGenerator;

signals:
    void send_inc_finished();
    void produce_finished();
};

#endif // RANDOMPRODUCER_H
