#include "randomproducer.h"
#include "buffque.h"
#include <QRandomGenerator>

int RandomProducer::numGenerator = 0;

RandomProducer::RandomProducer(QWidget *parent)
    : QObject{parent}
    , name(new ShowLabel("Producer" + QString::number(++numGenerator), parent))
{
    name->resize(160, 40);
}

void RandomProducer::MoveTo(const QPoint &pos)
{
    name->move(pos);
    if (instruction != nullptr)
        instruction->move(name->pos() + QPoint(180, 0));
}

void RandomProducer::Random()
{
    static int last, count = -1;
    count++;
    int inc;
    switch (count % 6)
    {
    case 0: // 0 ~ 1024
        inc = QRandomGenerator::global()->bounded(INC_NUM);
        break;
    case 2: // 前地址随机取
        inc = QRandomGenerator::global()->bounded(last + 1);
        break;
    case 4: // 后地址随机取
        inc = last + QRandomGenerator::global()->bounded(INC_NUM - last);
        break;
    default: // 顺序取下一指令
        inc = (last + 1) & (INC_NUM - 1);
    }
    last = inc;
    instruction->SetHexText(inc);
}

void RandomProducer::Produce(QWidget *parent)
{
    if (have)
        return;
    have = true;
    instruction = new ShowLabel(parent);
    instruction->move(name->pos() + QPoint(180, 0));
    instruction->ShowLoad(QRandomGenerator::global()->bounded(1000, 7000));
    instruction->show();
    connect(instruction, &ShowLabel::animation_finished, this, [this]() {
        instruction->disconnect(this);
        emit produce_finished();
    });
}

bool RandomProducer::SendInc(BuffQue *buffer)
{
    if (!buffer->Push(instruction))
        return false;
    have = false;
    instruction = nullptr;
    if (!stpFlg)
        emit send_inc_finished();
    return true;
}

void RandomProducer::ReSet()
{
    numGenerator = 0;
}
