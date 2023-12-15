#include "startwidget.h"
#include "ui_startwidget.h"

#include <QFile>
#include <QTextStream>

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    // Rear File
    QFile file(CONFIG_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line = in.readLine();
        auto data = line.split(" ");
        file.close();
        ui->comboBox->setCurrentIndex(data[0].toInt());
        ui->spinBox_buffer->setValue(data[1].toInt());
        ui->spinBox_memory->setValue(data[2].toInt());
    }
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::on_pushButton_clicked()
{
    QFile file(CONFIG_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->comboBox->currentIndex() << " "
            << ui->spinBox_buffer->value() << " "
            << ui->spinBox_memory->value() << Qt::endl;
        file.close();
    }
    emit start(
        ui->comboBox->currentIndex(),
        ui->spinBox_buffer->value(),
        ui->spinBox_memory->value()
    );
}

