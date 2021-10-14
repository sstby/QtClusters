#ifndef NEWDATADIALOG_H
#define NEWDATADIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include "data.h"

namespace Ui {
class NewDataDialog;
}
//Класс диалогового окна для создания новых точек координат
class NewDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDataDialog(QWidget *parent = nullptr);//Конструктор
    ~NewDataDialog();//Деструктор
    void getData(Data* data, int length);
    void showDataTable();
    void setTableWidth();
    void toSave();
    int n_rows = 0;

private slots:
    //Действие при нажатии кнопки ОК
    void on_pushButton_Accepted_clicked();

    void on_pushButton_clicked();

    //Дейтсвие при нажатии кнопки Добавить точку
    void on_addDot_clicked();



    void on_pushButton_Rejected_clicked();

    void on_pushButton_DeleteRow_clicked();

private:
    Ui::NewDataDialog *ui;
    Data dataCoords;//Инициализация массива точек координат
signals:
    void sendData(Data* array);//Функци отправки данных в основно окно
protected:
    virtual void keyPressEvent(QKeyEvent * event);

};

#endif // NEWDATADIALOG_H
