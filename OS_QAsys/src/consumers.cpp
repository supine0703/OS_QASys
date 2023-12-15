#include "consumers.h"
#include "buffque.h"
#include "memory.h"

Consumers::Consumers(int size, BuffQue *buffer, QWidget *parent)
    : QObject{parent}
    , mem(new Memory(size, parent))
{
    mem->MarkRed(0);
    connect(this, &Consumers::start_consumer, mem, [this, buffer]() {
        if (stpFlg)
            return;
        ShowLabel *label;
        if (!mem->Busy() && buffer->Pop(label)) {
            mem->data->total++;
            if (MissingPage(label) && !mem->Replacement(label)) {
                connect(label, &ShowLabel::animation_finished, this, [this, label]() {
                    mem->Replacement(label);
                    label->disconnect(this);
                });
            }
            UpdateData();
        } else {
        }
    });
    connect(mem, &Memory::replace_finish, this, &Consumers::start_consumer);
    connect(buffer, &BuffQue::pop_finished, this, &Consumers::start_consumer);
    connect(buffer, &BuffQue::push_finished, this, &Consumers::start_consumer);
}

void Consumers::MemoryMoveTo(const QPoint &pos)
{
    mem->MoveTo(pos);
}

void Consumers::MemDataMoveTo(const QPoint &pos)
{
    mem->MemDataMoveTo(pos);
}

void Consumers::Start()
{
    stpFlg = false;
    emit start_consumer();
}

void Consumers::Stop()
{
    stpFlg = true;
}

bool Consumers::MissingPage(ShowLabel *label)
{
    for (int i = 0, end = mem->memBlocks.size(); i < end; i++)
    {
        if (mem->memBlocks[i]->memBlock->text() == "")
            break;
        else if (mem->memBlocks[i]->memBlock->text().toInt(nullptr, 16)
                 == (label->text().toInt(nullptr, 16) >> 4))
        {
            label->MarkBlue();
            if (!mem->Replacement(label, i)) {
                connect(label, &ShowLabel::animation_finished, this, [this, i, label]() {
                    mem->Replacement(label, i);
                    label->disconnect(this);
                });
            }
            mem->data->hit++;
            hitPage = i;
            return false;
        }
    }
    mem->data->miss++;
    hitPage = -1;
    return true;
}

void Consumers::UpdateData()
{
    mem->data->replace = mem->data->miss - mem->memBlocks.size();
    if (mem->data->replace < 0)
        mem->data->replace = 0;
    mem->data->hitRate = 1.0 * mem->data->hit / mem->data->total;
    mem->data->missRate = 1.0 * mem->data->miss / mem->data->total;
    mem->data->Update();
}



