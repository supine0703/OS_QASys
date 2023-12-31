#ifndef PAGEREPLACE_H
#define PAGEREPLACE_H

#include "memory.h"
#include "consumers.h"

class FIFOConsumer : public Consumers, public UpdateAddBolck
{ // 先进先出算法
public:
    FIFOConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0; // pointer to the page next to be replaced.
    int count = 0; // input which count.
};


class LRUConsumer : public Consumers, public UpdateAddBolck
{ // 最近最久未使用算法
public:
    LRUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0; // pointer to the page next to be replaced.
    QVector<int> time; // time of each page from last access time.
};


class LFUConsumer : public Consumers, public UpdateAddBolck
{ // 最少使用置换算法
public:
    LFUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0; // pointer to the page next to be replaced.
    QVector<int> count; // count of all each page.
};


class NRUConsumer : public Consumers, public UpdateAddBolck
{ // 最近未用算法
public:
    NRUConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0; // pointer to the page next to be replaced.
    QVector<bool> flag; // flag about is last visited.
};

class OPTConsumer : public Consumers, public UpdateAddBolck
{ // 最佳置换算法
public:
    OPTConsumer(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void Update() override;

private:
    int p = 0; // pointer to the page next to be replaced.
    QVector<int> next; // time of each page from next visited.
};

#endif // PAGEREPLACE_H
