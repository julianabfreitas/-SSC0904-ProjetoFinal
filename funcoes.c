#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <pthread.h> 
#include "funcoes.h"

//arquivo com funções utilizadas no 'server.c'

//função para encriptar a mensagem
char *encriptar(char *mensagem, int rotacao){
    int i = 0; //auxiliar
    char aux; //auxiliar - guarda os caracteres após a transformação

    for(i=0; i < strlen(mensagem); i++){
        //verifica se é um espaço
        if(mensagem[i] == ' '){
            aux = ' '; //se for salva isso no auxiliar
        }
        //verifica se é um valor entre A e Z-rotação (ou seja, se não vai dar a volta na alfabeto)
        else if((mensagem[i] >= 'A' && mensagem[i] <= ('Z' - rotacao)) || (mensagem[i] >= 'a' && mensagem[i] <= ('z' - rotacao))){
                aux = mensagem[i] + rotacao; //se for, faz a criptografia normal
        }
        //caso seja um valor que dê a volta no alfabeto
        else if(mensagem[i] > ('Z' - rotacao) || mensagem[i] > ('z' - rotacao)){ 
            if(mensagem[i] > 97){ 
                aux = 'a' + rotacao - ('z' - mensagem[i]) - 1; //devemos considerar as voltas nas contas
            }else{
                aux = 'A' + rotacao - ('Z' - mensagem[i]) - 1;
            } 
        }
        mensagem[i] = aux; //salva o valor transformado na mensagem 
    }
    
    return mensagem; //retorna a mensagem criptografada
}

//função para as threads
void *gerencia_conexao(void *socket_servidor){
    
    //declaração de variáveis
	int sock = *(int*)socket_servidor;
	int tam_msg, rotacao;
	char mensagem_cliente[1800];
    char mensagem_criptografada[1800];

    //recebe a mensagem do cliente
        //nesse caso, recebe a rotação, variável necessária para a criptografia
    if(tam_msg = recv(sock, &rotacao, sizeof(int), 0) < 0){
		puts("Falha no recv");
		return 0;
	}

	//while que fica recebendo mensagem do cliente
	while((tam_msg = recv(sock, mensagem_cliente, 1800 , 0)) > 0){	
        
        //caso o cliente deseje sair é só ele digitar '!'
        if(mensagem_cliente[0] == '!'){
            puts("O cliente desconectou :/");
            return 0;
        }

        //cópia da mensagem criptografada para variável 'mensagem_criptografa'
            //a criptografia foi feita pela função 'encriptar'
        strcpy(mensagem_criptografada, encriptar(mensagem_cliente, rotacao));

        //servidor manda mensagem para o cliente
		write(sock, mensagem_criptografada, strlen(mensagem_criptografada));
        
        //limpeza das variáveis
        memset(mensagem_cliente,0,1800);
        memset(mensagem_criptografada,0,1800);
	}

    
	if(tam_msg == 0){
		puts("O cliente desconectou :/");
		fflush(stdout);
	}

    //erro na mensagem
	if(tam_msg == -1){
		perror("Erro no recebimento da mensagem :(\n");
	}

    //fecha o socket
    close(sock);

	//Free no ponteiro do socket
	free(socket_servidor);
	
	return 0;
}