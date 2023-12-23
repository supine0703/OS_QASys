#include "pagereplace.h"
#include "recordwidget.h"

FIFOConsumer::FIFOConsumer(int size, BuffQue *buffer, QWidget *parent)
    : Consumers(size, buffer, parent)
{
    mem->data->SetAlgorithm("FIFO");
    mem->SetInterface(this);
    for (auto& m : mem->memBlocks)
        m->SetAddition(0);
}

void FIFOConsumer::Update()
{
    if (hitPageBlock != MEM_NULL)
    {
        if (mem->whoMark == hitPageBlock)
            mem->MarkRed(hitPageBlock);
        return;
    }
    mem->memBlocks[p]->SetAddition(++count);
    p = (p + 1) % mem->memBlocks.size();
    mem->MarkRed(p);
}

//------------------------------------------------------------------------------

LRUConsumer::LRUConsumer(int size, BuffQue *buffer, QWidget *parent)
    : Consumers(size, buffer, parent)
{
    mem->data->SetAlgorithm("LRU");
    mem->SetInterface(this);
    time = QVector<int>(mem->memBlocks.size(), 0);
    for (auto& m : mem->memBlocks)
        m->SetAddition(0);
}

void LRUConsumer::Update()
{
    bool missFlg = (hitPageBlock == MEM_NULL);
    if (missFlg)
        time[p] = -1;
    for (int i = 0, end = mem->memBlocks.size(), max = 0; i < end; i++)
    {
        time[i]++;
        mem->memBlocks[i]->SetAddition(time[i]);
        if (missFlg && time[i] > max)
        {
            max = time[i];
            p = i;
        }
    }
    if (missFlg)
        mem->MarkRed(p);
    else
    {
        time[hitPageBlock] = 0;
        mem->memBlocks[hitPageBlock]->SetAddition(0);
        if (mem->whoMark == hitPageBlock)
            mem->MarkRed(hitPageBlock);
    }
}


//------------------------------------------------------------------------------

LFUConsumer::LFUConsumer(int size, BuffQue *buffer, QWidget *parent)
    : Consumers(size, buffer, parent)
{
    mem->data->SetAlgorithm("LFU");
    mem->SetInterface(this);
    count = QVector<int>(PAGE_MAX_NUM + 1, 0);
    for (auto& m : mem->memBlocks)
        m->SetAddition(0);
}

void LFUConsumer::Update()
{
    auto index = [this](int num) {
        int value = mem->memBlocks[num]->MemValue();
        return (value == MEM_NULL) ? PAGE_MAX_NUM : value;
    };
    if (hitPageBlock != MEM_NULL)
    {
        int tmp = ++count[index(hitPageBlock)];
        mem->memBlocks[hitPageBlock]->SetAddition(tmp);
        if (mem->whoMark == hitPageBlock)
            mem->MarkRed(hitPageBlock);
        return;
    }
    mem->memBlocks[p]->SetAddition(++count[index(p)]);
    p = 0;
    for (int i = 0, end = mem->memBlocks.size(), min = INT_MAX; i < end; i++)
    {
        int tmp = index(i);
        if (count[tmp] < min)
        {
            min = count[tmp];
            p = i;
        }
    }
    mem->MarkRed(p);
}


//------------------------------------------------------------------------------


NRUConsumer::NRUConsumer(int size, BuffQue *buffer, QWidget *parent)
    : Consumers(size, buffer, parent)
{
    mem->data->SetAlgorithm("NRU");
    mem->SetInterface(this);
    flag = QVector<bool>(mem->memBlocks.size(), 0);
    for (auto& m : mem->memBlocks)
        m->SetAddition(0);
}

void NRUConsumer::Update()
{
    if (hitPageBlock != MEM_NULL)
    {
        flag[hitPageBlock] = 1;
        mem->memBlocks[hitPageBlock]->SetAddition(1);
        if (mem->whoMark == hitPageBlock)
            mem->MarkRed(hitPageBlock);
        return;
    }
    flag[p] = 1;
    mem->memBlocks[p]->SetAddition(1);
    p = (p + 1) % flag.size();
    while (flag[p])
    {
        flag[p] = 0;
        mem->memBlocks[p]->SetAddition(0);
        p = (p + 1) % flag.size();
    }
    mem->MarkRed(p);
}

OPTConsumer::OPTConsumer(int size, BuffQue *buffer, QWidget *parent)
    : Consumers(size, buffer, parent)
{
    mem->data->SetAlgorithm("OPT");
    mem->SetInterface(this);
    next = QVector<int>(mem->memBlocks.size(), INT_MAX);
    for (auto& m : mem->memBlocks)
        m->SetAddition(MEM_NULL);
}

void OPTConsumer::Update()
{
    int n;
    if (hitPageBlock == MEM_NULL)
        n = p;
    else
    {
        n = hitPageBlock;
        mem->UnMark(p);
    }
    next[n] = RECD()(mem->memBlocks[n]->MemValue());
    mem->memBlocks[n]->SetAddition(next[n]);
    next[n]++;


    for (int i = 0, end = mem->memBlocks.size(), max = 0; i < end; i++)
    {
        if (next[i] == INT_MAX)
        {
            p = i;
            break;
        }
        next[i]--;
//        if (next[i] == 1)
//        {
//            p = i;
//            max = INT_MAX;
//        }
        mem->memBlocks[i]->SetAddition(next[i]);
        if (next[i] > max)
        {
            p = i;
            max = next[i];
        }
    }

    mem->MarkRed(p);
}
