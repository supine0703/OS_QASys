#ifndef MEMORY_H
#define MEMORY_H

#include "memblock.h"
#include "memdata.h"

class Memory : public QObject
{
    friend class Consumers;
    friend class FIFOConsumer;
    friend class LRUConsumer;
    friend class LFUConsumer;
    friend class NRUConsumer;
    friend class OPTConsumer;

    Q_OBJECT
    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;
public:
    explicit Memory(int blockNum, QWidget *parent = nullptr);
    ~Memory();
    void MoveTo(const QPoint &pos);
    void MemDataMoveTo(const QPoint &pos);
    void MarkRed(int num);
    void UnMark(int num);
    bool Replace(ShowLabel *page, int num = -1);
    void SetInterface(UpdateAddBolck *uab);
    bool Busy() { return busy; }

private:
    bool busy = false;
    int whoMark = -1;
    QVector<MemBlock*> memBlocks;
    MemData *data;

signals:
    void mem_replace_finished();
};

#endif // MEMORY_H
