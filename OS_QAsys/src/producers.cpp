#include "producers.h"
#include "randomproducer.h"
#include "buffque.h"
#include <QThread>


Producers::Producers(int num, BuffQue* buffer, QWidget *parent)
    : QObject{parent}
    , ers(QVector<RandomProducer*>(num))
{
    Q_ASSERT(0 < num  && num <= 10);
    for (int i = 0; i < num; i++)
    {
        ers[i] = new RandomProducer(parent);
        ers[i]->MoveTo(QPoint(0, 80 * i));
        connect(this, &Producers::start_produce, ers[i], &RandomProducer::Start);
        connect(this, &Producers::start_produce, this, [this, i, parent]() {
            ers[i]->Produce(parent);
        });
        connect(ers[i], &RandomProducer::send_inc_finished, this, &Producers::start_produce);
        connect(ers[i], &RandomProducer::produce_finished, this, [this, i, buffer]() {
            que.enqueue(i);
            ers[i]->Random();
            if (ers[que.head()]->SendInc(buffer)) {
                que.dequeue();
            }
        });
        connect(buffer, &BuffQue::push_finished, this, [this, buffer]() {
            if (!que.empty()) {
                if (ers[que.head()]->SendInc(buffer))
                    que.dequeue();
            }
        });
        connect(buffer, &BuffQue::is_not_full, this, [this, buffer]() {
            if (!que.empty()) {
                if (ers[que.head()]->SendInc(buffer))
                    que.dequeue();
            }
        });
    }
}

//Producers::~Producers()
//{
//    for (int i = 0, end = ths.size(); i < end; i++)
//    {
//        ths[i]->quit();
//        ths[i]->wait();
//        ths[i]->deleteLater();
//        ers[i]->deleteLater();
//    }
//}

void Producers::MoveTo(const QPoint &pos)
{
    int i = 0;
    for (auto& er : ers)
        er->MoveTo(pos + QPoint(0, 80 * i++));
}

void Producers::Start()
{
    emit start_produce();
}

void Producers::Stop()
{
    for (auto& er : ers)
        er->Stop();
}
