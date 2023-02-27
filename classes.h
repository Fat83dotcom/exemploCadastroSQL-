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
    /*Struct responsavel pelo gerenciamento da tabela testes*/
    const string sqlInsertTTeste = "INSERT INTO teste (nome, idade) VALUES($1, $2)";
    const string sqlSelectTTestAll = "SELECT * FROM teste";
    const string sqlSelectTTestId = "SELECT * FROM teste WHERE codigo=$1";
    const string sqltSelectTTestNome = "SELECT * FROM teste WHERE nome=$1";
    const string sqltSelectTTestIdade = "SELECT * FROM teste WHERE idade=$1";
    const string sqlDeleteTTestId = "DELETE FROM teste WHERE codigo=$1";
    const string sqlSelectTTesteLike = "SELECT * FROM teste WHERE nome LIKE $1";
    vector<string> campos;

    vector<string> comandosSQL = {
        this->sqlInsertTTeste,
        this->sqlSelectTTestAll,
        this->sqlSelectTTestId,
        this->sqltSelectTTestNome,
        this->sqltSelectTTestIdade,
        this->sqlDeleteTTestId,
        this->sqlSelectTTesteLike
    };
    vector<string> declaracaoPrepare = {
        "insertTeste",
        "selectAll",
        "selectId",
        "selectNome",
        "selectIdade",
        "deleteId",
        "selectPChave"
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
        /*Metodo generico para gravação de arquivos*/
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

    string lerArquivo(const string &arquivoPath){
        /*Metodo generico para leitura de arquivos*/
        try {
            string dadosArquivo, palavra;
            ifstream arquivoEntrada;
            arquivoEntrada.open(arquivoPath, ios::in);
            if(!arquivoEntrada){
                throw std::invalid_argument("Arquivo não encontrado\nConfigurar banco de dados.");
            }
            while (getline(arquivoEntrada, palavra)) {
                dadosArquivo += palavra;
            }
            arquivoEntrada.close();
            return dadosArquivo;
        }
        catch (const exception &e) {
            throw;
        }
    }

    string montarEntradaBD(string usuario, string host, string senha, string porta){
        /*Cria a string responsavel pela validação do banco de dados*/
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

    void executarTabelaTeste(ConectBD *c, TabelaTeste *tbT,
                             const string &declaracao, vector<string> v, int indicePreparacao);

    void inciarSQLTabelaTeste(pqxx::connection *c, TabelaTeste tbTeste, int indiceDeclaracao){
        /*Carrega a sentença que será usada no pqxx:prepare da tabela teste*/
        try {
            this->preparaDados(c,
                               tbTeste.declaracaoPrepare[indiceDeclaracao],
                               tbTeste.comandosSQL[indiceDeclaracao]);
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

    void executarPrepara(const string &nomeprepara, const vector<string> &args, int indiceDeclaracao){
        /*Executa uma query qualquer a partir da sua senteça e valores que serão gravados*/
        try{
            string login = this->a.lerArquivo(a.arquivoLogin);
            pqxx::connection con(login);
            pqxx::work w(con);
            pqxx::params argumentos;
            for (const auto &arg : args){
                argumentos.append(arg);
            }
            this->inciarSQLTabelaTeste(&con, tbTeste, indiceDeclaracao);
            w.exec_prepared(nomeprepara, argumentos);
            w.commit();
        }
        catch(const exception& e){
            throw ;
        }
    }

    vector<vector<string>> imprimirPreparaResult(const string &nomeprepara,
                                                 const vector<string> &args,
                                                 int indiceDeclaracao){
        try{
            vector<vector<string>> itens;
            vector<string> colunas;
            string login = this->a.lerArquivo(a.arquivoLogin);
            pqxx::connection con(login);
            pqxx::result r;
            pqxx::work w(con);
            this->inciarSQLTabelaTeste(&con, tbTeste, indiceDeclaracao);
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
