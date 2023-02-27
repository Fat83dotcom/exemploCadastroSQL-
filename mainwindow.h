#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include "classes.h"
#include "confidential.h"
#include <QMainWindow>
#include <QListWidgetItem>
using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void montadorPadraoTabela(int indiceDeclaracao, vector<string> argumentosConsulta);

private slots:
    void on_cadastrarDados_clicked();

    void on_btnConsultaAll_clicked();

    void on_btnConsultaId_clicked();

    void on_btnConsultaNome_clicked();

    void on_btnConsultaIdade_clicked();

    void on_btnDeletar_clicked();

    void on_alterarEntradaBD_clicked();

    void on_btnDeletarSelecionados_clicked();

    void on_cadastroNome_returnPressed();

    void on_cadastroIdade_returnPressed();

    void on_entradaConsultaId_returnPressed();

    void on_entradaConsultaNome_returnPressed();

    void on_entradaConsultaIdade_returnPressed();

    void on_entradaIdDelete_returnPressed();

    void on_btnConsultaPalavraChave_clicked();

    void on_entradaConsultaPChave_returnPressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
