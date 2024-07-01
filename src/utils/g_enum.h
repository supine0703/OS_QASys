#ifndef G_ENUM_H
#define G_ENUM_H

class G_E
{
    G_E() = delete;
    ~G_E() = delete;
    G_E(const G_E&) = delete;
    G_E(const G_E&&) = delete;
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

#endif // G_ENUM_H
