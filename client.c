#include <stdio.h>	
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

int main(){
    //declaração das variáveis
    int socket_cliente, rotacao;
    struct sockaddr_in servidor;
    char msg_cliente[1800], msg_servidor[1800];

    //criação do socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
        //usou-se sockstream porque desejou-se criar uma consexão TCP

    //verifica se o socket foi criado
    if(socket_cliente == -1){
        printf("Socket não pode ser criado! :(");
    }

    //mensagem de sucesso
    puts("Socket Criado! :)");

    //colocando os valores para o socket do servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8282);
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    //função que estabelece conexão entre o cliente e o servidor
    if(connect(socket_cliente, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){
		perror("Falha na conexão! :(");
		return 1;
	}

    //mensagem de sucesso
    puts("Conexão estabelecida com sucesso!");

    //o cliente tem a opção de escolher que o tamanho da rotação que ele deseja para a sua criptografia
    printf("Escolha a rotação desejada para a criptografia: ");
	scanf("%d", &rotacao);
    getchar();

    //o cliente envia para o servidor, o valor da rotação
    if(send(socket_cliente, &rotacao, sizeof(int), 0) < 0){
        puts("Falha no envio da mensagem! :(");
        return 1;
    }

    //while que fica lendo as mensagens que o cliente deseja criptografar
    while(1){
        //cliente insere mensagem
        printf("Insira a mensagem a ser criptografada: ");
		scanf("%[^\n]", msg_cliente);
        getchar();

        //caso o cliente digite '!', o processo acaba
        if(msg_cliente[0] == '!'){
            puts("O cliente desconectou");
		    fflush(stdout);
            return 0;
        }

		//Envio da mensagem do cliente para o servidor
		if(send(socket_cliente, msg_cliente, strlen(msg_cliente), 0) < 0){
			puts("Falha no envio da mensagem! :(");
			return 1;
		}

		//Recebimento da mensagem já criptografada do servidor
		if(recv(socket_cliente, msg_servidor, 1800, 0) < 0){
			puts("Falha no recv");
			break;
		}

		printf("(Servidor) A mensagem criptografada é: %s\n", msg_servidor);

		//limpeza das variáveis
        memset(msg_cliente,0,1800);
        memset(msg_servidor,0,1800);
	}

    return 0;
}