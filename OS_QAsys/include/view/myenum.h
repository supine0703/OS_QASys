#ifndef MYENUM_H
#define MYENUM_H

class MyE
{
    MyE() = delete;
    ~MyE() = delete;
    MyE(const MyE&) = delete;
    MyE(const MyE&&) = delete;
public:
    enum PageReplaceAlgorithm
    {
        FIFO = 0, LRU, LFU, NRU, OPT,
        ALG_NUM
    };
    enum ProduceWay
    {
        Random = 0, RdFile
    };
    enum WhichWindow
    {
        StartWindow = 0, ShowWindow
    };
};

#endif // MYENUM_H
