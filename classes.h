#ifndef CONECPOSTGRES_H
#define CONECPOSTGRES_H

#include <string>
#include <pqxx/pqxx>
#include <pqxx/transaction_base>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "confidential.h"


using std::getline;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::string;
using std::vector;
using std::exception;

typedef struct TabelaTestes{

    const string sqlInsertTTeste = "INSERT INTO teste (nome, idade) VALUES($1, $2)";
    const string sqlSelectTTestAll = "SELECT * FROM teste";
    const string sqlSelectTTestId = "SELECT * FROM teste WHERE codigo=$1";
    const string sqltSelectTTestNome = "SELECT * FROM teste WHERE nome=$1";
    const string sqltSelectTTestIdade = "SELECT * FROM teste WHERE idade=$1";
    const string sqlDeleteTTestId = "DELETE FROM teste WHERE codigo=$1";
    vector<string> campos;

    vector<string> comandosSQL = {
        this->sqlInsertTTeste,
        this->sqlSelectTTestAll,
        this->sqlSelectTTestId,
        this->sqltSelectTTestNome,
        this->sqltSelectTTestIdade,
        this->sqlDeleteTTestId
    };
    vector<string> declaracaoPrepare = {
        "insertTeste",
        "selectAll",
        "selectId",
        "selectNome",
        "selectIdade",
        "deleteId"
    };
}TabelaTeste;

typedef struct TabelaCliente{
    const string tabela = "cliente";
    const string colunas = "nome_cliente, cpf_cliente, "
    "data_nasc, endereco_rua, endereco_numero, endereco_bairro, "
    "enderco_cep, endereco_estado, endereco_cidade";
    string nome_cliente;
    string cpf_cliente;
    string data_nasc;
    string endereco_rua;
    string endereco_numero;
    string endereco_bairro;
    string endereco_cep;
    string endereco_estado;
    string endereco_cidade;
}TabelaCliente;

class ArquivoBD{

public:
    const string arquivoLogin = ".loginBDPostgreSQL";

    void gravarArquivo(const string &arquivo, string dados){
        try {
            ofstream arquivoSaida;
            arquivoSaida.open(arquivo, ios::out);
            if(!arquivoSaida){
                throw (string("O Arquivo " + arquivo + " não pode ser aberto."));
            }
            arquivoSaida << dados;
            arquivoSaida.close();
        }
        catch (const exception &e) {
            throw;
        }
    }

    string lerArquivo(const string &arquivo){
        try {
            string dados, linha;
            ifstream arquivoEntrada;
            arquivoEntrada.open(arquivo, ios::in);
            if(!arquivoEntrada){
                throw std::invalid_argument("Arquivo não encontrado\nConfigurar banco de dados.");
            }
            while (getline(arquivoEntrada, linha)) {
                dados += linha;
            }
            arquivoEntrada.close();
            return dados;
        }
        catch (const exception &e) {
            throw;
        }
    }

    string montarEntradaBD(string usuario, string host, string senha, string porta){
        try {
            string entradaBD = "dbname=projeto_cmaismais user=" + usuario +
                        " password=" + senha + " hostaddr=" + host + " port=" + porta;
            return entradaBD;
        }
        catch (const exception &e) {
            throw;
        }
    }
};

class ConectBD {
private:
    TabelaTeste tbTeste;
    ArquivoBD a;

public:
    ConectBD(){}

    void executarTabelaTeste(ConectBD *c, TabelaTeste *tb_t,
        const string &declaracao, vector<string> v);

    void inciarSQLTabelaTeste(pqxx::connection *c, TabelaTeste tbTeste){
        /*Carrega as sentenças que serão usadas no pqxx:prepare*/
        try {
            for(int i = 0; i < tbTeste.comandosSQL.size(); i++){
                this->preparaDados(c, tbTeste.declaracaoPrepare[i], tbTeste.comandosSQL[i]);
            }
        }
        catch (const exception &e) {
            throw;
        }
    }

    void preparaDados(pqxx::connection *con, const string &nomePrepara, const string &sql){
        /*Vincula uma sentença a sua query SQL*/
        try{
            con->prepare(nomePrepara, sql);
        }
        catch(const exception& e){
            throw ;
        }
    }

    void executarPrepara(const string &nomeprepara, const vector<string> &args){
        /*Executa uma query qualquer a partir da sua senteça e valores que serão gravados*/
        try{
            string login = this->a.lerArquivo(a.arquivoLogin);
            pqxx::connection con(login);
            pqxx::work w(con);
            pqxx::params argumentos;
            for (const auto &arg : args){
                argumentos.append(arg);
            }
            this->inciarSQLTabelaTeste(&con, tbTeste);
            w.exec_prepared(nomeprepara, argumentos);
            w.commit();
        }
        catch(const exception& e){
            throw ;
        }
    }

    vector<vector<string>> imprimirPreparaResult(const string &nomeprepara, const vector<string> &args){
        try{
            vector<vector<string>> itens;
            vector<string> colunas;
            string login = this->a.lerArquivo(a.arquivoLogin);
            pqxx::connection con(login);
            this->inciarSQLTabelaTeste(&con, tbTeste);
            pqxx::result r;
            pqxx::work w(con);
            if (args.size() == 0) {
                r = w.exec_prepared(nomeprepara);
            }
            else{
                pqxx::params argumentos;
                for (const auto &arg : args){
                    argumentos.append(arg);
                }
                r = w.exec_prepared(nomeprepara, argumentos);
                w.commit();
            }
            for (const auto &row: r){
                for (const auto &field: row){
                    colunas.push_back(field.c_str());
                }
                itens.push_back(colunas);
                colunas.clear();
            }
            return itens;
        }
        catch(const exception& e){
            throw ;
        }
    }
};

#endif
