#include "mainwindow.h"
#include "showwidget.h"
#include "startwidget.h"
#include "recordwidget.h"
#include "myenum.h"
#include "mysettings.h"

#include <QScreen>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QActionGroup>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setStyleSheet(QString("font: italic %1pt \"%2\";").arg(FONT_SIZE).arg(_FONT_));

    // new record dock widget.
    recdDock = new QDockWidget("&Record", this);
    recdDock->setStyleSheet("border: 1px solid black;");
    recdDock->setWidget(&RECD());
    recdDock->setFloating(true);
    // there will be a lot of problems, no compromise
    recdDock->setAllowedAreas(Qt::NoDockWidgetArea);

    // add menu list.
    auto fileM = this->menuBar()->addMenu("&File");
    auto viewM = this->menuBar()->addMenu("&View");

    // add menu action.
    // file menu.
    auto file_mainMenu = fileM->addAction("&Return Menu");
    fileM->addSeparator();
    auto file_save = fileM->addAction("&Save As");
    auto file_load = fileM->addAction("&Load From");
    // view menu.
    viewM->addAction(recdDock->toggleViewAction());
    viewM->addSeparator();
    auto view_trace = viewM->addMenu("&Trace Record");
    // trace menu
    auto view_trace_group = new QActionGroup(view_trace);
    view_trace_group->setExclusive(true);
    QAction *view_trace_all[] = {
        view_trace->addAction("&Default"),
        view_trace->addAction("&Produce"),
        view_trace->addAction("&Consume")
    };
    for (auto action : view_trace_all)
    {
        action->setCheckable(true);
        view_trace_group->addAction(action);
    }

    // let window show in the center.
    auto Geometry = [this](int width, int height) {
        QSize screenSize = QApplication::primaryScreen()->size();
        int x = (screenSize.width() - width) >> 1;
        int y = (screenSize.height() - height) / 3;
        this->setGeometry(x, y, width, height);
    };

    // show start widget.
    auto ShowMainMenu =
        [this, Geometry, file_save, file_load, view_trace_all]() {
        whichWindow = MyE::StartWindow;
        recdDock->setVisible(false);
        this->menuBar()->setVisible(false);
        auto startW = new StartWidget(this);
        this->setCentralWidget(startW);
        Geometry(startW->width(), startW->height());
        // go to show widget.
        connect(startW, &StartWidget::start, this,
                [this, Geometry, file_save, file_load, view_trace_all]
                (int way, int which, int buff, int mem) {
            whichWindow = MyE::ShowWindow;
            cpWay = way; cpWhich = which; cpBuff = buff; cpMem = mem;
            this->menuBar()->setVisible(true);
            auto showW = new ShowWidget(way, which, buff, mem, this);
            this->setCentralWidget(showW);
            switch (way)
            {
            case MyE::Random: // only save, can't load.
                file_save->setEnabled(true);
                file_load->setEnabled(false);
                break;
            case MyE::RdFile: // only load, can't save.
                file_save->setEnabled(false);
                file_load->setEnabled(true);
                break;
            }
            // setting about main window when showing.
            if (SCFG().contains(_MAIN_WINDOW_GEOMETRY_)) {
                this->setGeometry(SCFG().value(_MAIN_WINDOW_GEOMETRY_).toRect());
            } else {
                Geometry(showW->width(), showW->height() + (FONT_SIZE << 1));
            }
            if (SCFG().contains(_RECORD_DOCK_ISHIDDEN_)) {
                recdDock->setHidden(SCFG().value(_RECORD_DOCK_ISHIDDEN_).toBool());
            } else {
                recdDock->setFloating(true);
            }
            if (SCFG().contains(_MAIN_WINDOW_GEOMETRY_)
                && SCFG().value(_MAIN_WINDOW_MAXIMIZE_).toBool()) {
                this->showMaximized();
            }
            // setting about main window new show widget first
            if (once) {
                once = false;
                if (SCFG().contains(_RECORD_DOCK_GEOMETRY_)) {
                    recdDock->setGeometry(SCFG().value(_RECORD_DOCK_GEOMETRY_).toRect());
                } else {
                    recdDock->setGeometry(this->x() + 100, this->y() + 100, 300, 300);
                }
                if (SCFG().contains(_RECORD_DOCK_TRACKING_)) {
                    view_trace_all[SCFG().value(_RECORD_DOCK_TRACKING_).toInt()]
                        ->setChecked(true);
                } else {
                    view_trace_all[0]->setChecked(true);
                }
            }

            connect(showW, &QObject::destroyed, this, [view_trace_all]() {
                SCFG().setValue(
                    _RECORD_DOCK_TRACKING_,
                    (view_trace_all[2]->isChecked() << 1) | view_trace_all[1]->isChecked());
            });
        });
    };

    // connect signals.
    connect(file_mainMenu, &QAction::triggered, this, [this, ShowMainMenu, Geometry]() {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Return main menu", "Are you sure you want to return main menu ?",
            QMessageBox::Yes | QMessageBox::No
        );
        if (reply == QMessageBox::Yes) {
            this->SaveStateInfo();
            delete this->centralWidget();
            QCoreApplication::processEvents(QEventLoop::AllEvents); // clear all events.
            RECD().Reset();
            ShowMainMenu();
        }
    });
    connect(file_save, &QAction::triggered, this, [this, file_save]() {
        QString saveFilePath  = QFileDialog::getSaveFileName(
            this, "Save As", SHARE_PATH, "textfile (*.txt)");
        if (!saveFilePath.isEmpty()) { // Write File.
            QFile file(saveFilePath);
            bool success = file << RECD();
            if (success) {
                QMessageBox::StandardButton reply = QMessageBox::information(
                    this, "Save Success", QString("Save as '%1'.").arg(saveFilePath),
                    QMessageBox::Ok
                );
            } else { // comment out ? normally dosen't triggered.
                QMessageBox::StandardButton reply = QMessageBox::critical(
                    this, "Save Error", "Failed to wirte file.",
                    QMessageBox::Reset | QMessageBox::Cancel
                );
                if (reply == QMessageBox::Reset) {
                    emit file_save->triggered(); // maybe better to use while.
                }
             }
        }
    });
    connect(file_load, &QAction::triggered, this, [this, file_load]() {
        QString loadFilePath = QFileDialog::getOpenFileName(
            this, "Load From", SHARE_PATH, "textfile (*.txt)");
        if (!loadFilePath.isEmpty()) { // Read File.
            delete this->centralWidget();
            QCoreApplication::processEvents(QEventLoop::AllEvents); // clear all events.
            this->setCentralWidget(new ShowWidget(cpWay, cpWhich, cpBuff, cpMem, this));
            QFile file(loadFilePath);
            bool success = file >> RECD();
            if (success) {
                QMessageBox::StandardButton reply = QMessageBox::information(
                    this, "Load Success", QString("Load From '%1'.").arg(loadFilePath),
                    QMessageBox::Ok
                );
            } else {
                QMessageBox::critical(
                    this, "Load Error", "Failed to read file.",
                    QMessageBox::Ok
                );
            }
        }
    });
    connect(view_trace_group, &QActionGroup::triggered, this,
            [view_trace_group](QAction *action) {
        RECD().SetTrace(view_trace_group->actions().indexOf(action));
    });

    // something need do after connect.
    ShowMainMenu();
}

MainWindow::~MainWindow()
{
}

void MainWindow::SaveStateInfo()
{
    SCFG().setValue(_MAIN_WINDOW_MAXIMIZE_, this->isMaximized());
    SCFG().setValue(_MAIN_WINDOW_GEOMETRY_, this->normalGeometry());
    SCFG().setValue(_RECORD_DOCK_GEOMETRY_, recdDock->geometry());
    SCFG().setValue(_RECORD_DOCK_ISHIDDEN_, recdDock->isHidden());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (whichWindow == MyE::ShowWindow)
    {
        this->SaveStateInfo();
    }
}


