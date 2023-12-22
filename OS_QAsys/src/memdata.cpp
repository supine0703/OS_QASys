#include "memdata.h"

#include "showlabel.h"

MemData::MemData(int size, QWidget *parent)
    : QObject{parent}
    , memSize(size)
    , info(new DataLabel(this, parent, QString("Size: %1KB").arg(size)))
    , hitLabel(new DataLabel(this, parent, "Hit:", "0"))
    , missLabel(new DataLabel(this, parent, "Miss:", "0"))
    , totalLabel(new DataLabel(this, parent, "Total:", "0"))
    , replaceLabel(new DataLabel(this, parent, "Replace:", "0"))
    , hitRateLabel(new DataLabel(this, parent, "Hit Rate:", "0.000"))
    , missRateLabel(new DataLabel(this, parent, "Miss Rate:", "0.000"))
{
    Q_ASSERT(parent != nullptr);
    info->Mark();
    MoveTo(QPoint(0, 0));
}

void MemData::Update()
{
    int total = hit + miss;
    replace = miss - memSize;
    if (replace < 0)
        replace = 0;
    hitLabel->Update(QString::number(hit));
    missLabel->Update(QString::number(miss));
    totalLabel->Update(QString::number(total));
    replaceLabel->Update(QString::number(replace));
    hitRateLabel->Update(QString::number(1.0 * hit / total, 'f', 3));
    missRateLabel->Update(QString::number(1.0 * miss / total, 'f', 3));
}

void MemData::SetAlgorithm(const QString& str)
{
    info->Update(str);
}

void MemData::MoveTo(const QPoint &pos)
{
    QPoint y(0, 50);
    int i = 0;
    auto dlabels = this->findChildren<DataLabel*>();
    for(auto dl : dlabels)
        dl->MoveTo(pos + y * i++);
}

void MemData::HitPage()
{
    hit++;
}

void MemData::MissPage()
{
    miss++;
}

MemData::DataLabel::DataLabel(QObject *tp, QWidget *p, const QString &s, const QString &d)
    : QObject{tp}
    , str(new ShowLabel(s, p))
    , data(new ShowLabel(d, p))
{
    str->resize(160, 40);
    this->MoveTo(QPoint(0, 0));
}

void MemData::DataLabel::MoveTo(const QPoint &pos)
{
    str->move(pos);
    data->move(pos + QPoint(158, 0));
}

void MemData::DataLabel::Update(const QString &dat)
{
    data->setText(dat);
}

void MemData::DataLabel::Mark()
{
    str->MarkBlue();
    data->MarkBlue();
}
