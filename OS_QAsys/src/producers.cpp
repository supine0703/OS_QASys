#include "producers.h"
#include "incproducer.h"
#include "buffque.h"
#include "myenum.h"

Producers::Producers(int num, BuffQue *buffer, int which, QWidget *parent)
    : QObject{parent}
    , ers(QVector<IncProducer*>(num))
    , wayLs(QVector<ShowLabel*>(num))
    , bufque(buffer)
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(0 < num  && num <= 10);
    Q_ASSERT(buffer != nullptr);

    for (int i = 0; i < num; i++)
    {
        switch (which)
        {
        case MyE::Random: // Random.
            wayLs[i] = new ShowLabel("Random", parent);
            ers[i] = new RandomInc(parent);
            break;
        case MyE::RdFile: // RdFile.
            wayLs[i] = new ShowLabel("RdFile", parent);
            ers[i] = new RdFileInc(parent);
            break;
        }
        wayLs[i]->MarkPurple();
        wayLs[i]->move(QPoint(180, 80 * i));
        ers[i]->MoveTo(QPoint(0, 80 * i));
        connect(this, &Producers::start_produce, this, [this, i]() {
            if (!stpFlg) {
                ers[i]->Produce(static_cast<QWidget*>(this->parent()));
            }
        });
        connect(ers[i], &IncProducer::produce_finished, this, [this, i]() {
            productQue.enqueue(i);
            emit try_send_to_buffer();
        });
        connect(ers[i], &IncProducer::send_inc_finished, this, &Producers::start_produce);
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
    for (int i = 0, end = ers.size(); i < end; i++)
    {
        wayLs[i]->move(pos + QPoint(180, 80 * i));
        ers[i]->MoveTo(pos + QPoint(0, 80 * i));
    }
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
