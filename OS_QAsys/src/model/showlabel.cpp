#include "showlabel.h"
#include <QPropertyAnimation>

void ShowLabel::Init()
{
    this->resize(80, 40);
    this->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    this->UnMark();
    endPos = this->pos();
}

void ShowLabel::Mark(const QString &color)
{
    this->setStyleSheet(
        QString("border: 2px solid black; color: %1; background: white;").arg(color)
        );
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
    this->Mark("red");
}

void ShowLabel::MarkBlue()
{
    this->Mark("blue");
}

void ShowLabel::MarkPurple()
{
    this->Mark("purple");
}

void ShowLabel::UnMark()
{
    this->Mark("black");
}

bool ShowLabel::ShowMoveTo(const QPoint &pos, int msecs, QEasingCurve::Type curve)
{
    if (flg) return false;
    flg = true;
    AnimationMoveTo(pos, msecs, curve);
    return true;
}

bool ShowLabel::ShowMove(int x, int y, int msecs, QEasingCurve::Type curve)
{
    return ShowMoveTo(this->pos() + QPoint(x, y), msecs, curve);
}

void ShowLabel::ShowForcingMove(int x, int y, int msecs)
{
    flg = true;
    AnimationMoveTo(endPos + QPoint(x, y), msecs, QEasingCurve::OutCubic);
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
//    animation->setEasingCurve(QEasingCurve::InOutQuad); // easing curve
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->MarkBlue();
        flg = false;
        emit animation_finished();
    });
    return true;
}

void ShowLabel::AnimationMoveTo(const QPoint &pos, int msecs, QEasingCurve::Type curve)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(msecs * speedRate);   // time
    animation->setStartValue(this->pos()); // start pos
    animation->setEndValue(endPos = pos);  // end pos
    animation->setEasingCurve(curve);      // easing curve
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        flg = false;
        emit animation_finished();
    });
}

float ShowLabel::speedRate = 1;
void ShowLabel::SetSpeed(int rate)
{
    speedRate = 0.1 + 0.023 * rate;
}
