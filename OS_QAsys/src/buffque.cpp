#include "buffque.h"

BuffQue::BuffQue(int queSize, QWidget *parent)
    : QObject{parent}
    , back(new QLabel(parent))
{
    Q_ASSERT(4 <= queSize && queSize <= 24);
    back->resize(84, queSize * 40 + 4);
    back->setStyleSheet("border: 2px solid gray;");
    basicPos = QPoint(2, 2);
    buffer = QVector<ShowLabel*>(queSize);
}

bool BuffQue::Push(ShowLabel *page)
{
    if (head == tail && count == buffer.size()) // is full
        return false;

    mutex.lock();
    if (ibusy)
    {
        mutex.unlock();
        return false;
    }
    ibusy = true;
    
    page->MarkRed();
    if (page->ShowMoveTo(basicPos + QPoint(0, count * 40)))
        Q_ASSERT(false);
    connect(page, &ShowLabel::animation_finished, this, [this, page]() {
        page->UnMark();
        page->disconnect(this);
        ibusy = false;
        emit push_finished();
    });

    buffer[tail] = page; // push
    tail = (tail + 1) % buffer.size();
    count++;

    mutex.unlock();
    return true;
}

bool BuffQue::Pop(ShowLabel *&page)
{
    if (head == tail && !count) // is empty
        return false;

    mutex.lock();
    if (obusy)
    {
        mutex.unlock();
        return false;
    }
    obusy = true;

    int last = head;
    bool full = (head == tail);
    page = buffer[head]; // pop
    head = (head + 1) % buffer.size();
    count--;
    page->MarkRed();

    for (int i = head; i != tail; i = (i + 1) % buffer.size())
    {
        buffer[i]->ShowForcingMove(0, -40);
        last = i;
    }
    if (--last < 0)
        last = buffer.size() - 1;
    if (!count)
        obusy = false;
    else
        connect(buffer[last], &ShowLabel::animation_finished, this, [this]() {
            obusy = false;
            emit pop_finished();
        });
    if (full)
        connect(buffer[last], &ShowLabel::animation_finished, this, [this]() {
            emit is_not_full();
        });
    mutex.unlock();
    return true;
}

void BuffQue::MoveTo(const QPoint &pos)
{
    basicPos = pos + QPoint(2, 2);
    back->move(pos);
    int cunt = 0;
    if (head == tail && !count) // is empty
        return;
    buffer[head]->move(basicPos + QPoint(0, cunt++ * 40));
    for (int i = (head + 1) % buffer.size(); i != tail; i = (i + 1) % buffer.size())
        buffer[i]->move(basicPos + QPoint(0, i * 40));
}


