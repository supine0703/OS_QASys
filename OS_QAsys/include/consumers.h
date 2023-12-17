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
    virtual ~Consumers() { }
    void MemoryMoveTo(const QPoint& pos);
    void MemDataMoveTo(const QPoint& pos);
    void Start(); // start to consume
    void Stop();  // stop to consume

private:
    void ConctUpdate(); // need update data about Memory if consume finished
    bool MissingPage();
    void UpdateData();


    ShowLabel *label; // use to receive what buffer pop
    bool stpFlg = true;

protected:
    Memory* mem;
    int hitPage = -1;

signals:
    void start_consumer();
};

#endif // CONSUMERS_H
