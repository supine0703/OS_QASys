#include "recordwidget.h"

#include <QFile>
#include <QMutex>
#include <QTextStream>
#include <QTextEdit>
#include <QTextCursor>
#include <QScrollBar>

RecordWidget *RecordWidget::ptr = nullptr;
RecordWidget &RecordWidget::Singleton()
{
    if (ptr == nullptr)
    {
        static QMutex mutex;
        mutex.lock();
        if (ptr == nullptr)
            ptr = new RecordWidget();
        mutex.unlock();
    }
    return *ptr;
}

// -----------------------------------------------------------------------------

RecordWidget::RecordWidget(Qt::GlobalColor produceColor,
                           Qt::GlobalColor consumeColor,
                           Qt::GlobalColor markColor,
                           QWidget *parent)
    : QWidget{parent}
    , mtx(new QMutex())
    , textEdit(new QTextEdit(this))
    , produceFormat(new QTextCharFormat())
    , consumeFormat(new QTextCharFormat())
    , markFormat(new QTextCharFormat())
{
    textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    produceFormat->setForeground(produceColor);
    consumeFormat->setForeground(consumeColor);
    markFormat->setForeground(markColor);
    this->Reset();
}

RecordWidget::~RecordWidget()
{
    ptr = nullptr;
    delete produceFormat;
    delete consumeFormat;
    delete markFormat;
    delete mtx;
}

void RecordWidget::Reset()
{
    mtx->try_lock();
    mtx->unlock();
    this->data.clear();
    this->textEdit->clear();
    p_ptr = c_ptr = 0;
}

auto MarkText = [](QTextEdit *text, int num, int space, QTextCharFormat *color) {
    auto cursor = text->textCursor();
    cursor.setPosition(num * space);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, space - 1);
    cursor.setCharFormat(*color);
    cursor.clearSelection();
};

int RecordWidget::operator++()
{
    mtx->lock();
    if (data.size() == 0)
    {
        mtx->unlock();
        return INC_NULL;
    }
    if (data.size() <= p_ptr)
        p_ptr = 0;
    if (trace == 1)
    {
        auto bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum() * p_ptr / data.size());
    }

    MarkText(textEdit, p_ptr, 12, p_ptr != c_ptr ? produceFormat : markFormat);
    p_ptr++;

    mtx->unlock();
    return p_ptr >= data.size() ? INC_NULL : data[p_ptr];
}

int RecordWidget::operator++(int)
{
    mtx->lock();
    if (data.size() == 0)
    {
        mtx->unlock();
        return INC_NULL;
    }
    if (data.size() <= p_ptr)
        p_ptr = 0;
    if (trace == 1)
    {
        auto bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum() * p_ptr / data.size());
    }

    int tmp = p_ptr;
    MarkText(textEdit, p_ptr, 12, p_ptr != c_ptr ? produceFormat : markFormat);
    p_ptr++;

    mtx->unlock();
    return data[tmp];
}

RecordWidget &RecordWidget::operator>>(int &number)
{
    mtx->lock();
    if (data.size() == 0)
    {
        number = INC_NULL;
        return *this;
    }
    if (data.size() <= c_ptr)
        c_ptr = 0;
    if (trace == 2)
    {
        auto bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum() * c_ptr / data.size());
    }

    number = data[c_ptr];
    MarkText(textEdit, c_ptr, 12, consumeFormat);
    c_ptr++;
    MarkText(textEdit, c_ptr, 12, markFormat);

    mtx->unlock();
    return *this;
}

RecordWidget &RecordWidget::operator<<(int number)
{
    mtx->lock();
    data.push_back(number);
    auto bar = textEdit->verticalScrollBar();
    bool bottom = (bar->value() == bar->maximum());
    textEdit->insertPlainText(
        QString("%1:%2  ")
            .arg(data.size(), 5, 10, QChar('_'))
            .arg(number, 4, 16, QChar('0')).toUpper());
    if (bottom)
        bar->setValue(bar->maximum());
    mtx->unlock();
    return *this;
}

int RecordWidget::operator()(int page)
{
    mtx->lock();
    int count = 0;
    int i = c_ptr;
    if (i == data.size())
        i = 0;
    do
    {
        count++;
        if ((data[i] >> PAGE_INC_BIT) == page)
            break;
        if (++i == data.size())
            i = 0;
    } while (i != c_ptr);
    mtx->unlock();
    return count;
}

void RecordWidget::SetTrace(int index)
{
    trace = index;
}

void RecordWidget::resizeEvent(QResizeEvent *event)
{
    textEdit->resize(this->size());
}


// -----------------------------------------------------------------------------

bool operator<<(QFile &file, const RecordWidget &record)
{
    // Write File
    bool open = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (open)
    {
        QTextStream out(&file);
        for (auto dt : record.data)
            out << QString::number(dt, 16) << " ";
        out << "#";
        file.close();
    }
    return open;
}

bool operator>>(QFile &file, RecordWidget &record)
{
    // Read File
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        auto read = in.readLine().split(" ");
        file.close();
        int size = read.size() - 1;
        if (size > 4 && read[size] == "#")
        {
            record.Reset();
            record.mtx->lock();
            record.data.resize(size);
            QString temp;
            for (int i = 0; i < size; i++)
            {
                bool ok;
                int num = read[i].toInt(&ok, 16);
                if (ok)
                {
                    record.data[i] = num;
                    temp += QString("%1:%2  ")
                            .arg(i, 5, 10, QChar('_'))
                            .arg(num, 4, 16, QChar('0')).toUpper();
                }
                else
                {
                    record.Reset();
                    return false;
                }
            }
            record.textEdit->setPlainText(temp);
            record.mtx->unlock();
            return true;
        }
    }
    return false;
}
