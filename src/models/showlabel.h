#ifndef SHOWLABEL_H
#define SHOWLABEL_H

#include <QLabel>
#include <QEasingCurve>

#define SHOWLABEL_ANIMATION_DEFAULT_TIME 1000
class QPropertyAnimation;
class QMutex;

class ShowLabel : public QLabel
{
    Q_OBJECT
private:
    QMutex *flg; // flag showing animation.
    bool forcing = false;
    void Mark(const QString&);

public:
    ShowLabel(QWidget* parent = nullptr);
    ShowLabel(const QString &text, QWidget* parent = nullptr);
    ShowLabel(int number, QWidget* parent = nullptr);
    ~ShowLabel();

    void SetHexText(int number); // set hex number in text.

    void MarkRed();    // the word is marker in red.
    void MarkBlue();   // the word is marker in blue.
    void MarkPurple(); // the word is marker in purple.
    void UnMark();     // restore default.

    bool ShowMoveTo(
        const QPoint &pos,
        int msecs = SHOWLABEL_ANIMATION_DEFAULT_TIME,
        QEasingCurve::Type = QEasingCurve::InOutSine
    ); // show move to absolute coord.
    bool ShowMove(
        int x, int y,
        int msecs = SHOWLABEL_ANIMATION_DEFAULT_TIME,
        QEasingCurve::Type = QEasingCurve::InOutSine
    ); // show move to relative coord.
    void ShowForcingMove(
        int x, int y,
        int msecs = SHOWLABEL_ANIMATION_DEFAULT_TIME
    ); // forcing show move to relative coord.
    bool ShowLoad(
        int msecs = SHOWLABEL_ANIMATION_DEFAULT_TIME,
        const QString &color = "blue"
    ); // show process of load.

private:
    void AnimationMoveTo(
        const QPoint&, int, QEasingCurve::Type
        ); // show animation about move.
    void FinishAnimation();
    QPoint endPos;
    QPropertyAnimation *animation = nullptr;

public:
    static void SetSpeed(int rate);
private:
    static float speedRate;

signals:
    void animation_finished();
};

#endif // SHOWLABEL_H
