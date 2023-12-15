#include "memblock.h"
#include <QRandomGenerator>

float MemBlock::spedRate = 1;
int MemBlock::numGenerator = 0;

MemBlock::MemBlock(QWidget *parent)
    : QObject{parent}
    , blockNum(new ShowLabel(parent))
    , memBlock(new ShowLabel(parent))
    , addBlock(new ShowLabel(parent))
{
    blockNum->setText(QString::number(numGenerator++));
    this->Move(0, 0);
}

MemBlock::~MemBlock()
{
    delete blockNum;
    delete memBlock;
    delete addBlock;
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

void MemBlock::SetParent(QWidget *parent)
{
    blockNum->setParent(parent);
    memBlock->setParent(parent);
    addBlock->setParent(parent);
}

bool MemBlock::Replacement(ShowLabel *page)
{
    if (!page->ShowMoveTo(
            this->memBlock->pos(),
            spedRate * QRandomGenerator::global()->bounded(500,1500))
        )
        return false;
    connect(page, &ShowLabel::animation_finished, this, [this, page]() {
        this->memBlock->deleteLater();
        this->memBlock = page;
        page->SetHexText(page->text().toInt(nullptr, 16) >> 4); // Inc to Page
        page->disconnect(this);
//        AddBlockUpdate();
        emit replace_finished();
    });
    return true;
}

void MemBlock::AddBlockUpdate()
{
    if (uab)
        uab->Update();
}

void MemBlock::ReSet()
{
    numGenerator = 0;
}
