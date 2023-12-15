#ifndef PRODUCERS_H
#define PRODUCERS_H

#include <QObject>
#include <QQueue>

class QThread;
class BuffQue;
class RandomProducer;

class Producers : public QObject
{
    Q_OBJECT
    Producers(const Producers&) = delete;
    Producers& operator=(const Producers&) = delete;
public:
    explicit Producers(int num, BuffQue *buffer, QWidget *parent = nullptr);
//    ~Producers();

    void MoveTo(const QPoint& pos);
    void Start();
    void Stop();

private:
    QVector<RandomProducer*> ers;
    QQueue<int> que;

signals:
    void start_produce();
    void pro();
};

#endif // PRODUCERS_H
