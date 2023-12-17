#include "mainwindow.h"
#include "showwidget.h"
#include "startwidget.h"

#include <QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setStyleSheet(QString("font: italic %1pt \"%2\";").arg(FONT_SIZE).arg(_FONT_));

    // add menu list
    auto file = this->menuBar()->addMenu("&File");

    // add menu action
    auto mainMenu = file->addAction("&Return Menu");

    auto Geometry = [this](int width, int height) {
        QSize screenSize = QApplication::primaryScreen()->size();
        int x = (screenSize.width() - width) >> 1;
        int y = (screenSize.height() - height) / 3;
        this->setGeometry(x, y, width, height);
    };

    auto ShowMainMenu = [this, Geometry]() {
        this->menuBar()->setVisible(false);
        auto startW = new StartWidget(this);
        this->setCentralWidget(startW);
        Geometry(startW->width(), startW->height());
        // go to show widget
        connect(startW, &StartWidget::start, this, [this, Geometry](int which, int buff, int mem) {
            this->menuBar()->setVisible(true);
            auto showW = new ShowWidget(which, buff, mem, this);
            this->setCentralWidget(showW);
            Geometry(showW->width(), showW->height() + (FONT_SIZE << 1));
        });
    };

    // return main menu
    connect(mainMenu, &QAction::triggered, this, [this, ShowMainMenu, Geometry]() {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Return main menu", "Are you sure you want to return main menu ?",
            QMessageBox::Yes | QMessageBox::No
            );
        if (reply == QMessageBox::Yes) {
            ShowMainMenu();
        }
    });

    ShowMainMenu();
}

MainWindow::~MainWindow()
{
}

