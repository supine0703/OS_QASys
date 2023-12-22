#include "pagereplace.h"

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
    if (hitPage != -1)
    {
        if (mem->whoMark == hitPage)
            mem->MarkRed(hitPage);
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
    bool missFlg = (hitPage == -1);
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
        time[hitPage] = 0;
        mem->memBlocks[hitPage]->SetAddition(0);
        if (mem->whoMark == hitPage)
            mem->MarkRed(hitPage);
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
    if (hitPage != -1)
    {
        int tmp = ++count[index(hitPage)];
        mem->memBlocks[hitPage]->SetAddition(tmp);
        if (mem->whoMark == hitPage)
            mem->MarkRed(hitPage);
        return;
    }
    mem->memBlocks[p]->SetAddition(++count[index(p)]);
    p = 0;
    for (int i = 0, end = mem->memBlocks.size(), min = 0x7fffffff; i < end; i++)
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
    if (hitPage != -1)
    {
        flag[hitPage] = 1;
        mem->memBlocks[hitPage]->SetAddition(1);
        if (mem->whoMark == hitPage)
            mem->MarkRed(hitPage);
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
