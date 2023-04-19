#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "Lexico.h"
#include "Sintatico.h"
#include "Semantico.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::console(const std::string text){
    QString qtext = QString::fromStdString(text);
    ui->Terminal->setPlainText(text);
}
*/
void MainWindow::on_Abrir_clicked()
{
    QString fileContent;

       QString filename= QFileDialog::getOpenFileName(this, "Choose File");


      if(filename.isEmpty())
          return;

      QFile file(filename);

      if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
          return;

      QTextStream in(&file);

      fileContent = in.readAll();

      file.close();

      ui->Codigo->clear();
      ui->Codigo->setPlainText(fileContent);
}

void MainWindow::on_Salvar_clicked()
{

    QString filename= QFileDialog::getSaveFileName(this, "Save As");

        if (filename.isEmpty())
            return;

        QFile file(filename);


        //Open the file
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);

        out << ui->Codigo->toPlainText() << "\n";

        file.close();

}

void MainWindow::on_Correr_clicked()
{
    try{
        Lexico lexico = Lexico(ui->Codigo->toPlainText().toStdString());

        Sintatico sintatico = Sintatico();
        Semantico semantico = Semantico();
        sintatico.parse(&lexico, &semantico);
        ui->Terminal->setPlainText("Comando aceito!\n");
    } catch ( AnalysisError e) {
        ui->Terminal->clear();
        ui->Terminal->setPlainText(QString("Comando nao identificado! \n%1").arg(e.getMessage()));
    }
}
