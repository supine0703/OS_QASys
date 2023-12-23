#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#define RECD RecordWidget::Singleton

#include <QWidget>

class QFile;
class QMutex;
class QTextEdit;
class QTextCursor;
class QTextCharFormat;

#define INC_NULL -1

class RecordWidget : public QWidget
{
public:
    static RecordWidget &Singleton();
private:
    static RecordWidget *ptr;

    RecordWidget(
        Qt::GlobalColor produceColor = Qt::blue,
        Qt::GlobalColor consumeColor = Qt::darkMagenta,
        Qt::GlobalColor markColor = Qt::red,
        QWidget *parent = nullptr);
public:
    ~RecordWidget();
    void Reset();
    int operator++(); // produce
    int operator++(int); // produce
    RecordWidget &operator>>(int &number); // consume
    RecordWidget &operator<<(int number); // load
    int operator()(int page); // find next which page

    void SetTrace(int index);

private:
    QMutex *mtx;
    QTextEdit *textEdit;
    QTextCharFormat *produceFormat, *consumeFormat, *markFormat;
    QVector<int> data;
    int p_ptr = 0, c_ptr = 0;
    int trace = 0;

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    friend bool operator<<(QFile &file, const RecordWidget &record);
    friend bool operator>>(QFile &file, RecordWidget &record);
};

#endif // RECORDWIDGET_H
