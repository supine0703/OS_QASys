#ifndef CONSUMERS_H
#define CONSUMERS_H

#include <QObject>

class UpdateAddBolck;
class ShowLabel;
class BuffQue;
class Memory;

class Consumers : public QObject
{
    Q_OBJECT
    Consumers(const Consumers&) = delete;
    Consumers& operator=(const Consumers&) = delete;
public:
    explicit Consumers(int size, BuffQue* buffer, QWidget *parent = nullptr);
    void MemoryMoveTo(const QPoint& pos);
    void MemDataMoveTo(const QPoint& pos);
    void Start();
    void Stop();

private:
    bool MissingPage(ShowLabel *label);
    void UpdateData();


protected:
    Memory* mem;
    int hitPage = -1;
    bool stpFlg = true;

signals:
    void start_consumer();
};

#endif // CONSUMERS_H
