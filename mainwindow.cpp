#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "classes.h"
#include <vector>
#include <stdexcept>
#include <QTableView>
#include <QStandardItemModel>

using std::exception;

ConectBD c;
TabelaTeste tbT;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
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
        idade.toInt(&ok);
        if(ok){
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
        std::cout << erro.toStdString() << std::endl;
     }
}

void MainWindow::on_btnConsultaAll_clicked(){
    try {
        QStandardItemModel *modelo = new QStandardItemModel(0, 3, ui->tabelaReultadoQuery);
        modelo->clear();
        modelo->setHorizontalHeaderLabels({"Id", "Nome", "Idade"});
        vector<string>argumentos, itens;
        vector<vector<string>> resultado;
        argumentos = {};
        resultado = c.imprimirPreparaResult(tbT.declaracaoPrepare[1], argumentos);
        for (int i = 0; i < resultado.size() ; i++ ) {
            QList<QStandardItem *> itensLinhas = {
                new QStandardItem(QString::fromStdString(resultado[i][0])),
                new QStandardItem(QString::fromStdString(resultado[i][1])),
                new QStandardItem(QString::fromStdString(resultado[i][2]))
            };
            modelo->appendRow(itensLinhas);
        }
        ui->tabelaReultadoQuery->setModel(modelo);
        ui->tabelaReultadoQuery->show();
    }
    catch (const exception &e) {
        throw;
    }

}

void MainWindow::on_btnConsultaId_clicked(){

}

void MainWindow::on_btnConsultaNome_clicked(){

}

void MainWindow::on_btnConsultaIdade_clicked(){

}

void MainWindow::on_btnDeletar_clicked(){

}

void MainWindow::on_alterarEntradaBD_clicked(){
    try {
        ArquivoBD a;
        string usuarioBD, hostBD, senhaBD, portaBD;
        usuarioBD = ui->entradaUsuarioBD->text().toStdString();
        hostBD = ui->entradaHostBD->text().toStdString();
        senhaBD = ui->entradaSenhaBD->text().toStdString();
        portaBD = ui->entradaPortaBD->text().toStdString();
        if(usuarioBD.empty() || hostBD.empty() || senhaBD.empty() || portaBD.empty()){
            ui->statusTransacao->setText("Só é possivel definir com as 4 entradas preenchidas.");
            ui->entradaHostBD->clear();
            ui->entradaUsuarioBD->clear();
            ui->entradaSenhaBD->clear();
            ui->entradaPortaBD->clear();
        }
        else{
            string login;
            login = a.montarEntradaBD(usuarioBD, hostBD, senhaBD, portaBD);
            a.gravarArquivo(a.arquivoLogin, login);
            ui->entradaHostBD->clear();
            ui->entradaUsuarioBD->clear();
            ui->entradaSenhaBD->clear();
            ui->entradaPortaBD->clear();
            ui->statusTransacao->setText("Dados salvos com sucesso!");
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnDeletarSelecionados_clicked(){

}


void MainWindow::on_cadastroNome_returnPressed(){
    this->on_cadastrarDados_clicked();
}


void MainWindow::on_cadastroIdade_returnPressed(){
    this->on_cadastrarDados_clicked();
}

