#ifndef MEMBLOCK_H
#define MEMBLOCK_H

#include "showlabel.h"

class UpdateAddBolck
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
    explicit MemBlock(QWidget *parent=nullptr);
    ~MemBlock();
    void MarkRed();
    void UnMark();
    void MoveTo(const QPoint& pos);
    void Move(int x, int y);
    void SetParent(QWidget *parent);
    bool Replacement(ShowLabel *page);
    void AddBlockUpdate();

    void SetInterface(UpdateAddBolck *uab)
    { this->uab = uab; }

private:
    UpdateAddBolck *uab = nullptr;
    ShowLabel *blockNum;
public:
    ShowLabel *memBlock;
    ShowLabel *addBlock;

public:
    static void SetSpeed(int rate) { spedRate = 0.4 + 0.025 * rate; }
    static void ReSet();
private:
    static float spedRate;
    static int numGenerator;

signals:
    void replace_finished();
};

#endif // MEMBLOCK_H
