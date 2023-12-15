#ifndef MEMDATA_H
#define MEMDATA_H

#include <QObject>

class ShowLabel;

class MemData : public QObject
{
    Q_OBJECT
public:
    explicit MemData(int size, QWidget *parent = nullptr);
    void MoveTo(const QPoint& pos);
    void Update();
    void SetAlgorithm(const QString &str);

    int total = 0, hit = 0, miss = 0, replace = 0;
    float hitRate = 0, missRate = 0;

private:
    ShowLabel *sizeStr;
    ShowLabel *algorithmStr;
    ShowLabel *hitStr, *hitData;
    ShowLabel *missStr, *missData;
    ShowLabel *totalStr, *totalData;
    ShowLabel *replaceStr, *replaceData;
    ShowLabel *hitRateStr, *hitRateData;
    ShowLabel *missRateStr, *missRateData;

signals:

};

#endif // MEMDATA_H
