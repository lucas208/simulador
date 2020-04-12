#ifndef BIBLIOTECA_HPP
#define BIBLIOTECA_HPP

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip> 

using namespace std;

//classe do programa, formato endereço
class Endereco{
  
  public:
  //construtores
  Endereco() = default;
  Endereco(string num, char tipo,int64_t usado) : num_{num}, tipo_{tipo},usado_{usado}{};
  //métodos
  void setNum(string);
  void setTipo(char);
  string getNum();
  char getTipo();
  int64_t getUsado();
  void setUsado(int64_t);
  //atributos
  private:
    string num_;
    char tipo_;
    int64_t usado_;

};

//demais funções
int64_t horaAtual();
bool temPagina(vector<Endereco> &pgmemoria, int n);
bool escreve(vector<Endereco> &pgmemoria, Endereco end, int n);
bool busca(vector<Endereco> &pgmemoria, Endereco end);
void aleatoria(vector<Endereco> &pgmemoria, Endereco end);
void fifo(vector<Endereco> &pgmemoria, Endereco end);
void lru(vector<Endereco> &pgmemoria, Endereco end);
Endereco formataEnd(string endereco);

#endif 