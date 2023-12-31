#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>

class ShowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowWidget(
        int way, int which, int bufSize, int memSize, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QSize view1Size;
    QPoint basic;

    QWidget *view1;

signals:

};

#endif // SHOWWIDGET_H
