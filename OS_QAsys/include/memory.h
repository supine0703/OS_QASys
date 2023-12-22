#ifndef MEMORY_H
#define MEMORY_H

#include "memblock.h"
#include "memdata.h"

class UpdateAddBolck // implementing interface by different page replacement algorithms.
{
public:
    virtual ~UpdateAddBolck() { }
    virtual void Update() = 0;
};

class Memory : public QObject
{
    friend class Consumers;
    friend class FIFOConsumer;
    friend class LRUConsumer;
    friend class LFUConsumer;
    friend class NRUConsumer;
    friend class OPTConsumer;

    Q_OBJECT
public:
    explicit Memory(int blockNum, QWidget *parent = nullptr);
    void MoveTo(const QPoint &pos);
    void MemDataMoveTo(const QPoint &pos);
    void MarkRed(int num);
    void UnMark(int num);
    bool Replace(ShowLabel *page, int num = -1);
    void SetInterface(UpdateAddBolck *uab);
    bool Busy() { return busy; }

private:
    UpdateAddBolck *uab = nullptr; // interface pointer.
    bool busy = false;
    int whoMark = -1;
    QVector<MemBlock*> memBlocks;
    MemData *data;

signals:
    void mem_replace_finished();
};

#endif // MEMORY_H
