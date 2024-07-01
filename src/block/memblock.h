#ifndef MEMBLOCK_H
#define MEMBLOCK_H

#include "models/showlabel.h"

#define MEM_NULL -1

class MemBlock : public QObject
{
    Q_OBJECT
public:
    explicit MemBlock(QWidget *parent = nullptr);
    void MarkRed();
    void UnMark();
    void MoveTo(const QPoint& pos); // move to absolute psition.
    void Move(int x, int y); // move to relative psition.
    bool ReplacePage(ShowLabel *page);
    void UpdateAddBlock(); // update additional block.

    int MemValue();
    void SetAddition(int info);


private:
    ShowLabel *blockNum; // address (num).
    ShowLabel *memBlock; // memory value.
    ShowLabel *addBlock; // additional block.

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
