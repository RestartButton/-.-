#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
