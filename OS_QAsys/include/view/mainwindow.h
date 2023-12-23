#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QDockWidget *recdDock;
    int cpWay, cpWhich, cpBuff, cpMem;
    int whichWindow;
    bool once = true;

    void SaveStateInfo();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;

};
#endif // MAINWINDOW_H
