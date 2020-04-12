#include "biblioteca.hpp"

int main(int argc, char **argv){
  //altera a semente do random
  srand(horaAtual());
  //captura o tempo em que a funçao começa sua execução
  int64_t inicio = horaAtual();
  //variáveis de contagem
  int acessos = 0;
  int pagefaults = 0;
  int pglidas = 0;
  int pgescritas = 0;
  int hits = 0;
  
  //resgatando os dados fornecidos pelo usuário
  string tecnica = argv[1];
  ifstream entrada(argv[2]);
  int tam_pagina = atoi(argv[3]);
  int tam_memoria = atoi(argv[4]);
  int npaginas = tam_memoria/tam_pagina;
 
  //verificando a tecnica escolhida
  if(tecnica != "lru" and tecnica != "fifo" and tecnica != "random"){
    cout<<"Tecnica não cadastrada, tente novamente com lru,fifo ou random"<<endl;
    return 1;
  }
  //verificando se o tamanho das páginas e da memória correspondem ao intervalo sugerido
  if(tam_pagina<2 or tam_pagina>64){
    cout<<"O valor do tamanho da página está fora do intervalo sugerido (2 a 64)KB"<<endl;
    return 1;
  }
  if(tam_memoria<128 or tam_memoria>16384){
    cout<<"O valor do tamanho da memória está fora do intervalo sugerido (128 a 16384)KB"<<endl;
    return 1;
  }
  cout<<endl;
  cout<<"Executando o simulador..."<<endl;
  //declarando nosso vetor de páginas para representar a memória
  vector<Endereco> pgmemoria;

  //verificando a leitura do arquivo e lendo-o linha por linha
  if(not entrada){
    perror(argv[2]);
  }else{
      string endereco;
      while(not entrada.eof()){
        //lê o endereço no arquivo e contabiliza o acesso
        getline(entrada, endereco);
        //cout<<endereco<<endl;
        acessos++;
        //formata o endereço para o formato da minha classe
        Endereco endFormatado = formataEnd(endereco);
        //verifica se é de escrita
        if(endFormatado.getTipo() == 'W' or endFormatado.getTipo() == 'w'){
          //contabiliza páginas escritas
          ++pgescritas;
          //realiza busca pra ver se o endereço já consta na memória
          if(busca(pgmemoria,endFormatado)){
            //se encontrar incrementa hits e atualiza tempo se for lru
            hits++;
            if(tecnica == "lru"){
              endFormatado.setUsado(horaAtual());
            }  
          }else{
            //se não achar incrementa pagefaults
            pagefaults++;
            //tenta escrever o endereço, se consegue atualiza tempo(lru)
            if(escreve(pgmemoria,endFormatado,npaginas)){
              if(tecnica == "lru"){
                endFormatado.setUsado(horaAtual());
              }
            }else{
              //se nao conseguiu escrever incrementa pagefaults
              pagefaults++;
              //substitui a pagina colocando na posiçao
              //de acordo com a tecnica
              if(tecnica == "lru"){
                lru(pgmemoria,endFormatado);
              }else if(tecnica == "fifo"){
                fifo(pgmemoria,endFormatado);
              }else{
                aleatoria(pgmemoria,endFormatado);
              }
            }
          }
        //se nao for de escrita verifica se é um acesso de leitura
        }else if(endFormatado.getTipo() == 'R' or endFormatado.getTipo() == 'r'){
          //contabiliza pglidas
          ++pglidas;
          //se encontra o endereço na memoria contabiliza hits,
          //atualiza tempo se a tecnica for lru
          if(busca(pgmemoria,endFormatado)){
            hits++;
            if(tecnica == "lru"){
              endFormatado.setUsado(horaAtual());
            }
          }else{
            //se não encontra atualiza pagefaults
            pagefaults++;
            //tenta escrever o endereço se ainda tem espaço e atualiza tempo (lru)
            if(escreve(pgmemoria,endFormatado,npaginas)){
              if(tecnica == "lru"){
                endFormatado.setUsado(horaAtual());
              }
            }else{
             //se não conseguir substitui e incrementa pagefaults
              pagefaults++;
              if(tecnica == "lru"){
                lru(pgmemoria,endFormatado);
              }else if(tecnica == "fifo"){
                fifo(pgmemoria,endFormatado);
              }else{
                aleatoria(pgmemoria,endFormatado);
              }
            }
          }
        }
      }
    }  
    //após a leitura do arquivo ele é fechado
    //o tempo do final do processamento é capturado
    entrada.close();
    //decrementando acessos em uma unidade para exibir o número de linhas correto
    acessos--;

    int64_t fim = horaAtual();
    //exibindo o relatório curto
    cout<<"Arquivo de entrada: "<< argv[2] <<endl;
    cout<<"Tamanho da memória: "<< tam_memoria <<" KB"<<endl;
    cout<<"Tamanho das páginas: "<< tam_pagina <<" KB"<<endl;
    cout<<"Técnica de reposição: "<< tecnica <<endl;
    cout<<"Páginas lidas: "<< pglidas<<setprecision(4) <<" ("<<float(pglidas)/acessos * 100<<"%)"<<endl;
    cout<<"Páginas escritas: "<< pgescritas <<setprecision(4) <<" ("<<float(pgescritas)/acessos * 100<<"%)"<<endl;
  
    //exibição dos dados na memória, para verificação
    cout<<endl;
    cout<<"EXIBIÇÃO DOS DADOS NA MEMÓRIA"<<endl;
    cout<<endl;
    float r = 0;
    float w = 0;
    for(int i = 0;i<pgmemoria.size();i++){
      cout<< pgmemoria[i].getNum() <<" "<<pgmemoria[i].getTipo()<<" " << pgmemoria[i].getUsado() <<endl;
      if(pgmemoria[i].getTipo() == 'W' or pgmemoria[i].getTipo() == 'w'){
        w++;
      }else{
        r++;
      }
    }

    
    cout<<endl;
    cout<<"Número de paginas: " << npaginas <<endl;
    cout<<"Ocupadas: "<<pgmemoria.size()<<endl;
    cout<<"Vazias: "<< npaginas - pgmemoria.size() <<endl;
    cout<<"Uso das páginas da memória: "<< float(pgmemoria.size())/npaginas * 100 <<"%"<<endl;
    cout<<"R: " << r<< " ou "<< r/pgmemoria.size()*100 <<"%" <<endl;
    cout<<"W: " << w<<" ou "<< w/pgmemoria.size()*100  <<"%"<<endl;
    cout<<endl;
    cout<<"Mais dados e estatísticas "<<endl;
    cout<<endl;
    cout<<"Acessos: "<< acessos <<endl;
    cout<<"Hit rate: "<< setprecision(4) << float(hits)/acessos * 100<<"%" <<endl;
    cout<<"Pagefaults: "<<pagefaults<< setprecision(3) << " ("<<float(pagefaults)/acessos * 100<<"%)"<<endl; 
    cout<<"Tempo de processamento: "<< fim - inicio << " ms"<<endl;
    cout<<endl;
    
}



	



