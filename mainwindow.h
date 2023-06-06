#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Semantico.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Abrir_clicked();

    void on_Salvar_clicked();

    void on_Correr_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_SavAssem_clicked();

private:
    Ui::MainWindow *ui;

    void show_simbol_table(Semantico semantico);
};

#endif // MAINWINDOW_H
