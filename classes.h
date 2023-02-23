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

//class ArquivoBD();

class ArquivoBD{

public:
    const string arquivoUsuario = ".usuarioBD";
    const string arquivoSenha = ".senhaBD";
    const string arquivoHost = ".hostBD";
    const string arquivoPorta = ".portaBD";
//    ArquivoBD() {}

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
            string dados;
            ifstream arquivoEntrada;
            arquivoEntrada.open(arquivo, ios::in);
            if(!arquivoEntrada){
                throw (string("O arquivo" + arquivo + " não pode ser abreto."));
            }
            arquivoEntrada >> dados;
            arquivoEntrada.close();
            return dados;
        }
        catch (const exception &e) {
            throw;
        }
    }

    string montarEntradaBD(){
        try {
            string usuario, host, senha, porta, entradaBD;
            usuario = this->lerArquivo(this->arquivoUsuario);
            host = this->lerArquivo(this->arquivoHost);
            senha = this->lerArquivo(this->arquivoSenha);
            porta = this->lerArquivo(this->arquivoPorta);
            entradaBD = "dbname=projeto_cmaismais user= " + usuario +
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
    string BD;
    pqxx::connection con;
    TabelaTeste tbTeste;


public:

    ConectBD(string banco) : con(banco){}

    void executarTabelaTeste(ConectBD *c, TabelaTeste *tb_t,
        const string &declaracao, vector<string> v);

    void inciarSQLTabelaTeste(TabelaTeste tbTeste){
        try {

            for(int i = 0; i < tbTeste.comandosSQL.size(); i++){
                this->preparaDados(tbTeste.declaracaoPrepare[i], tbTeste.comandosSQL[i]);
            }
        }
        catch (const exception &e) {
            throw;
        }
    }

    void preparaDados(const string &nomePrepara, const string &sql){
        try{
            this->con.prepare(nomePrepara, sql);
        }
        catch(const exception& e){
            throw ;
        }
    }

    void executarPrepara(const string &nomeprepara, const vector<string> &args){
        try{
            pqxx::work w(this->con);
            pqxx::params argumentos;
            for (const auto &arg : args){
                argumentos.append(arg);
            }
            w.exec_prepared(nomeprepara, argumentos);
            w.commit();
        }
        catch(const exception& e){
            throw ;
        }
    }

    void imprimirPreparaResult(const string &nomeprepara, const vector<string> &args){
        try{
            pqxx::result r;
            pqxx::work w(this->con);
            if (args.size() == 0) {
                r = w.exec_prepared(nomeprepara);
            }
            else{
                pqxx::params argumentos;
                for (const auto &arg : args){
                    argumentos.append(arg);
                }
                r = w.exec_prepared(nomeprepara, argumentos);
            }
            w.commit();
        }
        catch(const exception& e){
            throw ;
        }
    }
};

#endif
