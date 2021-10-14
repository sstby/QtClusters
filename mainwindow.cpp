#include "mainwindow.h"
#include "ui_mainwindow.h"

//Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Clustering");
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
}
//Деструктор
MainWindow::~MainWindow()

{
    delete[] dataCoords;
    dataCoords = NULL;
    delete dlg;
    delete graphic;
    delete ui;
}
//Метод для вывода таблицы кластеров
void MainWindow::showClustersTable()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int rowsToInsert = dataInClasterQty[0];
    for (int i = 0; i < clastersQty; ++i) {
        if (dataInClasterQty[i] > rowsToInsert)
        {
            rowsToInsert = dataInClasterQty[i];
        }
    }


    for (int r = 0; r < rowsToInsert; ++r)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    }


    for (int i = 0, j = 0; i < clastersQty*2; ++i)
    {
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        QTableWidgetItem * item = new QTableWidgetItem( "Кластер "+QString::number((i+1)/2+1));
        item->setTextAlignment(Qt::AlignCenter);
        QFont font;
        font.setBold(true);
        item->setFont(font);
        //ui->tableWidget->setItem(0, i, new QTableWidgetItem( "Кластер "+QString::number((i+1)/2+1)));
        ui->tableWidget->setItem(0, i, item);
        if ( i % 2 != 0)
        {
            ui->tableWidget->setSpan(0,j,1,2);
            j = j + 2;

        }
    }
    for (int j = 0, col = 0, skip = 0; j < clastersQty; j++)
    {
        QTableWidgetItem * item_x = new QTableWidgetItem("X");
        QTableWidgetItem * item_y = new QTableWidgetItem("Y");
        item_x->setTextAlignment(Qt::AlignCenter);
        item_y->setTextAlignment(Qt::AlignCenter);
        QFont font;
        font.setBold(true);
        item_x->setFont(font);
        item_y->setFont(font);
        ui->tableWidget->setItem(1, col, item_x);
        ui->tableWidget->setItem(1, col+1, item_y);

        for (int i = 0; i < dataInClasterQty[j]; ++i)
        {

            QTableWidgetItem * item_x = new QTableWidgetItem(QString::number(dataCoords->m_Data[skip].x));
            QTableWidgetItem * item_y = new QTableWidgetItem(QString::number(dataCoords->m_Data[skip].y));
            item_x->setTextAlignment(Qt::AlignCenter);
            item_y->setTextAlignment(Qt::AlignCenter);
            font.setBold(false);
            item_x->setFont(font);
            item_y->setFont(font);
            ui->tableWidget->setItem(i+2, col, item_x);
            ui->tableWidget->setItem(i+2, col+1, item_y);
            skip++;
        }
        col = col + 2;
    }

    makeClusterTableStyle();

}

//Метод для очистки таблицы кластеров
void MainWindow::clearClustersTable()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}
//Метод для установки ширины ячеек в таблице кластеровв
void MainWindow::makeClusterTableStyle()
{
    //Ширина таблицы делиться на количество кластеров для равномерного отображения
    int w = ui->tableWidget->width()/clastersQty;
    for (int i = 0; i < clastersQty*2; i++)
    {
        ui->tableWidget->setColumnWidth(i, w/2);
    }

}



//Действие при нажатии на кнопку кластеризации
void MainWindow::on_pushButton_clicked()
{

    addVal = ui->AdditionalValue->value();//Считываеться дополнительное значение
    if (dataCoords->getDataCount() != 0 && addVal != 0)
    {
        clearClustersTable(); //Предварительная очистка таблицы кластеров

        //Создаем процес для запуска пайтон кода
        QProcess * pro = new QProcess(this);
        pro->start("python", QStringList()<<"clasters_script.py");
        pro->waitForStarted();
        pro->write(QString::number(method_ID).toUtf8()+QByteArray("\r\n"));//Ввод выбранного метода
        pro->write(QString::number(n_rows).toUtf8()+QByteArray("\r\n"));//Ввод количества точек

        //Ввод каждой точки
        for (int i = 0; i < n_rows;i++)
        {
            pro->write(QString::number(dataCoords->m_Data[i].x).toUtf8()+QByteArray("\r\n"));
            pro->write(QString::number(dataCoords->m_Data[i].y).toUtf8()+QByteArray("\r\n"));
        }

        pro->write(QString::number(addVal).toUtf8()+QByteArray("\r\n"));//Ввод дополнительной переменной
        pro->waitForFinished(-1);//Ожидание окончания выполнения пайтон скрипта

        //Для проверки вывода с Python
        qDebug() << "finished";
        QString p_stdout = pro->readAll();
        qDebug() << p_stdout;
        QString p_stderr = pro->readAllStandardError();
        if(!p_stderr.isEmpty())
         qDebug()<<"Python error:"<<p_stderr;

        //Предварительная очистка массива точек
        dataCoords->Clear();

        //Открытия файла, который был создан после выполннения скрипта
        QFile result("out.txt");
        QTextStream r(&result);
        if(!result.open(QIODevice::ReadOnly))
        {
            qWarning("Cannot open file for reading");
        }
        //Считывание первой строки файла, в которой указано количество кластеров и сколько точек в каждом кластере
        QString firstLine = r.readLine();
        QStringList firstLineSplit = firstLine.split(' ');
        clastersQty = firstLineSplit[0].toInt();
        dataInClasterQty = new int[clastersQty];
        for (int i = 0; i < clastersQty; i++)
        {
            dataInClasterQty[i] = firstLineSplit[i+1].toInt();
        }

        //Создание нового отсортированного массива точек координат
        dataCoords->Coordinates(n_rows);
        int i = 0;
        while(!r.atEnd())
        {
            QString line = r.readLine();
            QStringList l = line.split(' ');
            dataCoords->m_Data[i].x = l[0].toDouble();
            dataCoords->m_Data[i].y = l[1].toDouble();
            i++;

        }
        result.close();

        graphic = new graphicView();//Создание указателя на окно графика
        graphic->getData(dataCoords, clastersQty, dataInClasterQty);//Передача данных в класс графика
        graphic->setWindowTitle("Graphic");
        graphic->show();//Вызов окна графика

        showClustersTable();//Вызов функции заполнения таблицы данными
    }
    else
    {
        QMessageBox warning;
        warning.setIcon(QMessageBox::Warning);
        QString text;
        if (!dataCoords->getDataCount())
        {
            text += "Немає даних!\n";
        }

        if (addVal == 0)
        {
            switch (method_ID)
            {
            case 0:
                text += "Не введено порогове значення h";
                break;
            case 1:
                text += "Не введено значення гамма";
                break;
            case 2:
                text += "Не введено кiлькiсть кластерiв";
                break;
            }

        }
        warning.setText(text);
        warning.exec();

    }


}

//Действие при изменение метода кластеризации
void MainWindow::on_clasteriseMethod_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Метод найпростішого розташування центрів")
    {
        method_ID = 0;
        ui->methodLabel->setText("Порогове значення h");
        ui->AdditionalValue->setSingleStep(0.5);
    }
    else if (arg1 == "Метод максимінної відстані")
    {
        method_ID = 1;
        ui->methodLabel->setText("Значення гамма");
        ui->AdditionalValue->setSingleStep(0.1);
    }
    else if (arg1 == "Метод k-внутрішньогрупових середніх")
    {
        method_ID = 2;
        ui->methodLabel->setText("Кiлькiсть кластерiв");
        ui->AdditionalValue->setSingleStep(1);
    }

}

//Действие при ажатии кнопки Open
void MainWindow::on_actionOpen_triggered()
{
    if (n_rows != 0)
    {
        dataCoords->Clear();
        n_rows = 0;
    }

    //Вызов окна выбора файла и подсчет в этом файле количества строк
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Txt (*.txt)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileName;
    if (dialog.exec() && QDialog::Accepted == true)
    {
        fileName = dialog.selectedFiles();
        QFile count(fileName[0]);
        QTextStream c(&count);
        if(!count.open(QIODevice::ReadOnly))
        {
            qWarning("Cannot open file for reading");
        }
        //Подсчет точек
        while(!c.atEnd())
        {
            c.readLine();
            n_rows++;
        }
        count.close();
        dataCoords->Coordinates(n_rows);//Создание массива точек координат

        QFile file(fileName[0]);
        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning("Cannot open file for reading");
        }
        QTextStream in(&file);
        int i = 0;

        //Заполнение массива точек координат из файла
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList l = line.split(' ');
            dataCoords->m_Data[i].x = l[0].toDouble();
            dataCoords->m_Data[i].y = l[1].toDouble();
            i++;
        }
        file.close();
    }
    qDebug() << n_rows;


}

//Действие при нажатии NewData
void MainWindow::on_actionNew_Data_triggered()
{
    dlg = new NewDataDialog(this);//Создание указателя на диалоговое окно
    connect(dlg, SIGNAL(sendData(Data*)), SLOT(getData(Data*)));//Связываем диалогове окно с главным
    dlg->setWindowTitle("New Data");
    if(dlg->exec()){}//Открытие диалогового окна
}

//Функция получения данных из диалогово окна
void MainWindow::getData(Data *array)
{
    n_rows = array->getDataCount(); //Устанавливаем количество рядков равным количеству рядов в переданном масиве
    dataCoords = array; //Создаем массив
    update();//Обновляем

}


//Действие при нажатии Clear
void MainWindow::on_actionClear_triggered()
{

    Clear();

}


void MainWindow::on_actionClose_triggered()
{
    Clear();
    close();
}

void MainWindow::Clear()
{
    if (n_rows != 0)
    {
        dataCoords->Clear();
        clearClustersTable();
        ui->AdditionalValue->setValue(0.00);
    }

}


void MainWindow::on_actionEdit_triggered()
{
    dlg = new NewDataDialog(this);//Создание указателя на диалоговое окно
    connect(dlg, SIGNAL(sendData(Data*)), SLOT(getData(Data*)));//Связываем диалогове окно с главным
    dlg->setWindowTitle("Edit Data");
    dlg->getData(dataCoords, dataCoords->getDataCount());
    if(dlg->exec()){}//Открытие диалогового окна
}


void MainWindow::on_actionAutor_triggered()
{
    QMessageBox warning;
    warning.setIcon(QMessageBox::Information);
    warning.setText("Роботу виконала студентка групи 4-IC-30\nВелика Анастасія Андріївна");
    warning.exec();
}

