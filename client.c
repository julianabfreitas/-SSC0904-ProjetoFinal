#include <stdio.h>	
#include <stdlib.h>
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

// Argumentos da chamada do client: ./client (rotação) (nome do arq inicial) (nome do arq final)

int main(int argc, char *argv[]){
    //declaração das variáveis
    int socket_cliente, rotacao = atoi(argv[1]);
    long tamanho;
    struct sockaddr_in servidor;
    FILE *arq_i, *arq_f;

    if(argc != 4){
        (argc < 4) ? printf("Número de parâmetros insuficientes\n") : printf("Número de parâmetros além do necessário\n");
        return 0;
    }

    arq_i = fopen(argv[2],"r");
    arq_f = fopen(argv[3],"w");

    fseek(arq_i,0,SEEK_END);
    tamanho = ftell(arq_i);
    fseek(arq_i,0,SEEK_SET);
    printf("%d",tamanho);

    char msg_cliente[tamanho], msg_servidor[tamanho];

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

    if(fread(msg_cliente,sizeof(char),tamanho,arq_i) == 0){
        printf("Erro ao ler o arquivo");
        return 0;
    }    

    //Envio da mensagem do cliente para o servidor
    if(send(socket_cliente, msg_cliente, strlen(msg_cliente), 0) < 0){
        puts("Falha no envio da mensagem! :(");
        return 1;
    }

    //Recebimento da mensagem já criptografada do servidor
    if(recv(socket_cliente, msg_servidor, tamanho, 0) < 0){
        puts("Falha no recv");
        return 0;
    }

    fwrite(msg_servidor,sizeof(char),tamanho,arq_f);

    //limpeza das variáveis
    memset(msg_cliente,0,tamanho);
    memset(msg_servidor,0,tamanho);

    fseek(arq_i,-1,SEEK_END);
    fputc('\0',arq_i);

    fclose(arq_i);
    fclose(arq_f);
    return 0;
}