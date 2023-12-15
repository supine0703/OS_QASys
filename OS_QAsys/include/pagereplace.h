#ifndef PAGEREPLACE_H
#define PAGEREPLACE_H

#include "memblock.h"
#include "consumers.h"


class FIFOConsumer : public Consumers, public UpdateAddBolck
{ // 先进先出算法
public:
    FIFOConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0;
    int count = 0;
};


class LRUConsumer : public Consumers, public UpdateAddBolck
{ // 最近最久未使用算法
public:
    LRUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0;
    QVector<int> time;
};


class LFUConsumer : public Consumers, public UpdateAddBolck
{ // 最少使用置换算法
public:
    LFUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0;
    QVector<int> count;
};


class NRUConsumer : public Consumers, public UpdateAddBolck
{ // 最近未用算法
public:
    NRUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0;
    QVector<bool> flag;
};

#endif // PAGEREPLACE_H
