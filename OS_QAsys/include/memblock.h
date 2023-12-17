#ifndef MEMBLOCK_H
#define MEMBLOCK_H

#include "showlabel.h"

class UpdateAddBolck // implementing interface by different page replacement algorithms
{
public:
    virtual void Update() = 0;
};

class MemBlock : public QObject
{
    Q_OBJECT
    MemBlock(const MemBlock&) = delete;
    MemBlock& operator=(const MemBlock&) = delete;
public:
    explicit MemBlock(QWidget *parent = nullptr);
    void MarkRed();
    void UnMark();
    void MoveTo(const QPoint& pos); // move to absolute psition
    void Move(int x, int y); // move to relative psition
    void SetParent(QWidget *parent);
    bool ReplacePage(ShowLabel *page);
    void UpdateAddBlock(); // update additional block

    void SetInterface(UpdateAddBolck *uab)
    { this->uab = uab; }

private:
    UpdateAddBolck *uab = nullptr; // interface pointer
    ShowLabel *blockNum; // address (num)
public:
    ShowLabel *memBlock;
    ShowLabel *addBlock;

public:
    static void SetSpeed(int rate);
    static void ReSet();
private:
    static float spedRate;
    static int numGenerator;

signals:
    void block_replace_finished();
};

#endif // MEMBLOCK_H
