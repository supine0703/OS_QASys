#include "producers.h"
#include "randomproducer.h"
#include "buffque.h"
#include <QThread>

Producers::Producers(int num, BuffQue *buffer, QWidget *parent)
    : QObject{parent}
    , ers(QVector<RandomProducer*>(num))
    , bufque(buffer)
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(0 < num  && num <= 10);
    Q_ASSERT(buffer != nullptr);
    for (int i = 0; i < num; i++)
    {
        ers[i] = new RandomProducer(parent);
        ers[i]->MoveTo(QPoint(0, 80 * i));
//        connect(this, &Producers::start_produce, ers[i], &RandomProducer::Start);
        connect(this, &Producers::start_produce, this, [this, i]() {
            if (!stpFlg) {
                ers[i]->Produce(static_cast<QWidget*>(this->parent()));
            }
        });
        connect(ers[i], &RandomProducer::produce_finished, this, [this, i]() {
            productQue.enqueue(i);
            emit try_send_to_buffer();
        });
        connect(ers[i], &RandomProducer::send_inc_finished, this, &Producers::start_produce);
    }
    connect(bufque, &BuffQue::push_finished, this, &Producers::try_send_to_buffer);
    connect(bufque, &BuffQue::is_not_full, this, &Producers::try_send_to_buffer);
    connect(this, &Producers::try_send_to_buffer, this, [this]() {
        if (!productQue.empty()) {
            if (ers[productQue.head()]->SendInc(bufque)) {
                productQue.dequeue();
            }
        }
    });
}

void Producers::MoveTo(const QPoint &pos)
{
    int i = 0;
    for (auto& er : ers)
        er->MoveTo(pos + QPoint(0, 80 * i++));
}

void Producers::Start()
{
    stpFlg = false;
    emit start_produce();
}

void Producers::Stop()
{
    stpFlg = true;
}
