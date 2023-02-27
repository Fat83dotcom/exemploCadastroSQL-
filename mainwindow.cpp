#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "classes.h"
#include <vector>
#include <stdexcept>
#include <QTableView>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <QMessageBox>

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
    const string &declaracao, vector<string> v, int indicePreparacao){
    try {
        for (auto i : v) tbT->campos.push_back(i.c_str());
        c->executarPrepara(declaracao, tbT->campos, indicePreparacao);
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
        string nomeStr = nome.toStdString();
        if(ok && !nomeStr.empty()){
            vector<string> campos = {nome.toStdString(), idade.toStdString()};
            c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[0], campos, 0);
            ui->cadastroNome->clear();
            ui->cadastroNome->setFocus();
            ui->cadastroIdade->clear();
            ui->ConfimacaoCadastro->setText("Cadastro Realizado com Sucesso.");
        }
        else{
            if(!ok && nomeStr.empty()){
                throw std::invalid_argument("Os campos: Nome não pode estar\nvazio e "
                                            "Idade deve ser\num número inteiro.");
            }
            else if (nomeStr.empty()) {
                throw std::invalid_argument("O campo nome não pode\nestar vazio");
            }
            else{
                throw std::invalid_argument("O campo idade deve\nser um número inteiro");
            }
        }
    }
     catch (const exception &e) {
        QString erro = QString::fromStdString(e.what());
        ui->ConfimacaoCadastro->setText(erro);
        std::cout << erro.toStdString() << std::endl;
     }
}

void MainWindow::montadorPadraoTabela(int indiceDeclaracao, vector<string> argumentosConsulta){
    try {
        vector<string> itens;
        vector<vector<string>> resultado;
        QStandardItemModel *tabela = new QStandardItemModel(0, 3, ui->tabelaReultadoQuery);
        tabela->setHorizontalHeaderLabels({"Id", "Nome", "Idade"});
        resultado = c.imprimirPreparaResult(
                    tbT.declaracaoPrepare[indiceDeclaracao],
                    argumentosConsulta,
                    indiceDeclaracao);
        if (resultado.size() > 0) {
            for (int i = 0; i < resultado.size() ; i++ ) {
                QList<QStandardItem *> itensLinhas = {
                    new QStandardItem(QString::fromStdString(resultado[i][0])),
                    new QStandardItem(QString::fromStdString(resultado[i][1])),
                    new QStandardItem(QString::fromStdString(resultado[i][2]))
                };
                tabela->appendRow(itensLinhas);
            }
            ui->tabelaReultadoQuery->setModel(tabela);
            ui->tabelaReultadoQuery->setColumnWidth(0, 100);
            ui->tabelaReultadoQuery->setColumnWidth(1, 100);
            ui->tabelaReultadoQuery->setColumnWidth(2, 100);
            ui->tabelaReultadoQuery->show();
            ui->statusConsultas->setText("Consulta realizada com sucesso.");
        }
        else{
            ui->statusConsultas->setText("Consulta vazia.");
            tabela->clear();
            ui->tabelaReultadoQuery->update();
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaAll_clicked(){
    vector<string> argumentosConsulta = {};
    this->montadorPadraoTabela(1, argumentosConsulta);
}

void MainWindow::on_btnConsultaId_clicked(){
    try {
        bool ok;
        QString idConsulta = ui->entradaConsultaId->text();
        string idConsultaStr = idConsulta.toStdString();
        idConsulta.toInt(&ok);
        ui->entradaConsultaId->setFocus();
        ui->entradaConsultaId->clear();
        if (ok && !idConsultaStr.empty()) {
            vector<string> argumentosConsulta = {idConsultaStr};
            this->montadorPadraoTabela(2, argumentosConsulta);
        }
        else{
            if (idConsultaStr.empty()) {
                ui->statusConsultas->setText("Digite um número inteiro.");
            }
            else {
                ui->statusConsultas->setText("Somente números inteiros.");
            }
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaNome_clicked(){
    try {
        QString nomeConsulta = ui->entradaConsultaNome->text();
        string nomeConsultaStr = nomeConsulta.toStdString();
        ui->entradaConsultaNome->setFocus();
        ui->entradaConsultaNome->clear();
        if (!nomeConsultaStr.empty()) {
            vector<string> argumentosConsulta = {nomeConsultaStr};
            this->montadorPadraoTabela(3, argumentosConsulta);
        }
        else{
            ui->statusConsultas->setText("Digite o nome a ser procurado.");
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaIdade_clicked(){
    try {
        bool ok;
        QString idConsulta = ui->entradaConsultaIdade->text();
        string idConsultaStr = idConsulta.toStdString();
        idConsulta.toInt(&ok);
        ui->entradaConsultaIdade->setFocus();
        ui->entradaConsultaIdade->clear();
        if (ok && !idConsultaStr.empty()) {
            vector<string> argumentosConsulta = {idConsultaStr};
            this->montadorPadraoTabela(4, argumentosConsulta);
        }
        else{
            if (idConsultaStr.empty()) {
                ui->statusConsultas->setText("Digite um número inteiro.");
            }
            else {
                ui->statusConsultas->setText("Somente números inteiros.");
            }
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnDeletar_clicked(){
    try {
        QMessageBox::StandardButton confirmacao = QMessageBox::question(this,
                              "Deletar Por Id", "Esta ação é irreversível, deseja continuar?",
                              QMessageBox::Yes|QMessageBox::No);
        if (confirmacao == QMessageBox::Yes) {
            bool ok;
            QString idDelete = ui->entradaIdDelete->text();
            ui->entradaIdDelete->setFocus();
            ui->entradaIdDelete->clear();
            idDelete.toInt(&ok);
            if (ok){
                vector<string> idDeletados;
                idDeletados.push_back(idDelete.toStdString());
                c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[5], idDeletados, 5);
                this->on_btnConsultaAll_clicked();
            }
            else {
                ui->statusConsultas->setText("Somente números inteiros.");
            }
        }
        else {
            ui->entradaIdDelete->setFocus();
            ui->entradaIdDelete->clear();
            ui->statusConsultas->setText("Ação cancelada !");
        }
    }
    catch (const exception &e) {
        throw ;
    }

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
    try {
        QMessageBox::StandardButton confirmacao = QMessageBox::question(this,
                              "Deletar Selecionados", "Esta ação é irreversível, deseja continuar?",
                              QMessageBox::Yes|QMessageBox::No);
        if (confirmacao == QMessageBox::Yes) {
            vector<string> idsSelecionados;
            QStandardItemModel* tabela = qobject_cast<QStandardItemModel*>(ui->tabelaReultadoQuery->model());
            ui->tabelaReultadoQuery->setModel(tabela);
            QItemSelectionModel *tabelaSelecionada = ui->tabelaReultadoQuery->selectionModel();
            QModelIndexList linhasSelecionadas = tabelaSelecionada->selectedRows();
            foreach (QModelIndex i, linhasSelecionadas) {
                QStandardItem* itemSelecionado = tabela->itemFromIndex(i);
                QString id = itemSelecionado->text();
                string idStr = id.toStdString();
                idsSelecionados.push_back(idStr);
            }
            for (int i = 0; i < idsSelecionados.size(); i++) {
                vector<string> buffer;
                buffer.push_back(idsSelecionados[i]);
                c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[5], buffer, 5);
            }
            this->on_btnConsultaAll_clicked();
            int qtdLInhasselecionadas = linhasSelecionadas.size();
            if (qtdLInhasselecionadas > 0) {
                ui->statusConsultas->setText(QString("Os registros foram deletados com\n sucesso.\n"
                                                  "Total de registros deletados: %1"
                                                     ).arg(qtdLInhasselecionadas));
            }
            else {
                ui->statusConsultas->setText(QString("Nenhum item foi selecionado.\n"
                                                     "Total de registros deletados: %1"
                                                        ).arg(qtdLInhasselecionadas));
            }
        }
        else{
            ui->entradaIdDelete->setFocus();
            ui->entradaIdDelete->clear();
            ui->statusConsultas->setText("Ação cancelada !");
        }

    }  catch (const exception &e) {
        throw;
    }

}

void MainWindow::on_btnConsultaPalavraChave_clicked(){
    try {
        string pChave = ui->entradaConsultaPChave->text().toStdString();
        ui->entradaConsultaPChave->setFocus();
        ui->entradaConsultaPChave->clear();
        if (!pChave.empty()) {
            pChave = "%" + pChave + "%";
            vector<string> container = {pChave};
            this->montadorPadraoTabela(6, container);
        }
        else {
            ui->statusConsultas->setText("Digite um nome ou parte dele.");
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_cadastroNome_returnPressed(){
    this->on_cadastrarDados_clicked();
}

void MainWindow::on_cadastroIdade_returnPressed(){
    this->on_cadastrarDados_clicked();
}

void MainWindow::on_entradaConsultaId_returnPressed(){
    this->on_btnConsultaId_clicked();
}

void MainWindow::on_entradaConsultaNome_returnPressed(){
    this->on_btnConsultaNome_clicked();
}

void MainWindow::on_entradaConsultaIdade_returnPressed(){
    this->on_btnConsultaIdade_clicked();
}

void MainWindow::on_entradaIdDelete_returnPressed(){
    this->on_btnDeletar_clicked();
}

void MainWindow::on_entradaConsultaPChave_returnPressed(){
    this->on_btnConsultaPalavraChave_clicked();
}

