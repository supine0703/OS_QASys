#include "startwidget.h"
#include "ui_startwidget.h"
#include "recordwidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include "myenum.h"
#include "mysettings.h"

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    // Read 'config.ini' File.
    if (SCFG().contains(PRODUCE_WAY))
        ui->comboBox_way->setCurrentIndex(SCFG().value(PRODUCE_WAY).toInt());
    if (SCFG().contains(_ALGORITHM_))
        ui->comboBox_algorithm->setCurrentIndex(SCFG().value(_ALGORITHM_).toInt());
    if (SCFG().contains(BUFFER_SIZE))
        ui->spinBox_buffer->setValue(SCFG().value(BUFFER_SIZE).toInt());
    if (SCFG().contains(MEMORY_SIZE))
        ui->spinBox_memory->setValue(SCFG().value(MEMORY_SIZE).toInt());
    if (SCFG().contains(PRODUCT_FILE_PATH))
        ui->lineEdit_path->setText(SCFG().value(PRODUCT_FILE_PATH).toString());
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::on_pushButton_clicked()
{
    // Check read product file.
    QFile out(ui->lineEdit_path->text());
    if (ui->comboBox_way->currentIndex() == MyE::RdFile && !(out >> RECD()))
    {
        QMessageBox::critical(
            this, "Read File Failed", "Failed to read product file.",
            QMessageBox::Ok
        );
        return;
    }
    // Write 'config.ini' File.
    SCFG().setValue(PRODUCE_WAY, ui->comboBox_way->currentIndex());
    SCFG().setValue(_ALGORITHM_, ui->comboBox_algorithm->currentIndex());
    SCFG().setValue(BUFFER_SIZE, ui->spinBox_buffer->value());
    SCFG().setValue(MEMORY_SIZE, ui->spinBox_memory->value());
    SCFG().setValue(PRODUCT_FILE_PATH, ui->lineEdit_path->text());

    emit start(
        ui->comboBox_way->currentIndex(),
        ui->comboBox_algorithm->currentIndex(),
        ui->spinBox_buffer->value(),
        ui->spinBox_memory->value()
    );
}

void StartWidget::on_comboBox_way_currentIndexChanged(int index)
{
    switch (index)
    {
    case MyE::Random: // random.
        if (ui->comboBox_algorithm->count() == MyE::ALG_NUM)
            ui->comboBox_algorithm->removeItem(MyE::OPT);
        ui->lineEdit_path->setEnabled(false);
        ui->toolButton_path->setEnabled(false);
        break;
    case MyE::RdFile: // read file.
        if (ui->comboBox_algorithm->count() == MyE::ALG_NUM - 1)
            ui->comboBox_algorithm->insertItem(MyE::OPT, "OPT");
        ui->lineEdit_path->setEnabled(true);
        ui->toolButton_path->setEnabled(true);
        break;
    }
}

void StartWidget::on_toolButton_path_clicked()
{
    QFileInfo fileInfo(ui->lineEdit_path->text());
    auto filePath = fileInfo.absolutePath();
    QString openFilePath  = QFileDialog::getOpenFileName(
        this, "Open As", fileInfo.exists() ? filePath : SHARE_PATH, "textfile (*.txt)");
    if (!openFilePath.isEmpty())
        ui->lineEdit_path->setText(openFilePath);
}

