/*
    Atos Brito Omena - 22154128
    Daniel Cardozo Santos - 22152299
    Joao Victor Nogueira de Souza - 22153464
    Grecia Cristina Garcia Rivera - 22153222
    */
   #include<bits/stdc++.h>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream> 
#include <fcntl.h>
#include <sys/stat.h>


using namespace std;

void executa(string comando);

string stringAlt(int f, int i, string comando);

vector<string> separaComandos (string comandoBruto);

void executaSimples (string comando);

int main(){

    string blocoComandos;

    while (true){
        // getline() resume a funcao getComando
        getline(cin, blocoComandos);
        
        // linha de comando vazia
        if (blocoComandos.empty()){
            continue;
        }
        else if (blocoComandos == "exit" || blocoComandos == "quit"){
            exit(0);
        }
        else{
            executa(blocoComandos);
        }
       
       

    }

    return 0;
}

void executa(string comando){

    int i = 0, f = 0;
    vector<string> blocoComandos;
    int operacao = 0;


    while(comando[i] != 0){

        switch(comando[i]){
            case '<' :
                    comando[i] = ' ';
                    
                    blocoComandos.push_back(stringAlt(f, i, comando));
                    f = i+1;

                    operacao = 1;
                break;
            case '>' :
                    comando[i] = ' ';
                    
                    blocoComandos.push_back(stringAlt(f, i, comando));
                    f = i+1;

                    operacao = 2;
                break;
            case '|' :
                    comando[i] = ' ';
                    
                    blocoComandos.push_back(stringAlt(f, i, comando));
                    f = i+1;

                    operacao = 3;
                break;
            case '&' :
                    comando[i] = ' ';
                    
                    blocoComandos.push_back(stringAlt(f, i, comando));
                    f = i+1;

                    operacao = 4;
                break;
        }
        i++;
    }

    blocoComandos.push_back(stringAlt(f,i,comando));

    // caso a linha de comando não contenha operador
    if(operacao == 0){
        vector<string> comandoSegmentado = separaComandos(blocoComandos[0]);
        if (comandoSegmentado[0] == "cd"){
            chdir(comandoSegmentado[1].c_str());
        }
        else{
            executaSimples(blocoComandos[0]);
        }
    }else{
        //retira espacos em branco no inicio e no final do segundo comando
        blocoComandos[1].erase(0, blocoComandos[1].find_first_not_of(" \t\n\r\f\v"));
        blocoComandos[1].erase(blocoComandos[1].find_last_not_of(" \t\n\r\f\v") + 1);

            
            if (operacao == 1){//operador <
                pid_t pid = fork();
                if (pid == 0){
                    FILE *file = freopen(blocoComandos[1].c_str(), "r", stdin);
                    system(blocoComandos[0].c_str());
                    fclose(file);
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }
                
            }

            if (operacao == 2){//operador >
                pid_t pid = fork();
                if (pid == 0){
                    FILE *file = freopen(blocoComandos[1].c_str(), "w", stdout);
                    system(blocoComandos[0].c_str());
                    fclose(file);
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }
            }
            
            if (operacao == 3){//operador |
                pid_t pid = fork();
                if (pid == 0){
                    FILE *file = freopen(".buffer", "w", stdout);
                    system(blocoComandos[0].c_str());

                    fclose(file);
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }
                sleep(1);

                pid = fork();
                if (pid == 0){
                    FILE *file = freopen(".buffer", "r", stdin);

                    system(blocoComandos[1].c_str());

                    fclose(file);
                    
                    system("rm .buffer");
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }
                
            }
            
            if (operacao == 4){//operador &
                pid_t pid = fork();
                if (pid == 0){
                    executaSimples(blocoComandos[0]);
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }

                pid = fork();
                if (pid == 0){
                    executaSimples(blocoComandos[1]);
                }
                else if (pid < 0){
                    perror("fork");
                    exit(1);
                }
                if (pid == 0){
                    exit(1);
                }
            }
        
    }
}

string stringAlt(int f, int i, string comando){
    string bloco = "";
    for(f ; f <= i ; f++){
        bloco.push_back(comando[f]);
    }
    bloco.push_back(0);

    return bloco;
}

vector<string> separaComandos (string comandoBruto){
    // uma lista com as palavras do comando bruto separadas
    stringstream s(comandoBruto);
    // vector que vai receber 1 palavra em cada um dos seus indices
    vector<string> comandosSeparados;
    // string que vai servir como "leitor" para a lista e passar para o vector
    string comando;
    // realizando a passagem da lista para o vector
    while(s >> comando){
        comandosSeparados.push_back(comando);
    }

    return comandosSeparados;
}

void executaSimples (string comando){
        vector<char*> comandoConvertido;

        
        for (const auto &i: separaComandos(comando)){
            comandoConvertido.push_back( const_cast<char*>(i.c_str()) );
        }

        comandoConvertido.push_back( nullptr );

        pid_t pid = fork();
        if (pid == 0){
            execvp( comandoConvertido[0], comandoConvertido.data());
            perror("execvp");
            
        }
        else if (pid < 0){
            perror("fork");
        }
}
