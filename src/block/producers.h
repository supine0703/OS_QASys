#ifndef PRODUCERS_H
#define PRODUCERS_H

#include <QObject>
#include <QQueue>

class BuffQue;
class ShowLabel;
class IncProducer;

class Producers : public QObject
{
    Q_OBJECT
public:
    explicit Producers(int num, BuffQue *buffer, int which, QWidget *parent = nullptr);

    void MoveTo(const QPoint& pos);
    void Start();
    void Stop();

private:
    BuffQue *bufque;
    QVector<IncProducer*> ers;
    QVector<ShowLabel*> wayLs;
    QQueue<int> productQue;
    bool stpFlg = true;

signals:
    void start_produce();
    void try_send_to_buffer();
};

#endif // PRODUCERS_H
