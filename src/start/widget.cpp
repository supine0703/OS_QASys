#include "widget.h"
#include "ui_startwidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include "record/widget.h"
#include "utils/config.h"
#include "utils/g_enum.h"

#define PRODUCE_WAY       "/option/produce_way"
#define _ALGORITHM_       "/option/_algorithm_"
#define BUFFER_SIZE       "/option/buffer_size"
#define MEMORY_SIZE       "/option/memory_size"
#define PRODUCT_FILE_PATH "/option/product_file_path"

#define BETWEEN(L, R) \
    [](auto v) { \
        bool ok; \
        auto i = v.toInt(&ok); \
        return (ok && L <= i && i < R); \
    }

StartWidget::StartWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    // register
    CFG().reg({
        { PRODUCE_WAY, { 0, BETWEEN(0, 2) } },
        { _ALGORITHM_, { 0, BETWEEN(0, 4) } },
        { BUFFER_SIZE, { 8, BETWEEN(4, 64) } },
        { MEMORY_SIZE, { 8, BETWEEN(4, 64) } },
        { PRODUCT_FILE_PATH, { "" } }
    });

    // Load config
    ui->comboBox_way->setCurrentIndex(CFG()[PRODUCE_WAY].toInt());
    ui->comboBox_algorithm->setCurrentIndex(CFG()[_ALGORITHM_].toInt());
    ui->spinBox_buffer->setValue(CFG()[BUFFER_SIZE].toInt());
    ui->spinBox_memory->setValue(CFG()[MEMORY_SIZE].toInt());
    ui->lineEdit_path->setText(CFG()[PRODUCT_FILE_PATH].toString());
}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::on_pushButton_clicked()
{
    // Check read product file.
    QFile out(ui->lineEdit_path->text());
    if (ui->comboBox_way->currentIndex() == G_E::RdFile)
    {
        if (ui->lineEdit_path->text().isEmpty() || !(out >> RECD()))
        {
            QMessageBox::critical(
                this, "Read File Failed", "Failed to read product file.",
                QMessageBox::Ok
            );
            return;
        }
    }

    // update config and emit
    emit start(
        (CFG()[PRODUCE_WAY] = ui->comboBox_way->currentIndex()).toInt(),
        (CFG()[_ALGORITHM_] = ui->comboBox_algorithm->currentIndex()).toInt(),
        (CFG()[BUFFER_SIZE] = ui->spinBox_buffer->value()).toInt(),
        (CFG()[MEMORY_SIZE] = ui->spinBox_memory->value()).toInt()
    );
    CFG()[PRODUCT_FILE_PATH] = ui->lineEdit_path->text();
}

void StartWidget::on_comboBox_way_currentIndexChanged(int index)
{
    switch (index)
    {
    case G_E::Random: // random.
        if (ui->comboBox_algorithm->count() == G_E::ALG_NUM)
        {
            ui->comboBox_algorithm->removeItem(G_E::OPT);
        }
        ui->lineEdit_path->setEnabled(false);
        ui->toolButton_path->setEnabled(false);
        break;
    case G_E::RdFile: // read file.
        if (ui->comboBox_algorithm->count() == G_E::ALG_NUM - 1)
        {
            ui->comboBox_algorithm->insertItem(G_E::OPT, "OPT");
        }
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

