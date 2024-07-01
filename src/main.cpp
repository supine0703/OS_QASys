#include <QApplication>
#include <QFontDatabase>
#include "MainWindow.h"

auto getFontFamily = [](QStringList fl) {
    QStringList ff;
    for (const auto& f : fl)
    {
        auto i = QFontDatabase::addApplicationFont(f);
        ff << QFontDatabase::applicationFontFamilies(i).at(0);
    }
    return ff.join(",");
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QApplication::setFont(QFont(getFontFamily({
        ":/font/font/JetBrainsMonoNL-Medium.ttf",
        ":/font/font/YouSheShaYuFeiTeJianKangTi.ttf",
    }), 13));
    QApplication::setWindowIcon(QIcon(":/cpu.png"));

    MainWindow w;
    w.show();
    return a.exec();
}
