#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartWidget *ui;

signals:
    void start(int, int, int);
};

#endif // STARTWIDGET_H
