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
#include <QDebug>
void MainWindow::on_Correr_clicked()
{
    try{
        Lexico lexico = Lexico(ui->Codigo->toPlainText().toStdString());

        Sintatico sintatico = Sintatico();
        Semantico semantico = Semantico();
        sintatico.parse(&lexico, &semantico);

        std::string output = "";

        while(!semantico.lista_warnings->empty()) {
            output += semantico.lista_warnings->top() + "\n";
            semantico.lista_warnings->pop();
        }
        auto tabela = semantico.tabela_simbolos;
        for( list<Simbolo>::iterator it = tabela->begin(); it != tabela->end(); it++) {
            if(!it->usada) {
                output += "Variavel declarada mas nao utilizada: " + it->nome + "\n";
            }
            std::cout << it->nome << " "  << it->escopo << " "  << it->func << " "  <<  it->vet << " "  << it->usada << " " << it->inic << " \n";
            qDebug() << QString("Nome: %1 | Escopo: %2 | Func: %3 | Vet: %4 | Usada: %5 | Inicializada: %6 | Tipo: %7")
                        .arg(QString::fromStdString(it->nome))
                        .arg(QString::fromStdString(it->escopo))
                        .arg(it->func)
                        .arg(it->vet)
                        .arg(it->usada)
                        .arg(it->inic)
                        .arg(QString::fromStdString(it->tipo));
        }

        output += "Comando aceito!\n";

        ui->Terminal->setPlainText(QString(output.c_str()));
    } catch ( LexicalError e) {
        ui->Terminal->clear();
        ui->Terminal->setPlainText(QString("Comando nao identificado! \n%1").arg(e.getMessage()));
    } catch ( SyntaticError e) {
        ui->Terminal->clear();
        ui->Terminal->setPlainText(QString("Comando nao identificado! \n%1").arg(e.getMessage()));
    } catch ( SemanticError e) {
        ui->Terminal->clear();
        ui->Terminal->setPlainText(QString("Comando nao identificado! \n%1").arg(e.getMessage()));
    }
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{

}
