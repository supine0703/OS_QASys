#ifndef MEMDATA_H
#define MEMDATA_H

#include <QObject>

class ShowLabel;

class MemData : public QObject
{
public:
    explicit MemData(int size, QWidget *parent = nullptr);
    void Update(); // update the value to label.
    void SetAlgorithm(const QString &str); // ask me which algorithm.
    void MoveTo(const QPoint& pos);

    void HitPage();
    void MissPage();

private:
    int memSize;
    int hit = 0, miss = 0, replace = 0;

    class DataLabel : public QObject
    {
    public:
        DataLabel(QObject*, QWidget*, const QString&, const QString& = "");
        void MoveTo(const QPoint& pos);
        void Update(const QString &dat);
        void Mark();
    private:
        ShowLabel *str, *data;
    };
    DataLabel *info;
    DataLabel *hitLabel, *missLabel, *totalLabel, *replaceLabel;
    DataLabel *hitRateLabel, *missRateLabel;
};

#endif // MEMDATA_H
