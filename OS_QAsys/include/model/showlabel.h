#ifndef SHOWLABEL_H
#define SHOWLABEL_H

#include <QLabel>

class ShowLabel : public QLabel
{
    Q_OBJECT
private:
    bool flg = false, forcing = false;
    void Init();

public:
    ShowLabel(QWidget *parent=nullptr);

    ShowLabel(const QString &text, QWidget *parent=nullptr);

    ShowLabel(int number, QWidget *parent=nullptr);

    void SetHexText(int number);

    void MarkRed();

    void MarkBlue();

    void UnMark();

    bool ShowMoveTo(const QPoint &pos, int msecs = 1000);

    bool ShowMove(int x, int y, int msecs = 1000);

    void ShowForcingMove(int x, int y, int msecs = 1000);

    bool ShowLoad(int msecs = 1000, const QString &color = "blue");

private:
    void AnimationMoveTo(const QPoint &pos, int msecs);

    QPoint endPos;

public:
    static void SetSpeed(int rate) { speedRate = 0.2 + 0.048 * rate; }
private:
    static float speedRate;

signals:
    void animation_finished();
};



#endif // SHOWLABEL_H
