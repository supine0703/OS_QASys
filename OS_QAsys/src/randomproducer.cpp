#include "randomproducer.h"
#include "buffque.h"
#include <QRandomGenerator>

int RandomProducer::numGenerator = 0;
RandomProducer::RandomProducer(QWidget *parent)
    : QObject{parent}
    , name(new ShowLabel("Producer" + QString::number(++numGenerator), parent))
{
    Q_ASSERT(parent != nullptr);
    name->resize(160, 40);
}

void RandomProducer::MoveTo(const QPoint &pos)
{
    name->move(pos);
    producePos = name->pos() + QPoint(180, 0);
    if (instruction != nullptr)
        instruction->move(producePos);
}

void RandomProducer::Produce(QWidget *parent)
{
    Q_ASSERT(parent != nullptr);
    if (have)
        return;
    have = true;
    instruction = new ShowLabel(parent);
    instruction->move(producePos);
    // produce once need average of 8s.
    instruction->ShowLoad(QRandomGenerator::global()->bounded(1000, 15000));
    instruction->show();
    connect(instruction, &ShowLabel::animation_finished, this, [this]() {
        instruction->disconnect(this);
        instruction->raise();
        instruction->SetHexText(this->Random());
        emit produce_finished();
    });
}

bool RandomProducer::SendInc(BuffQue *buffer)
{
    Q_ASSERT(buffer != nullptr);
    if (!buffer->Push(instruction))
        return false;
    have = false;
    instruction = nullptr;
    emit send_inc_finished();
    return true;
}

void RandomProducer::ReSet()
{
    numGenerator = 0;
}

int RandomProducer::Random()
{
    static QMutex metux;
    static int last, count = -1;
    metux.lock();
    count++;
    int inc;
    switch (count % 6)
    { // according to the principle od locality.
    case 0: // 0 ~ 1024
        inc = QRandomGenerator::global()->bounded(INC_NUM);
        break;
    case 2: // random generation of previous address.
        inc = QRandomGenerator::global()->bounded(last + 1);
        break;
    case 4: // random generation of post address.
        inc = last + QRandomGenerator::global()->bounded(INC_NUM - last);
        break;
    default: // generate next address.
        inc = (last + 1) & (INC_NUM - 1);
    }
    last = inc;
    metux.unlock();
    return inc;
}
