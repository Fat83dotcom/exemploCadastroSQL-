#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "classes.h"
#include <vector>
#include <stdexcept>

using std::exception;

ConectBD c;
TabelaTeste tbT;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
    c.inciarSQLTabelaTeste(tbT);
}

MainWindow::~MainWindow(){
    delete ui;
}

void ConectBD::executarTabelaTeste(ConectBD *c, TabelaTeste *tbT,
    const string &declaracao, vector<string> v){
    try {
        for (auto i : v) tbT->campos.push_back(i.c_str());
        c->executarPrepara(declaracao, tbT->campos);
        tbT->campos.clear();
    }
      catch (const exception &e) {
        throw;
     }
}

void MainWindow::on_cadastrarDados_clicked(){
    try {
        bool ok;
        QString nome = ui->cadastroNome->text();
        QString idade = ui->cadastroIdade->text();
        if(idade.toInt(&ok)){
            vector<string> campos = {nome.toStdString(), idade.toStdString()};
            c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[0], campos);
            ui->cadastroIdade->clear();
            ui->cadastroNome->clear();
            ui->ConfimacaoCadastro->setText("Cadastro Realizado com Sucesso.");
        }
        else{
            throw std::invalid_argument("Idade deve ser um número inteiro.");
        }
    }
     catch (const exception &e) {
        QString erro = QString::fromStdString(e.what());
        ui->ConfimacaoCadastro->setText(erro);
     }
}

void MainWindow::on_btnConsultaAll_clicked(){

}

void MainWindow::on_btnConsultaId_clicked(){

}

void MainWindow::on_btnConsultaNome_clicked(){

}

void MainWindow::on_btnConsultaIdade_clicked(){

}

void MainWindow::on_btnDeletar_clicked(){

}

