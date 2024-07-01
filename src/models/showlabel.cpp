#include "showlabel.h"
#include <QMutex>
#include <QPropertyAnimation>

void ShowLabel::Mark(const QString& color)
{
    this->setStyleSheet(
        QString("border: 2px solid black; color: %1; background: white;").arg(color)
    );
}

ShowLabel::ShowLabel(QWidget* parent)
    : QLabel(parent)
    , flg(new QMutex())
{
    this->resize(80, 40);
    this->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    this->UnMark();
    endPos = this->pos();
}

ShowLabel::ShowLabel(const QString& text, QWidget* parent)
    : ShowLabel(parent)
{
    this->setText(text);
}

ShowLabel::ShowLabel(int number, QWidget* parent)
    : ShowLabel(parent)
{
    SetHexText(number);
}

ShowLabel::~ShowLabel()
{
    flg->try_lock();
    flg->unlock();
    delete flg;
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
    if (!flg->try_lock())
        return false;
    AnimationMoveTo(pos, msecs, curve);
    return true;
}

bool ShowLabel::ShowMove(int x, int y, int msecs, QEasingCurve::Type curve)
{
    return ShowMoveTo(this->pos() + QPoint(x, y), msecs, curve);
}

void ShowLabel::ShowForcingMove(int x, int y, int msecs)
{
    forcing = true;
    AnimationMoveTo(endPos + QPoint(x, y), msecs, QEasingCurve::OutCubic);
}

bool ShowLabel::ShowLoad(int msecs, const QString &color)
{
    if (!flg->try_lock())
        return false;
    this->setStyleSheet(QString("background: %1; color: %1;").arg(color));
    animation = new QPropertyAnimation(this, "size", this);
    animation->setStartValue(QSize(1, this->height()));
    animation->setEndValue(this->size());
    animation->setDuration(msecs * speedRate); // time.
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->MarkBlue();
        FinishAnimation();
    });
    return true;
}

void ShowLabel::AnimationMoveTo(const QPoint &pos, int msecs, QEasingCurve::Type curve)
{
    if (forcing && animation != nullptr)
    {
        this->disconnect(animation);
        animation->deleteLater();
        forcing = false;
    }
    animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(this->pos()); // start pos.
    animation->setEndValue(endPos = pos);  // end pos.
    animation->setEasingCurve(curve);      // easing curve.
    animation->setDuration(msecs * speedRate);   // time.
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &ShowLabel::FinishAnimation);
}

void ShowLabel::FinishAnimation()
{
    this->disconnect(animation);
    animation->deleteLater();
    animation = nullptr;
    flg->try_lock();
    flg->unlock();
    emit animation_finished();
}

float ShowLabel::speedRate = 1;
void ShowLabel::SetSpeed(int rate)
{
    speedRate = 0.1 + 0.023 * rate;
}
