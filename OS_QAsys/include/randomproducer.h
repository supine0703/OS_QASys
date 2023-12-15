#ifndef RANDOMPRODUCER_H
#define RANDOMPRODUCER_H

#include <QObject>
#include "showlabel.h"
class BuffQue;

class RandomProducer : public QObject
{
    Q_OBJECT
    RandomProducer(const RandomProducer&) = delete;
    RandomProducer& operator=(const RandomProducer&) = delete;
public:
    explicit RandomProducer(QWidget *parent=nullptr);
    void MoveTo(const QPoint& pos);
    void Random();
    void Produce(QWidget *parent=nullptr);
    bool SendInc(BuffQue *buffer);
    void Start() { stpFlg = false; }
    void Stop() { stpFlg = true; }

private:
    ShowLabel *name;

    ShowLabel *instruction = nullptr;
    bool have = false; // can't do
    bool stpFlg = false;

public:
    static void ReSet();
private:
    static int numGenerator;

signals:
    void send_inc_finished();
    void produce_finished();
};

#endif // RANDOMPRODUCER_H
