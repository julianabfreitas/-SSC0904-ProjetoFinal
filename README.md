# Projeto Final
Projeto Final referente a disciplina SSC0904 - Sistemas Computacionais Distribuídos.
<br>
<br>
<b>Integrantes do grupo:</b>

<ul>
<li>Beatriz Helena Dias Rocha, NUSP: 11300051</li>
<li>Juliana Bernardes Freitas, NUSP: 11317928</li>
<li>Juliana Martins Leôncio Eusébio, NUSP: 10693340</li>
<li>Rafael Kuhn Takano, NUSP: 11200459</li>
</ul>
 
## A aplicação
Essa aplicação trata-se da conexão de um servidor com vários clientes, em que os clientes mandam arquivos para o servidor que os devolve encriptografados (método cifra de césar). A inspiração veio do trabalho 1 da disciplina em que o grupo fez um seminário com o tema "O que é e como reduzir incidentes de ransoware", dessa forma essa criptografia seria uma alusão a isso. 
 
## Como compilar o código

### Servidor
Para o servidor, utilize o seguinte comando no terminal linux de sua preferência.
  
`gcc server.c funcoes.c -pthread -o server`
  
E em seguida, rode o executável.
  
`./server`
  
### Cliente
  
Para o cliente, utilize o seguinte comando no terminal linux de sua preferência.

`gcc client.c -o client`
  
 E em seguida, rode o executável.
  
`./client (número de rotação) (arquivo_inicial.txt) (arquivo_criptografado.txt)`
