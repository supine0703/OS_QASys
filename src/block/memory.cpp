#include "memory.h"

#define COL 16

Memory::Memory(int blockNum, QWidget *parent)
    : QObject{parent}
    , data(new MemData(blockNum, parent))
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(4 <= blockNum && blockNum <= 64);
    memBlocks = QVector<MemBlock*>(blockNum);
    for (auto& mb : memBlocks)
    {
        mb = new MemBlock(parent);
        connect(mb, &MemBlock::block_replace_finished, this, [this, mb]() {
            mb->UnMark();
            if (uab)
                uab->Update();
            busy = false;
            emit mem_replace_finished();
        });
    }
    this->MoveTo(QPoint(0, 0));
    this->MemDataMoveTo(QPoint(0, 0));
}

void Memory::MoveTo(const QPoint &pos)
{
    int row = 0, col = 0;
    auto add = [&]() {
        row++;
        if (row == COL)
        {
            row = 0;
            col++;
        }
    };

    for (auto& mb : memBlocks)
    {
        mb->MoveTo(pos + QPoint(col * 250, row * 50));
        add();
    }
}

void Memory::MemDataMoveTo(const QPoint &pos)
{
    data->MoveTo(pos);
}

void Memory::MarkRed(int num)
{
    Q_ASSERT(0 <= num && num < memBlocks.size());
    memBlocks[whoMark = num]->MarkRed();
}

void Memory::UnMark(int num)
{
    Q_ASSERT(0 <= num && num < memBlocks.size());
    memBlocks[num]->UnMark();
    whoMark = -1;
}

bool Memory::Replace(ShowLabel *page, int num)
{
    busy = true;
    if (num == -1)
        num = whoMark;
    Q_ASSERT(num != -1);
    Q_ASSERT(0 <= num && num < memBlocks.size());
    if (!memBlocks[num]->ReplacePage(page))
        return false;
    return true;
}

void Memory::SetInterface(UpdateAddBolck *uab)
{
    this->uab = uab;
}
