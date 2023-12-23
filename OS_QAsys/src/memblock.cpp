#include "memblock.h"
#include "recordwidget.h"
#include <QRandomGenerator>

int MemBlock::numGenerator = 0;
MemBlock::MemBlock(QWidget *parent)
    : QObject{parent}
    , blockNum(new ShowLabel(QString::number(numGenerator++), parent))
    , memBlock(new ShowLabel(parent))
    , addBlock(new ShowLabel(parent))
{
    this->Move(0, 0);
}

void MemBlock::MarkRed()
{
    blockNum->MarkRed();
    memBlock->MarkRed();
    addBlock->MarkRed();
}

void MemBlock::UnMark()
{
    blockNum->UnMark();
    memBlock->UnMark();
    addBlock->UnMark();
}

void MemBlock::MoveTo(const QPoint &pos)
{
    blockNum->move(pos);
    memBlock->move(pos + QPoint(78, 0));
    addBlock->move(pos + QPoint(156, 0));
}

void MemBlock::Move(int x, int y)
{
    QPoint pos = blockNum->pos() + QPoint(x, y);
    this->MoveTo(pos);
}

bool MemBlock::ReplacePage(ShowLabel *page)
{
    if (!
        page->ShowMoveTo(
            this->memBlock->pos(),
            QRandomGenerator::global()->bounded(900,2100) * spedRate
        )
    ) return false;
    connect(page, &ShowLabel::animation_finished, this, [this, page]() {
        page->disconnect(this);
        this->memBlock->deleteLater();
        this->memBlock = page;
        int inc;
        RECD() >> inc; // consume inc
        page->SetHexText(inc >> PAGE_INC_BIT); // Inc to Page.
        emit block_replace_finished();
    });
    return true;
}

int MemBlock::MemValue()
{
    return memBlock->text().isEmpty() ? MEM_NULL : memBlock->text().toInt(nullptr, 16);
}

void MemBlock::SetAddition(int info)
{
    addBlock->setText(info != MEM_NULL ? QString::number(info) : "NOP");
}

float MemBlock::spedRate = 1;
void MemBlock::SetSpeed(int rate)
{
    spedRate = 0.4 + 0.016 * rate;
}

void MemBlock::ReSet()
{
    numGenerator = 0;
}
