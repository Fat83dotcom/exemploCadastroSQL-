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
#include <QLabel>
#include<QStandardItem>

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

void MainWindow::preparaProximaEntrada(QLineEdit *caixaText){
    caixaText->clear();
    caixaText->setFocus();
}

string MainWindow::converteEntradaParaString(QLineEdit *caixaEntrda){
    string entradaDados = caixaEntrda->text().toStdString();
    return entradaDados;
}

string MainWindow::verificaEntradaInteiro(QLineEdit *caixaEntrda, bool *verificadorInteiro){
    QString dados = caixaEntrda->text();
    dados.toInt(verificadorInteiro);
    string caixaEntradaStr = dados.toStdString();
    return caixaEntradaStr;
}

void MainWindow::limpaCaixaEntrada(QLineEdit *caixaEntrada){
    caixaEntrada->clear();
}

void MainWindow::atualizarLabelStatus(QLabel *labelStatus, QString mensagem){
    labelStatus->setText(mensagem);
}

void MainWindow::apagarTabela(QTableView *tabela){
    tabela->setModel(NULL);
}

void MainWindow::inserirDadosLinhas(QStandardItemModel *tabela, vector<vector<string>> resultadoConsulta, int i){
    QList<QStandardItem *> linhasInseridas = {
        new QStandardItem(QString::fromStdString(resultadoConsulta[i][0])),
        new QStandardItem(QString::fromStdString(resultadoConsulta[i][1])),
        new QStandardItem(QString::fromStdString(resultadoConsulta[i][2]))
    };
        tabela->appendRow(linhasInseridas);
}

void MainWindow::mostrarTabela(QStandardItemModel *modeloTabela,
                               QTableView *tabela,
                               const QStringList nomeColunas){
    modeloTabela->setHorizontalHeaderLabels(nomeColunas);
    tabela->setModel(modeloTabela);
    tabela->show();
}

void MainWindow::on_cadastrarDados_clicked(){
    try {
        bool ok;
        string nomeStr = this->converteEntradaParaString(ui->cadastroNome);
        string idadeStr = this->verificaEntradaInteiro(ui->cadastroIdade, &ok);
        if(ok && !nomeStr.empty()){
            vector<string> campos = {nomeStr, idadeStr};
            c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[0], campos, 0);
            this->preparaProximaEntrada(ui->cadastroNome);
            this->limpaCaixaEntrada(ui->cadastroIdade);
            this->atualizarLabelStatus(ui->ConfimacaoCadastro, QString("Cadastro Realizado com Sucesso."));
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
        this->atualizarLabelStatus(ui->ConfimacaoCadastro, erro);
        std::cout << erro.toStdString() << std::endl;
     }
}

void MainWindow::montadorTabelaPadrao(int indiceDeclaracao, vector<string> argumentosConsulta){
    try {
        QStandardItemModel *modeloTabela = new QStandardItemModel(0, 3, ui->tabelaReultadoQuery);
        vector<vector<string>> resultadoConsulta;
        resultadoConsulta = c.resultadoConsultaPrepara(
                    tbT.declaracaoPrepare[indiceDeclaracao],
                    argumentosConsulta,
                    indiceDeclaracao);
        if (resultadoConsulta.size() > 0) {
            for (int i = 0; i < resultadoConsulta.size() ; i++ ) {
                this->inserirDadosLinhas(modeloTabela, resultadoConsulta, i);
            }
            this->mostrarTabela(modeloTabela, ui->tabelaReultadoQuery, {"Id", "Nome", "Idade"});
            this->atualizarLabelStatus(ui->statusConsultas, QString("Consulta realizada com sucesso."));
        }
        else{
            this->atualizarLabelStatus(ui->statusConsultas, QString("Consulta vazia."));
            this->apagarTabela(ui->tabelaReultadoQuery);
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaAll_clicked(){
    vector<string> argumentosConsulta = {};
    this->montadorTabelaPadrao(1, argumentosConsulta);
}

void MainWindow::on_btnConsultaId_clicked(){
    try {
        bool ok;
        string idConsultaStr = this->verificaEntradaInteiro(ui->entradaConsultaId, &ok);
        this->preparaProximaEntrada(ui->entradaConsultaId);
        if (ok && !idConsultaStr.empty()) {
            vector<string> argumentosConsulta = {idConsultaStr};
            this->montadorTabelaPadrao(2, argumentosConsulta);
        }
        else{
            if (idConsultaStr.empty()) {
                this->atualizarLabelStatus(ui->statusConsultas, QString("Digite um número inteiro."));
            }
            else {
                this->atualizarLabelStatus(ui->statusConsultas, QString("Somente números inteiros."));
            }
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaNome_clicked(){
    try {
        string nomeConsultaStr = this->converteEntradaParaString(ui->entradaConsultaNome);
        this->preparaProximaEntrada(ui->entradaConsultaNome);
        if (!nomeConsultaStr.empty()) {
            vector<string> argumentosConsulta = {nomeConsultaStr};
            this->montadorTabelaPadrao(3, argumentosConsulta);
        }
        else{
            this->atualizarLabelStatus(ui->statusConsultas, QString ("Digite o nome a ser procurado."));
        }
    }
    catch (const exception &e) {
        throw;
    }
}

void MainWindow::on_btnConsultaIdade_clicked(){
    try {
        bool ok;
        string idConsultaStr = verificaEntradaInteiro(ui->entradaConsultaIdade, &ok);
        this->preparaProximaEntrada(ui->entradaConsultaIdade);
        if (ok && !idConsultaStr.empty()) {
            vector<string> argumentosConsulta = {idConsultaStr};
            this->montadorTabelaPadrao(4, argumentosConsulta);
        }
        else{
            if (idConsultaStr.empty()) {
                this->atualizarLabelStatus(ui->statusConsultas, QString("Digite um número inteiro."));
            }
            else {
                this->atualizarLabelStatus(ui->statusConsultas, QString("Somente números inteiros."));
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
            string idDeleteStr = this->verificaEntradaInteiro(ui->entradaIdDelete, &ok);
            this->preparaProximaEntrada(ui->entradaIdDelete);
            if (ok){
                vector<string> idDeletados;
                idDeletados.push_back(idDeleteStr);
                c.executarTabelaTeste(&c, &tbT, tbT.declaracaoPrepare[5], idDeletados, 5);
                this->on_btnConsultaAll_clicked();
            }
            else {
                this->atualizarLabelStatus(ui->statusConsultas, QString("Somente números inteiros."));
            }
        }
        else {
            this->atualizarLabelStatus(ui->statusConsultas, QString("Ação cancelada !"));
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
        usuarioBD = this->converteEntradaParaString(ui->entradaUsuarioBD);
        hostBD = this->converteEntradaParaString(ui->entradaHostBD);
        senhaBD = this->converteEntradaParaString(ui->entradaSenhaBD);
        portaBD = this->converteEntradaParaString(ui->entradaPortaBD);
        if(usuarioBD.empty() || hostBD.empty() || senhaBD.empty() || portaBD.empty()){        
            this->preparaProximaEntrada(ui->entradaUsuarioBD);
            this->limpaCaixaEntrada(ui->entradaHostBD);
            this->limpaCaixaEntrada(ui->entradaSenhaBD);
            this->limpaCaixaEntrada(ui->entradaPortaBD);
            this->atualizarLabelStatus(ui->statusTransacao, QString("Só é possivel definir com as 4 entradas preenchidas."));
        }
        else{
            string login;
            login = a.montarEntradaBD(usuarioBD, hostBD, senhaBD, portaBD);
            a.gravarArquivo(a.arquivoLogin, login);
            this->preparaProximaEntrada(ui->entradaUsuarioBD);
            this->limpaCaixaEntrada(ui->entradaHostBD);
            this->limpaCaixaEntrada(ui->entradaSenhaBD);
            this->limpaCaixaEntrada(ui->entradaPortaBD);
            this->atualizarLabelStatus(ui->statusTransacao, QString("Dados salvos com sucesso!"));
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
            // Os coringas do operador Like devem ser colocado dentro das variaveis, não no comando SQL
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

