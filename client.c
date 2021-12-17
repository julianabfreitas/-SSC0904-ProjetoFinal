#include <stdio.h>	
#include <stdlib.h>
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

// Argumentos da chamada do client: ./client (rotação) (nome do arq inicial) (nome do arq final)

int main(int argc, char *argv[]){
    //declaração das variáveis
    int socket_cliente, rotacao = atoi(argv[0]);
    struct sockaddr_in servidor;
    char msg_cliente[1800], msg_servidor[1800];
    FILE* arq_i, arq_f;

    arq_i = fopen(argv[1],'r');
    arq_f = fopen(argv[2],'w');

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

    //o cliente envia para o servidor, o valor da rotação
    if(send(socket_cliente, &rotacao, sizeof(int), 0) < 0){
        puts("Falha no envio da rotação! :(");
        return 1;
    }

    //while que fica lendo as mensagens que o cliente deseja criptografar
    while(1){

        if(fread(msg_cliente,sizeof(char),1800,arq_i) == 0){
            break;
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

        fwrite(msg_servidor,sizeof(char),1800,arq_f);
		
        //printf("(Servidor) A mensagem criptografada é: %s\n", msg_servidor);

		//limpeza das variáveis
        memset(msg_cliente,0,1800);
        memset(msg_servidor,0,1800);
	}

    fclose(arq_i);
    fclose(arq_f);
    return 0;
}