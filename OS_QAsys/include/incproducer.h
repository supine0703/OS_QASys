#ifndef INCPRODUCER_H
#define INCPRODUCER_H

#include <QObject>
#include "showlabel.h"
class BuffQue;

class IncProducer : public QObject
{
    Q_OBJECT
protected:
    virtual int OutputInc() = 0;

public:
    explicit IncProducer(QWidget *parent = nullptr);
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
    static int numGenerator;

signals:
    void send_inc_finished();
    void produce_finished();
};

// -----------------------------------------------------------------------------

class RandomInc : public IncProducer
{
protected:
    int OutputInc() override;

public:
    explicit RandomInc(QWidget *parent = nullptr);
};

// -----------------------------------------------------------------------------

class RdFileInc : public IncProducer
{
protected:
    int OutputInc() override;

public:
    explicit RdFileInc(QWidget *parent = nullptr);
};

#endif // INCPRODUCER_H
