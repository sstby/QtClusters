#include "newdatadialog.h"
#include "ui_newdatadialog.h"

//Консутрктор
NewDataDialog::NewDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDataDialog)
{
    ui->setupUi(this);
    //Создание названий столбоцов в таблице
    QStringList title;
    title << "X" << "Y";
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(title);
    connect(this, SIGNAL(accepted()), SLOT(on_pushButton_Accepted_clicked()));//Соединение основного и диалового окна
    setTableWidth();
}
//Деструктор
NewDataDialog::~NewDataDialog()
{
    delete ui;
}

void NewDataDialog::getData(Data *data, int length)
{
    n_rows = length;
    //qDebug() << n_rows;
    dataCoords.Coordinates(n_rows);
    for (int i = 0; i < n_rows; ++i) {
        dataCoords.m_Data[i].x = data->m_Data[i].x;
        dataCoords.m_Data[i].y = data->m_Data[i].y;
    }

    showDataTable();
}
void NewDataDialog::showDataTable()
{
    for (int i = 0; i < n_rows; ++i) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());//Вставляем новыю строку
        QTableWidgetItem * item_x = new QTableWidgetItem(QString::number(dataCoords.m_Data[i].x));
        QTableWidgetItem * item_y = new QTableWidgetItem(QString::number(dataCoords.m_Data[i].y));
        item_x->setTextAlignment(Qt::AlignCenter);
        item_y->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, item_x);
        ui->tableWidget->setItem(i, 1, item_y);
    }
}

void NewDataDialog::setTableWidth()
{
    //Ширина таблицы делиться на количество кластеров для равномерного отображения
    int w = ui->tableWidget->width()-19;
    for (int i = 0; i < 2; i++)
    {
        ui->tableWidget->setColumnWidth(i, w/2);
    }
}

void NewDataDialog::toSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Txt files (*.txt)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("text"), file.errorString());
            return;
        }
        else
        {
            qDebug() << "Попытка записи";
            QTextStream out(&file);
            for (int i = 0; i < dataCoords.getDataCount(); ++i) {
                out  << dataCoords.m_Data[i].x << " " << dataCoords.m_Data[i].y ;
                if (i!= dataCoords.getDataCount()-1) out << "\n" ;
            }

        }
        file.close();
    }

}

//Действие при нажатии кнопки Добавить точку
void NewDataDialog::on_addDot_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());//Вставляем новыю строку
    int c_row = ui->tableWidget->rowCount() - 1;//Указываем текущий ряд
    for (int i = 0;i < 2 ;i++ ) {
        ui->tableWidget->setItem(c_row, i, new QTableWidgetItem(0)); //Вставляем в каждую новую ячейку пустое значение
    }
}


void NewDataDialog::on_pushButton_clicked()
{
    for (int j = 0; j < ui->spinBox->value(); ++j) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());//Вставляем новыю строку
        int c_row = ui->tableWidget->rowCount() - 1;//Указываем текущий ряд
        for (int i = 0;i < 2 ;i++ ) {
            ui->tableWidget->setItem(c_row, i, new QTableWidgetItem(0)); //Вставляем в каждую новую ячейку пустое значение
        }
    }

}


void NewDataDialog::on_pushButton_Accepted_clicked()
{

    int rows = ui->tableWidget->rowCount();//Указываем количество строк
    dataCoords.Coordinates(rows);//Преобразовываем в массив данных
    for(int i = 0;i < rows; i++)
    {
        for(int j = 0;j < 2; j++)
        {

            if(ui->tableWidget->item(i,j)->text().isEmpty())
            {
                QMessageBox::critical(this, "Error", "Заполнены не все ячейки");//Проверка на пустые ячейки
                return;
            }
        }
        //Заполняем  массив данными каждого рядка
        dataCoords.m_Data[i].x = ui->tableWidget->item(i,0)->text().toInt();
        dataCoords.m_Data[i].y = ui->tableWidget->item(i,1)->text().toInt();

    }
    if (ui->radioButton->isChecked())
    {
        toSave();
    }
    emit sendData(&dataCoords); //Отправляем массив и цвет в основное окно
    this->close();//Закрываем диалоговое окно
}



void NewDataDialog::on_pushButton_Rejected_clicked()
{
    this->close();
}


void NewDataDialog::on_pushButton_DeleteRow_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    //qDebug() << ui->tableWidget->selectedItems().length();
}

void NewDataDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key = Qt::Key_Delete && ui->tableWidget->selectedItems().length() == 2)
    {
        on_pushButton_DeleteRow_clicked();
    }
}

