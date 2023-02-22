#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include "classes.h"
#include "confidencial.h"
#include <QMainWindow>
using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cadastrarDados_clicked();

    void on_btnConsultaAll_clicked();

    void on_btnConsultaId_clicked();

    void on_btnConsultaNome_clicked();

    void on_btnConsultaIdade_clicked();

    void on_btnDeletar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
