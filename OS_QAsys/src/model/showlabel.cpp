#include "showlabel.h"
#include <QPropertyAnimation>

float ShowLabel::speedRate = 1;

void ShowLabel::Init()
{
    this->resize(80, 40);
    this->setStyleSheet("border: 2px solid black;");
    this->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    endPos = this->pos();
}

ShowLabel::ShowLabel(QWidget *parent)
    : QLabel(parent)
{
    this->Init();
}

ShowLabel::ShowLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    this->Init();
}

ShowLabel::ShowLabel(int number, QWidget *parent)
    : QLabel(parent)
{
//    int num = number & 0x000f;
//    QString numberStr(QChar(num + (num > 9 ? 55 : 48))); // 65: 'A', 48: '0'
//    for (int i = 0; i < 3; i++)
//    {
//        number >>= 4;
//        num = number & 0x000f;
//        numberStr = QChar(num + (num > 9 ? 55 : 48)) + numberStr;
//    }
//    this->setText(numberStr);
    SetHexText(number);
    this->Init();
}

void ShowLabel::SetHexText(int number)
{
    QString hexString = QString("%1").arg(number, 4, 16, QChar('0')).toUpper();
    this->setText(hexString);
}

void ShowLabel::MarkRed()
{
    this->setStyleSheet("border: 2px solid black; color: red;");
}

void ShowLabel::MarkBlue()
{
    this->setStyleSheet("border: 2px solid black; color: blue;");
}

void ShowLabel::UnMark()
{
    this->setStyleSheet("border: 2px solid black;");
}

bool ShowLabel::ShowMoveTo(const QPoint &pos, int msecs)
{
    if (flg) return false;
    flg = true;
    AnimationMoveTo(pos, msecs);
    return true;
}

bool ShowLabel::ShowMove(int x, int y, int msecs)
{
    return ShowMoveTo(this->pos() + QPoint(x, y), msecs);
}

void ShowLabel::ShowForcingMove(int x, int y, int msecs)
{
    forcing = flg = true;
    AnimationMoveTo(endPos + QPoint(x, y), msecs);
    forcing = false;
}

bool ShowLabel::ShowLoad(int msecs, const QString &color)
{
    if (flg) return false;
    flg = true;
    this->setStyleSheet(QString("background: %1; color: %1;").arg(color));
    QPropertyAnimation *animation = new QPropertyAnimation(this, "size");
    animation->setDuration(msecs * speedRate); // time
    animation->setStartValue(QSize(1, this->height()));
    animation->setEndValue(this->size());
    animation->setEasingCurve(QEasingCurve::InOutQuad); // easing curve
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        flg = false;
        this->MarkBlue();
        emit animation_finished();
    });
    return true;
}

void ShowLabel::AnimationMoveTo(const QPoint &pos, int msecs)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(msecs * speedRate);   // time
    animation->setStartValue(this->pos()); // start pos
    animation->setEndValue(endPos = pos);  // end pos
    animation->setEasingCurve(             // easing curve
        forcing ? QEasingCurve::OutCubic : QEasingCurve::InOutQuad);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        flg = false;
        emit animation_finished();
    });
}
