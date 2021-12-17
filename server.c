#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>	
#include <unistd.h>	
#include <pthread.h> 
#include "funcoes.h"

int main(){
    //declaração das variáveis
    int socket_servidor, socket_cliente, tamanho, tam_msg, *novo_socket; 
    struct sockaddr_in servidor, cliente;
    char *messagem, mensagem_cliente[1800];
    
    //criação do socket
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
        //usou-se sockstream porque desejou-se criar uma consexão TCP

    //verifica se o socket foi criado
    if(socket_servidor == -1){
        printf("Socket não pode ser criado! :(");
    }

    //mensagem de confirmação de criação do socket
    puts("Socket Criado! :)");

    //colocando os valores para o socket do servidor
    servidor.sin_family = AF_INET; //a mais usada
    servidor.sin_port = htons(8282); //porta aleatória
    servidor.sin_addr.s_addr = INADDR_ANY; //porque é um servidor
    
    //operação de bind que associa o servidor ao socket, IP e porta 
    if(bind(socket_servidor,(struct sockaddr *)&servidor,sizeof(struct sockaddr)) < 0){
        perror("Bind deu erro! :(");
        return 1;
    }

    //função de deixa o socket em modo escuta esperando por conexões
	listen(socket_servidor, 3);
    puts("Esperando conexão...");

    //caso ocorra algum problema com o cliente
    if(socket_cliente < 0){
		perror("Falha na conexão com o cliente :(");
		return 1;
	}

    tamanho = sizeof(struct sockaddr_in);

	//loop que fica aceitando conexões cos sockets clientes
    while((socket_cliente = accept(socket_servidor, (struct sockaddr *)&cliente, (socklen_t*)&tamanho))){

        //mensagem de sucesso
        puts("Conexão aceita! :)");
		
        //declaração da thread (uma para cada novo cliente)
        pthread_t nossa_thread;

        //armazenando espaço pro socket do cliente
	    novo_socket = malloc(1);
	    *novo_socket = socket_cliente;

        //criação da thread que chama a função 'gerencia_conexão' 
		if((pthread_create(&nossa_thread, NULL,  gerencia_conexao, (void*)novo_socket)) < 0){
			perror("Não foi possível criar a thread! :(");
			return 1;
		}
		
        //mensagem de sucesso
		puts("Nova thread criada! :)");
	}
    
    //caso ocorra algum problema com a conexão do socket cliente
    if (socket_cliente < 0){
		perror("Falha no estabelecimento de conexão! :(");
		return 1;
	}

    return 0;
}
