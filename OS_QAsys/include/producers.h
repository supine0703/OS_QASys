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
    BuffQue *bufque;
    QVector<RandomProducer*> ers;
    QQueue<int> productQue;
    bool stpFlg = true;

signals:
    void start_produce();
    void try_send_to_buffer();
};

#endif // PRODUCERS_H
