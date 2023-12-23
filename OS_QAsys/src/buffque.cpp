#include "buffque.h"

BuffQue::BuffQue(int queSize, QWidget *parent)
    : QObject{parent}
    , back(new QLabel(parent))
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(4 <= queSize && queSize <= 24);
    back->resize(84, queSize * 40 + 4);
    back->setStyleSheet("background: gray;");
    buffer = QVector<ShowLabel*>(queSize);
    this->MoveTo(QPoint(0, 0));
}

bool BuffQue::Push(ShowLabel *page)
{
    if (count == buffer.size()) // is full.
        return false;

    mutex.lock();
    if (ibusy)
    {
        mutex.unlock();
        return false;
    }
    ibusy = true;

    bool empty = (count == 0);
    // push {
    buffer[tail] = page; // push.
    tail = (tail + 1) % buffer.size();
//    count++; // }

    // show pushing animation.
    page->MarkRed();
    // move to buffer's position which belong.
    if (!page->ShowMoveTo(basicPos + QPoint(0, count * 40), 1000 * speedRate))
        Q_ASSERT_X(false, "class BuffQue", "try show animation which showing");
    connect(page, &ShowLabel::animation_finished, this, [this, empty, page]() {
        page->disconnect(this);
        page->UnMark();
        count++; // push successfully.
        ibusy = false; // animation finished.
        emit push_finished();
        if (empty) {
            emit is_not_empty();
        }
    });

    mutex.unlock();
    return true;
}

bool BuffQue::Pop(ShowLabel *&page)
{
    if (count == 0) // is empty.
        return false;

    mutex.lock();
    if (obusy)
    {
        mutex.unlock();
        return false;
    }
    obusy = true;

    bool full = (count == buffer.size());
    // pop {
    page = buffer[head]; // pop.
    head = (head + 1) % buffer.size();
    count--; // } push successfully.

    // show poping animation.
    page->MarkPurple();
    for (int i = head; i != tail; i = (i + 1) % buffer.size())
        buffer[i]->ShowForcingMove(0, -40, 1000 * speedRate); // all move up 40.
    if (!page->ShowMove(80, 0, 596 * speedRate, QEasingCurve::OutExpo))
        Q_ASSERT_X(false, "class BuffQue", "try show animation which showing");

    connect(page, &ShowLabel::animation_finished, this, [this, full, page]() {
        page->disconnect(this);
        obusy = false; // animation finished.
        emit pop_finished();
        if (full) {
            emit is_not_full();
        }
    });

    mutex.unlock();
    return true;
}

void BuffQue::MoveTo(const QPoint &pos)
{
    basicPos = pos + QPoint(2, 2);
    back->move(pos);
    int cunt = 0; // calculate the relativate position.
    if (head == tail && !count) // is empty.
        return;
    buffer[head]->move(basicPos + QPoint(0, cunt++ * 40));
    for (int i = (head + 1) % buffer.size(); i != tail; i = (i + 1) % buffer.size())
        buffer[i]->move(basicPos + QPoint(0, i * 40));
}

float BuffQue::speedRate = 1;
void BuffQue::SetSpeed(int rate)
{
    speedRate = 0.2 + 0.018 * rate;
}


