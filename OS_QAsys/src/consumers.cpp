#include "consumers.h"
#include "buffque.h"
#include "memory.h"

Consumers::Consumers(int size, BuffQue *buffer, QWidget *parent)
    : QObject{parent}
    , mem(new Memory(size, parent))
{
    Q_ASSERT(parent != nullptr);
    mem->MarkRed(0);
    connect(this, &Consumers::start_consumer, mem, [this, buffer]() {
        if (!stpFlg && !mem->Busy()) {
            buffer->Pop(label);
        }
    });
    connect(buffer, &BuffQue::pop_finished, this, [this]() {
        mem->data->total++;
        if (MissingPage()) { // replace page which is marked if missing page
            if (mem->Replace(label)) {
                ConctUpdate();
            } else { // if page is showing other animation
                connect(label, &ShowLabel::animation_finished, this, [this]() {
                    label->disconnect(this);
                    mem->Replace(label);
                    ConctUpdate();
                });
            }
        }
    });
    connect(mem, &Memory::mem_replace_finished, this, &Consumers::start_consumer);
    connect(buffer, &BuffQue::is_not_empty, this, &Consumers::start_consumer);
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

void Consumers::ConctUpdate()
{
    connect(label, &ShowLabel::animation_finished, this, [this]() {
        label->disconnect(this);
        UpdateData();
    });
}

bool Consumers::MissingPage()
{
    for (int i = 0, end = mem->memBlocks.size(); i < end; i++)
    {
        if (mem->memBlocks[i]->memBlock->text() == "")
            break; // if memory is not full
        else if (mem->memBlocks[i]->memBlock->text().toInt(nullptr, 16)
                 == (label->text().toInt(nullptr, 16) >> 4))
        { // replace page_i if not missing page
            label->MarkBlue();
            if (mem->Replace(label, i))
                ConctUpdate();
            else
            { // if page is showing other animation
                connect(label, &ShowLabel::animation_finished, this, [this, i]() {
                    label->disconnect(this);
                    mem->Replace(label, i);
                    ConctUpdate();
                });
            }
            mem->data->hit++;
            hitPage = i;
            return false;
        }
    }
    // missing page
    label->MarkRed();
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



