#include "biblioteca.hpp"


void Endereco::setNum(string num){
  num_ = num;
}

void Endereco::setTipo(char tipo){
  tipo_ = tipo;
}

string Endereco::getNum(){
  return num_;
}

char Endereco::getTipo(){
  return tipo_;
}

int64_t Endereco::getUsado(){
    return usado_;
}
void Endereco::setUsado(int64_t usado){
    usado_ = usado;
}

//referencia:
//https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
//função adaptada, retorna o tempo atual em milisegundos

int64_t horaAtual(){
  using namespace std::chrono;

  int64_t agorams = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  
  return agorams;
}

bool temPagina(vector<Endereco> &pgmemoria, int n){
  if(pgmemoria.empty() or pgmemoria.size() < n){
    return true;
  }
  return false;
}

bool escreve(vector<Endereco> &pgmemoria, Endereco end, int n){
  if(temPagina(pgmemoria,n)){
    pgmemoria.push_back(end);
    return true;
  }
  return false;
}

bool busca(vector<Endereco> &pgmemoria, Endereco end){
  //verifica se o vector está vazio
  if(pgmemoria.empty()){
    return false;
  }
  //compara o código do endereço passado com o código de todos
  //os endereços que estão nas páginas da memória
  for(int i = 0;i<pgmemoria.size();i++){
    if(pgmemoria[i].getNum() == end.getNum()){
      return true;
    }
  }
  return false;
}

void aleatoria(vector<Endereco> &pgmemoria, Endereco end){
  //gera indice randomico dentro do range do vetor
  int random = rand()%pgmemoria.size();
  //sobrescreve o elemento na posição randomica
  //com o elemento passado como argumento
  pgmemoria[random] = end;
}

void fifo(vector<Endereco> &pgmemoria, Endereco end){
  //apaga o primeiro elemento do vetor
  pgmemoria.erase(pgmemoria.begin());
  //insere o endereço passado como argumento no final do vetor
  pgmemoria.push_back(end);
}

void lru(vector<Endereco> &pgmemoria, Endereco end){
  //pega o tempo da posição do primeiro endereço, em milissegundos
  int64_t maisAntigo = pgmemoria[0].getUsado();
  //variavel utilizada para armazenar o indice da posicao do mais antigo
  //começa com zero porque é o índice do primeiro
  int indice = 0;
  //verifica todo resto para ver se tem alguma posição referenciada a mais tempo
  //por isso começa pelo índice 1
  for(int i = 1;i<pgmemoria.size();i++){
    if(pgmemoria[i].getUsado()<maisAntigo){
      maisAntigo = pgmemoria[i].getUsado();
      indice = i;
    }
  }
  //escrevendo o endereço passado como argumento na posição mais antiga (menos recentemente utilizada)
  pgmemoria[indice] = end;
  //atualiza tempo daquele endereço naquela posição
  end.setUsado(horaAtual());
}

Endereco formataEnd(string endereco){
  Endereco formatado;
  formatado.setNum(endereco.substr(0,8));
  formatado.setTipo(endereco.back());
  formatado.setUsado(horaAtual());
  return formatado;
}