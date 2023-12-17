#include "memdata.h"

#include "showlabel.h"

MemData::MemData(int size, QWidget *parent)
    : QObject{parent}
    , sizeStr(new ShowLabel(QString("Size: %1KB").arg(size), parent))
    , algorithmStr(new ShowLabel(parent))
    , hitStr(new ShowLabel("Hit:", parent))
    , hitData(new ShowLabel("0", parent))
    , missStr(new ShowLabel("Miss:", parent))
    , missData(new ShowLabel("0", parent))
    , totalStr(new ShowLabel("Total:", parent))
    , totalData(new ShowLabel("0", parent))
    , replaceStr(new ShowLabel("Replace:", parent))
    , replaceData(new ShowLabel("0", parent))
    , hitRateStr(new ShowLabel("Hit Rate:", parent))
    , hitRateData(new ShowLabel("0.000", parent))
    , missRateStr(new ShowLabel("Miss Rate:", parent))
    , missRateData(new ShowLabel("0.000", parent))
{
    Q_ASSERT(parent != nullptr);
    sizeStr->resize(160, 40);
    hitStr->resize(160, 40);
    missStr->resize(160, 40);
    totalStr->resize(160, 40);
    replaceStr->resize(160, 40);
    hitRateStr->resize(160, 40);
    missRateStr->resize(160, 40);
    MoveTo(QPoint(0, 0));
    sizeStr->MarkBlue();
    algorithmStr->MarkBlue();
}

void MemData::MoveTo(const QPoint &pos)
{
    QPoint y(0, 50), x(158, 0);
    int i = 0;
    sizeStr->move(pos + y * i);
    algorithmStr->move(pos + x + y * i++);
    totalStr->move(pos + y * i);
    totalData->move(pos + x + y * i++);
    hitStr->move(pos + y * i);
    hitData->move(pos + x + y * i++);
    missStr->move(pos + y * i);
    missData->move(pos + x + y * i++);
    replaceStr->move(pos + y * i);
    replaceData->move(pos + x + y * i++);
    hitRateStr->move(pos + y * i);
    hitRateData->move(pos + x + y * i++);
    missRateStr->move(pos + y * i);
    missRateData->move(pos + x + y * i++);
}

void MemData::Update()
{
    hitData->setText(QString::number(hit));
    missData->setText(QString::number(miss));
    totalData->setText(QString::number(total));
    replaceData->setText(QString::number(replace));
    hitRateData->setText(QString::number(hitRate, 'f', 3));
    missRateData->setText(QString::number(missRate, 'f', 3));
}

void MemData::SetAlgorithm(const QString& str)
{
    algorithmStr->setText(str);
}
