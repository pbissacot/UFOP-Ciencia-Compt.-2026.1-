#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct 
{
    char rnome[51];
    int rnivel;
    int rtentativas;
}Ranking;

typedef struct
{
    char nome[51];
    int nivel;
    int tentativas;
    int tentativasmax;
    int quantcores;
    int *vetorrandom;
    int *vetorjogador;
    int **historico;
}Jogo;

void limpar_buffer();
void criarmenu(char *opcao, Jogo jog, int conferejogo);
void alocavetordificuldade(Jogo *jog, int cores, int tent);
void gerarcores(Jogo *cor);
void coresdisponiveis();
void mostrarodadas(Jogo vegab);
int confereacerto(Jogo confereai);
void jogar(Jogo *jogador, int rodadainicial, int *ganhou);
void carregarank(Ranking *ranking, int *qtd);
int colocarank(Ranking *RANK, int *QTD, char RNOME[51], int RNIVEL, int RTENTATIVAS);
void imprimegabarito(Jogo gabarito);
void vitoriaderrota(Jogo jogdor, int ganhouounao, int terminouounao, int *position, Ranking *rank, int *quantrank);
void pressionatecla(char TECLA);

int main()
{
    int tamanhonome, condvitoria, querojogar, salvofinalizado, posicaor;
    int jogoiniciado = 0; //variavel para saber se o jogador ja foi iniciado ou nao desde que o progama foi rodado (importante para liberar os vetores na saida do programa e quando for rodar um novo save ou um save antigo)
    int jogofinalizado = 0;//variavel para saber se o jogo foi finalizado(ganhou ou perdeu)
    int quantrank = 0;
    int nomevalido;
    char opmenu, salvar;
    char tecla = 0;
    char nomearquivo[55];//variavel importante para colocar o .cor no final do nome e poder salvar o arquivo certinho
    char continuasave[55];//variavel com a mesma função da acima so que serve para quando for continuar um jogo
    char verificacor[5];//variavel importante para descobrir se o jogador digitou .cor no final do nome quando estiver tentando rodar e continuar um save

    Jogo jogador;
    Ranking rank[10];

    carregarank(rank, &quantrank);

    do
    {
        criarmenu(&opmenu, jogador, jogoiniciado);

        if(opmenu == 'A' || opmenu == 'a')
        {
            printf("==============================\n");
            printf("            AJUDA\n");
            printf("==============================\n\n");

            printf("OBJETIVO\n");
            printf("O objetivo do jogo é descobrir a sequência de cores secretadeterminada de\n");
            printf("forma aleatória pelo sistema antes que as suas tentativas acabem\n\n");

            printf("COMO JOGAR\n");
            printf("A cada rodada, digite uma cor por vez (usando o número dela)\n");
            printf("até completar a quantidade de cores da sua dificuldade.\n");
            printf("Depois de cada tentativa, você recebe uma dica:\n\n");
            printf("C = Certo (a cor está certa e na posição certa)\n");
            printf("E = Existe (a cor existe na sequência, mas esta na posição errada)\n");
            printf("- = a cor não existe na sequência secreta\n\n");
            printf("OBS: Como solicitado no PDF com as instruções do professor, o gabarito\n");
            printf("ira mostrar as respostas espelhadas e não na ordem correta!!\n");
            printf("Exemplo:\n");
            printf("Gabarito: 1 2 3 4\n");
            printf("Vetor jogador: 1 4 5 6 (- - E C)\n\n");

            printf("SAINDO NO MEIO DO JOGO\n");
            printf("Se quiser parar no meio de uma partida, digite apenas o numero '0'\n");
            printf("quando o sistema te der a opção de digitar as cores\n\n");

            printf("SALVAR E CONTINUAR\n");
            printf("A opção 'Salvar jogo' guarda o progresso do ultimo save rodado pelo\n");
            printf("no programa (ao rodar um jogo pela primeira vez ele informa qual é o nome\n");
            printf("do ultimo save no menu).\n");
            printf("Caso queira rodar um save antigo que foi salvo porem ainda nao\n");
            printf("concluido, escolha a opção 'Continuar', digite o nome do save sem nem por\n");
            printf("e nem tirar para voltar aonde parou.\n\n");

            printf("RANKING\n");
            printf("Para você entrar no ranking, tem de terminar uma partida por completo. A posição\n");
            printf("é definida pelo numero de tentativas, quanto menor, maior a sua posição no rank!!\n");
            printf("Em caso de empate na quantidade de tentativas entre dois saves, ganha aquele que\n");
            printf("tiver o nivel de dificuldade mais alto\n");

            pressionatecla(tecla);
        }
        else if(opmenu == 'N' || opmenu == 'n')
        {
            if(jogoiniciado == 1)//se o usuario ja ter começado um novo save e decidir sair e começar outro, precisa liberar o vetor do primeiro save pra depois alocar o do segundo
            {
                free(jogador.vetorrandom);
                free(jogador.vetorjogador);
                for(int i = 0; i < jogador.tentativasmax; i++)
                    free(jogador.historico[i]);
                free(jogador.historico);
            }

            jogoiniciado = 1;
            jogador.tentativas = 0;//Caso ele tenha jogado um save antes, isso aqui vai resetar as tentativas dele
            condvitoria = 0;//Caso ele tenha jogado um save antes que tenha ganho, isso vai impedir que ele pule todo jogo

            printf("Digite seu nome: ");
            fgets(jogador.nome, 51, stdin);
            
            tamanhonome = strlen(jogador.nome);
            jogador.nome[tamanhonome-1] = '\0'; //aqui ele pega o nome do jogador e tira o \n no final e substitui com um \0 para não ter complicações depooiisss na hora de imprimir o nome quando a pessoa ganahr

            printf("\nEscolha o nível:\n");
            printf("1 - Fácil (4 cores, 10 tentativas)\n");
            printf("2 - Médio (5 cores, 12 tentativas)\n");
            printf("3 - Difícil (6 cores, 15 tentativas)\n\n");

            do //usei um do while aqui pra ter certeza que o usuario vai escolher uma das dificuldades
            {
                printf("Opção: ");
                scanf("%d", &jogador.nivel);

                //alocar o vetor baseado na escolha da dificuldade
                switch(jogador.nivel)
                {
                    case 1:
                        alocavetordificuldade(&jogador, 4, 10); //passa a quantidade de cores e tentativas
                        break;
                    case 2:
                        alocavetordificuldade(&jogador, 5, 12);
                        break;
                    case 3:
                        alocavetordificuldade(&jogador, 6, 15); 
                        break;
                    default:
                        printf("\nOpção invalida! Digite novamente...\n");
                }
            }while((jogador.nivel != 1) && (jogador.nivel != 2) && (jogador.nivel != 3));

            printf("\n");
            coresdisponiveis();//escolhi usar uma função aqui prq eu quero usar de novo na opção de continuar jogo
            
            gerarcores(&jogador); //aqui gera as cores do save

            do
            {
                jogar(&jogador, 0, &condvitoria);
            }while((jogador.vetorjogador[0] != 0) && (jogador.tentativas < jogador.tentativasmax) && (condvitoria == 0)); //vai continuar rodando enquanto o jogador ainda nao digitar 0 e o limite de jogadas ainda nao ter atingido o maximo OU ate ele acertar eba
            //jogador.vetorjogador[0] != 0: para impedir que o usuario jogue depois que ele digite 0 e escolha 
            //jogador.tentativas < jogador.tentativasmax: para impedir que o usuario jogue (jogador.tentativasmax + 1) rodadas
            //condvitoria == 0: para impedir que o usuario continue jogando mesmo depois de ele ter acertado

            vitoriaderrota(jogador, condvitoria, jogofinalizado, &posicaor, rank, &quantrank);

            //Mensagem para mostrar no final do jogo, independentemente se o jogador venceu ou não
            if((condvitoria == 0) || (condvitoria == 1))
            {
                pressionatecla(tecla);
            }
        }
        else if(opmenu == 'C' || opmenu == 'c')
        {
            querojogar = 1;//pra saber se o usuario vai entrar ou não no save, se ele nao entrar, passa para valor 0 e nao entra no do while de novo
            FILE *continua;

            do
            {
                nomevalido = 1;

                printf("Digite o nome do 'save' que deseja continuar (0 para sair): ");
                fgets(continuasave, 51, stdin);

                if(continuasave[0] == '0')// se o usuario digitar 0 ele sai
                {
                    limpar_buffer();
                    querojogar = 0;//variavel que eu to usando pra nao repetir e do while

                    printf("\nSaindo...\n");

                    pressionatecla(tecla);
                }
                else
                {   
                    tamanhonome = strlen(continuasave);
                    continuasave[tamanhonome - 1] = '\0';//tirar o "\n" do final do nome

                    int k = 0;
                    for(int i = (tamanhonome - 5); i < (tamanhonome - 1); i++)
                    {
                        verificacor[k] = continuasave[i];//aqui a gente so vai verificar se tem .cor ou nao no final do nome
                        k++;
                    }
                    verificacor[4] = '\0';

                    nomevalido = strcmp(verificacor, ".cor");

                    if(nomevalido == 0)
                        printf("\nDigite o nome sem o '.cor' no final, bobinho!\n\n");
                    else
                    {
                        strcat(continuasave, ".cor");//colocar um .cor no final do nome do arquivo para abrir o arquivo

                        //Abrir o arquivo
                        continua = fopen(continuasave, "r");

                        if(continua == NULL)
                        {
                            printf("\nNão tem nenhum arquivo com esse nome poxa!! Digita um arquivo valido ai :(\n\n");
                            nomevalido = 0;
                        }
                    }
                }
            }while(nomevalido == 0);

            if(querojogar == 1)
            {
                if(jogoiniciado == 1)//liberar os vetores caso o jogador tenha jogado um save antes
                {
                    free(jogador.vetorrandom);
                    free(jogador.vetorjogador);
                    for(int i = 0; i < jogador.tentativasmax; i++)
                        free(jogador.historico[i]);
                    free(jogador.historico);
                }

                fgets(jogador.nome, 51, continua);//Dados do nome do save
                
                tamanhonome = strlen(jogador.nome);
                jogador.nome[tamanhonome-1] = '\0';

                fscanf(continua, "%d", &jogador.nivel);//Dados do nivel de dificuldade do save
                switch(jogador.nivel)
                {
                    case 1:
                        alocavetordificuldade(&jogador, 4, 10);
                        break;
                    case 2:
                        alocavetordificuldade(&jogador, 5, 12);
                        break;
                    case 3:
                        alocavetordificuldade(&jogador, 6, 15);
                        break;
                }

                for(int i = 0; i < jogador.quantcores; i++)
                    fscanf(continua, "%d", &jogador.vetorrandom[i]);//Dados do vetor random (gabarito)

                fscanf(continua, "%d", &jogador.tentativas);//Dados de quantas tentativas o usuario fez no save

                for(int i = 0; i < jogador.tentativas; i++)
                    for(int j = 0; j < jogador.quantcores; j++)
                        fscanf(continua, "%d", &jogador.historico[i][j]);//Dados do historico do usuario

                fclose(continua);

                printf("\nJogo de '%s' foi carregado!\n\n", jogador.nome);

                salvofinalizado = 0;
                //Aqui ele vai saber se o jogo do save escolhido ja foi finalizado ou nao, basicamente ele vai pegar a ultima jogada do historico e vai comparar com o gabarito
                //se for igual, ele vai adicionar mais um a salvofinalizado, se salvo finalizado for igual a quantidade de cores, quer dizer que ele ja acertou todas as cores!!
                for(int i = 0; i < jogador.quantcores; i++)
                    if(jogador.historico[(jogador.tentativas - 1)][i] == jogador.vetorrandom[i])
                        salvofinalizado++;

                //aqui ele tambem vai ver se o numero de tentativas tambem ja passou o numero maximo
                if((jogador.tentativas == jogador.tentativasmax) || (salvofinalizado == jogador.quantcores))
                {
                    printf("Esse jogo ja foi finalizado!!\n");
                    jogofinalizado = 1;

                    pressionatecla(tecla);
                }
                else
                {
                    jogoiniciado = 1;
                    condvitoria = 0;
                    jogofinalizado = 0;

                    coresdisponiveis();

                    printf("\nRodadas anteriores:\n");
                    mostrarodadas(jogador);//função que mostra as rodadas anteriores desse save escolhido

                    do
                    {
                        jogar(&jogador, jogador.tentativas, &condvitoria);//aqui eu passo o numero de tentativas que ele teve junto na função para que ele volte da rodada certinha que terminou
                    }while((jogador.vetorjogador[0] != 0) && (jogador.tentativas < jogador.tentativasmax) && (condvitoria == 0)); //vai continuar rodando enquanto o jogador ainda nao digitar 0 e o limite de jogadas ainda nao ter atingido o maximo OU ate ele acertar eba
                }
        
                //essa função serve para mostrar as mensagens de vitoria ou derrota e  tambem tem outra função dentro dela para saber se o jogador entrou ou nao no rank, aumentando quantrank e mudando posicao r caso isso aconteça
                //ALEM DISSO essa função tambem muda as posições do ranking geral
                vitoriaderrota(jogador, condvitoria, jogofinalizado, &posicaor, rank, &quantrank);

                if(((condvitoria == 0) || (condvitoria == 1)) && (jogofinalizado == 0))
                {
                    pressionatecla(tecla);
                }
            }
        }
        
        else if(opmenu == 'S' || opmenu == 's')
        {
            if(jogoiniciado != 1)//caso ele tenha entrado no jogo e de cara escolha salvar jogo sem nem ter um save disponivel
            {
                printf("Você ainda nem iniciou o jogo de verdade, oque que voce quer salvar ora bolas!! >:(\n");
                pressionatecla(tecla);
            }
            else//salvar o ultimo save que ele jogou
            {   
                do
                {
                    printf("Save: %s - Deseja salvar o ultimo round? (S/N): ", jogador.nome);
                    scanf(" %c", &salvar);
                    limpar_buffer();

                    if((salvar == 'S') || (salvar == 's'))//SE JA TIVER UM SAVE COM O MESMO NOME ELE SO SOBREESCREVE
                    {
                        //colocar um .cor no final do nome para criar o arquivo "nome" certinho
                        strcpy(nomearquivo, jogador.nome);
                        strcat(nomearquivo, ".cor");

                        FILE *save = fopen(nomearquivo, "w+");

                        //essas coisas aqui de baixo ele (vai criar um arquivo nome.cor caso ele nao tenha sido criado) vai salvar as informações dentro do arquivo
                        fprintf(save, "%s\n", jogador.nome);//salva o nome do jogador

                        fprintf(save, "%d\n", jogador.nivel);//salva o nivel do save

                        for(int i = 0; i < (jogador.quantcores - 1); i++)
                            fprintf(save, "%d ", jogador.vetorrandom[i]);//salva todo o gabarito menos a ultima cor    
                        fprintf(save, "%d\n", jogador.vetorrandom[(jogador.quantcores - 1)]);//salva a ultima cor com um \n para pular linha

                        fprintf(save, "%d\n", jogador.tentativas);//salva o numero de tentativas

                        for(int i = 0; i < jogador.tentativas; i++)//para salvar o historico do jogador eba
                        {
                            for(int j = 0; j < (jogador.quantcores - 1); j++)
                                fprintf(save, "%d ", jogador.historico[i][j]);//salva todo o vetor de cores da rodada i + 1 menos a ultima cor
                            fprintf(save, "%d\n", jogador.historico[i][(jogador.quantcores - 1)]);//salva a ultima cor do vetor da rodada i + 1 com um \n para pular linha
                        }

                        fclose(save);

                        printf("\nJogo Salvo!\n");
                        pressionatecla(tecla);
                    }
                    else if((salvar == 'N') || (salvar == 'n'))
                    {
                        printf("\nOperação cancelada!\n");
                        pressionatecla(tecla);
                    }
                    else
                        printf("\nDigita uma opção valida AGORAA!!!!! >:((\n\n");
                }while((salvar != 'S') && (salvar != 's') && (salvar != 'n') && (salvar != 'N'));
                //coloquei esse do while pra ele ficar rodando enquanto o usuario não digitar uma opção valida
            }
        }
        else if(opmenu == 'R' || opmenu == 'r')
        {
            printf("==============================\n");
            printf("           RANKING\n");
            printf("==============================\n\n");

            if(quantrank == 0)//caso o jogo tenha sido aberto e ninguem tiver concluido uma miseria de partida
                printf("Ainda não tem nenhum jogador no ranking... :(\n");
            else
                for(int i = 0; i < quantrank; i++)//imprime as posições e seus dados
                    printf("%dº | %s - Nivel: %d - Tentativas: %d\n", (i + 1), rank[i].rnome, rank[i].rnivel, rank[i].rtentativas);

            pressionatecla(tecla);
        }
        else if(opmenu == 'X' || opmenu == 'x')
        {
            printf("Fechando o progama...\n");

            //liberar os vetores alocados dinamicamente (so libera caso algum jogo tenha sido rodado desde o inicio do progama)
            if(jogoiniciado == 1)
            {
                free(jogador.vetorrandom);
                free(jogador.vetorjogador);
                for(int i = 0; i < jogador.tentativasmax; i++)
                    free(jogador.historico[i]);
                free(jogador.historico);
            }

            return 0;
        }
        else//Caso o usuario não digite nenhuma das opções do MENU
        {
            printf("Opção invalida!\n");
            pressionatecla(tecla);
        }
    }while(opmenu != 'X' && opmenu != 'x');
    //Ele vai ficar repetindo o MENU ate o usuario digitar "X ou x""
}

void limpar_buffer()
{
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}

void criarmenu(char *opcao, Jogo jog, int conferejogo)
{
    printf("==============================\n");
    printf(" JOGO CÓDIGO SECRETO DE CORES\n");
    printf("==============================\n");

    printf("\nOpções de jogo:\n");

    printf("A - Ajuda\n");
    printf("N - Novo Jogo\n");
    printf("C - Carregar jogo\n");
    printf("S - Salvar jogo\n");
    printf("R - Ranking\n");
    printf("X - Sair\n\n");

    if(conferejogo == 1)
        printf("(Ultimo SAVE: %s)\n\n", jog.nome);

    printf("Digite a opção: ");
    scanf(" %c", opcao);
    limpar_buffer();
    printf("\n");
}

//alocar os vetoresbaseado nas especificações da dificuldade
void alocavetordificuldade(Jogo *jog, int cores, int tent)
{
    (*jog).vetorrandom = malloc(cores * sizeof(int));
    (*jog).vetorjogador = malloc(cores * sizeof(int));

    (*jog).historico = malloc(tent * sizeof(int*));
    for(int i = 0; i < tent; i++)
        (*jog).historico[i] = malloc(cores * sizeof(int)); 

    (*jog).tentativasmax = tent;
    (*jog).quantcores = cores;
}

//Função para gerar as cores do gabarito (NAO É RODADA EM CONTINUAR JOGO PORQUE ELE JA FOI RODADO ANTES)
void gerarcores(Jogo *cor)
{
    srand(time(NULL));

    for(int i = 0; i < (*cor).quantcores; i++)
        (*cor).vetorrandom[i] = ((rand()%6) + 1);//Gera numeros aleatorios de 1 ate 6
}

void coresdisponiveis()
{
    printf("Cores disponíveis:\n");
    printf("1 - Vermelho\n");
    printf("2 - Azul\n");
    printf("3 - Verde\n");
    printf("4 - Amarelo\n");
    printf("5 - Roxo\n");
    printf("6 - Laranja\n");
}

//Função para mostrar as rodadas anteriores de um save quando for rodada em "Continuar jogo"
void mostrarodadas(Jogo vegab)
{
    int condletras;

    for(int i = 0; i < vegab.tentativas; i++)
    {
        printf("Rodada %d: ", (i + 1));

        for(int j = 0; j < vegab.quantcores; j++)
            printf("%d ", vegab.historico[i][j]); //imprime os numeros digitados na rodada i + 1

        printf("(");

        for(int j = (vegab.quantcores - 1); j > 0 ; j--)
        {
            condletras = 0;

            if(vegab.historico[i][j] == vegab.vetorrandom[j])
                condletras = 1;
            else
                for(int x = 0; x < vegab.quantcores; x++)
                    if((vegab.historico[i][j] == vegab.vetorrandom[x]) && (x != j))
                        condletras = 2;
            
            switch(condletras)
            {
                case 0:
                    printf("- ");
                    break;
                case 1:
                    printf("C ");
                    break;
                case 2:
                    printf("E ");
                    break;
            }
        }

        condletras = 0;
        if(vegab.historico[i][0] == vegab.vetorrandom[0])
            condletras = 1;
        else
            for(int x = 0; x < vegab.quantcores; x++)
                if((vegab.historico[i][0] == vegab.vetorrandom[x]) && (x != 0))
                    condletras = 2;
        switch(condletras)
        {
            case 0:
                printf("-");
                break;
            case 1:
                printf("C");
                break;
            case 2:
                printf("E");
                break;
        }

        printf(")\n");
    }
}

//Função para comparar a ultima jogada com o gabarito (retorna 1 se ganhou, retorna 0 caso não tenha ganho ainda)
int confereacerto(Jogo confereai)
{
    int somapontos = 0;

    for(int i = 0; i < confereai.quantcores; i++)
        if(confereai.vetorjogador[i] == confereai.vetorrandom[i])
            somapontos++;

    if(somapontos == confereai.quantcores)
    {
        printf("\nParabéns, %s! Você venceu! yupii\n", confereai.nome);
        printf("Tentativas: %d\n\n", confereai.tentativas);
        return 1;
    }
    
    return 0;
}

//Função que carrega os dados do arquivo binario rank e salva no vetor de structs rank da main
void carregarank(Ranking *ranking, int *qtd)
{
    FILE *arquivo = fopen("ranking.rnk", "rb");//So vai abrir caso o arquivo ranking.rnk ja exista, se nao existir vai retornar NULL

    if(arquivo == NULL)//Caso nao exista o arquivo ranking.rnk (no caso nenhum jogo tiver sido finalizado desde que o progama esta no pc), ele volta volta a quantidade de pessoas no rank como 0 e termina
    {
        *qtd = 0;
        return;
    }
    else
    {
        fread(qtd, sizeof(int), 1, arquivo);//le a quantidade de jogadores

        if(*qtd > 0)
        {
            for(int i = 0; i < *qtd; i++)
                fread(&ranking[i], sizeof(Ranking), 1, arquivo); //le o arquivo armazenando os dados jogadores em rank[0], rank[1], rank[2]... ate chegar em rank[qtd - 1]
        }
    }

    fclose(arquivo);
}

//Função que pega um jogo ja finalizado e ganho e confere se ele pode entrar ou não no ranking, se ele conseguir, altera o ranking para depois abrir o arquivo binario e salvar os novos dados
int colocarank(Ranking *RANK, int *QTD, char RNOME[51], int RNIVEL, int RTENTATIVAS)
{
    int posicao = *QTD;//aqui ele entra no fim do rank, abaixo a gente vai conferir se ele sobe ou nao no rank comparando com outros resultados
 
    //descobrir em qual posição do rank a tentativa do atual save vai entrar
    for(int i = 0; i < *QTD; i++)
    {
        if(RTENTATIVAS < RANK[i].rtentativas)//percorre o vetor RANK procurando se a jogada atual foi melhor que alguma do rank
        {
            posicao = i;//o que ta na atual posicao i vai ser passado pra posicao abaixo como a gente vai ver logo ali depois em baixo
            break;
        }
        else if((RTENTATIVAS == RANK[i].rtentativas) && (RNIVEL >= RANK[i].rnivel))//caso empate, ele vai conferir os niveis
        {
            posicao = i;
            break;
        }
    }

    if(posicao >= 10)//se ele nao for maior que nenhum do rank, ele so nao entra e termina a função :(
        return -1;//nao pode ser zero aqui porque 0 é o primeiro lugar

    int ultimaposicao;//variavel que armazena a ultima posição  do rank

    //descobrir qual a ultima posição
    if(*QTD < 10)//se ainda nao tiver preenchido toda as 10 posições (no caso a quantidade for menor que 9), ele vai colocar a ultima posição com a mesmo quantidade de jogadores - (se tiver um jogador so no ranking, a ultima posicção vai ser o rank[1], aumentando o numero de posições em 2: rank[0] e rank[1])
        ultimaposicao = *QTD;//TEM QUE SER QTD E NAO (QTD - 1) PORQUE VAI ENTRAR OS NOVOS DADOS DO ROUND NOVO
    else//caso contrario(no caso ele ja tiver preenchido as 10 posições), ele colocar a ultima posição como nove (as posições nos vetores vão de 0 a 9)
        ultimaposicao = 9;//ultima posição = 9 porque o vetor vai de [0] ate [9]

    if(*QTD < 10)//aqui ele vai adicionar +1 o numero de pessoas no rank caso ainda nao tenha 10 jogadores (para futuras alterações no rank)
        (*QTD)++;

    //passar os dados da posição i - 1 para a posição i (ultima posição) ate chegar na posição que o jogador esta
    for(int i = ultimaposicao; i > posicao; i--)
        RANK[i] = RANK[(i - 1)];//exemplo: novo save entra no ranking em terceiro de 4 posições, o quinto (ultima posição QUE ESTA VAZIA POR ENQUANTO) recebe a informação do quarto (agora o quinto nao ta mais vazio entendeu), o quarto do terceiro e o terceiro vai receber a informação do novo savelogo abaixo

    //colocar as posiçoes do novo jogo na posição dela
    strcpy(RANK[posicao].rnome, RNOME);
    RANK[posicao].rnivel = RNIVEL;
    RANK[posicao].rtentativas = RTENTATIVAS;

    FILE *arquivo = fopen("ranking.rnk", "wb");//apaga o conteudo que tava no arquivo para reescrever (SE O ARQUIVO NAO EXISTIR ATE AGORA - no caso essa for o primeiro save qeu ta sendo rodado desde que o programa foi colocado no computador - ELE CRIA UM POR CAUSA DO "W")

    fwrite(QTD, sizeof(int), 1, arquivo);//imprime a quantidade de jogadores no ranking no arquivo binario
    for(int i = 0; i < *QTD; i++)
            fwrite(&RANK[i], sizeof(Ranking), 1, arquivo);//imprime todas as informações do jogador[0] ate o jogador[QTD - 1] no arquivo binario

    fclose(arquivo);
    return posicao;//retorna a posição (entre 0 e 9) atual do save
}

//Função que é basicamente o jogo. Passando rodada inicial porque eu preciso saber se ele ta no novo jogo ou continuar jogo e tambem volta a variavel ganhou em referencia para saber se ele ganhou ou nao e assim imprimir as menssagens e as outras funções na main
void jogar(Jogo *jogador, int rodadainicial, int *ganhou)
{
    int podenaopode;//variavel para saber se o jogador usou ou nao valores validos para o vetor
    char SIMNAO;//variavel para saber se o usuario quer ou nao voltar para o menu principal
    *ganhou = 0;


    for(int i = rodadainicial; i < (*jogador).tentativasmax; i++)
    {
        podenaopode = 1;//se ele digitar um valor invalido esse valor passa para 0 e mostra na tela que o jogador nao digitou valores validos, voltando i em uma unidade

        printf("\nTentativa %d de %d\n", (i + 1), (*jogador).tentativasmax);

        printf("Digite %d cores (0 para sair): ", (*jogador).quantcores);
        scanf("%d", &(*jogador).vetorjogador[0]);

        if(((*jogador).vetorjogador[0] == 0))//codigo pra se o usuario digitar 0
        {
            limpar_buffer();//para nao atrapalhar nas opções abaixo caso o jogador digite mais numeros alem do 0

            printf("\nDeseja voltar ao menu principal? (S/N): ");
            scanf(" %c", &SIMNAO);
            limpar_buffer();
                                    
            if((SIMNAO == 'S') || (SIMNAO == 's'))
            {
                printf("\nSaindo do jogo atual...\n");
                break; //para ele sair do loop for, no do while ele impede que o jogador que digitou 0 jogue de novo
            }
            else
                i--;//aqui ele so vai voltar antes do usuario digitar 0 
                    //EXEMPLO: Estava na rodada 3 (i = 2, 2 + 1 = 3), usuario digita 0, escolhe nao voltar para o menu = volta pro inicio do for na rodada 3 de novo (i = 3 - 1, 3 - 1 + 1 = 3)
        }
        else//caso ele tenha digitado um vetor
        {
            for(int j = 1; j < (*jogador).quantcores; j++)
                scanf("%d", &(*jogador).vetorjogador[j]);

            if(((*jogador).vetorjogador[0] != 1) && ((*jogador).vetorjogador[0] != 2) && ((*jogador).vetorjogador[0] != 3) && ((*jogador).vetorjogador[0] != 4) && ((*jogador).vetorjogador[0] != 5) && ((*jogador).vetorjogador[0] != 6))
                podenaopode = 0;
            for(int j = 1; j < (*jogador).quantcores; j++)
                if(((*jogador).vetorjogador[j] != 1) && ((*jogador).vetorjogador[j] != 2) && ((*jogador).vetorjogador[j] != 3) && ((*jogador).vetorjogador[j] != 4) && ((*jogador).vetorjogador[j] != 5) && ((*jogador).vetorjogador[j] != 6))
                {
                    podenaopode = 0;
                    break;
                }
                
            if(podenaopode == 0)
            {
                printf("\nDigite valores validos para as cores!! >:((\n");
                i--;
            }
            else
            {
                //copiar as cores escolhidas no round (i + 1) para a matriz que armazena o historico
                (*jogador).historico[i][0] = (*jogador).vetorjogador[0];
                for(int j = 1; j < (*jogador).quantcores; j++)
                    (*jogador).historico[i][j] = (*jogador).vetorjogador[j];
                                            
                (*jogador).tentativas++; 

                printf("\nResultado:\n");
                mostrarodadas(*jogador);
                *ganhou = confereacerto(*jogador);//saber se o vetor mais recente do historico é igual ao gabarito

                //se condvitoria == 1 (a função confere acerto retornar 1 (jogador acertou a sequencia)), vai terminar a repetição das rodadas
                if(*ganhou == 1)
                    break;
            }
        }
    }
}

//função que imprime o gabarito depois que o jogo é finalizado e o jogador nao ganha :(
void imprimegabarito(Jogo gabarito)
{    
    for(int i = 0; i < (gabarito.quantcores - 1); i++)
    {
        switch(gabarito.vetorrandom[i])
        {
            case 1:
                printf("Vermelho - ");
                break;
            case 2:
                printf("Azul - ");
                break;
            case 3:
                printf("Verde - ");
                break;
            case 4:
                printf("Amarelo - ");
                break;
            case 5:
                printf("Roxo - ");
                break;
            case 6:
                printf("Laranja - ");
                break;
        }
    }

    switch(gabarito.vetorrandom[(gabarito.quantcores - 1)])
    {
        case 1:
            printf("Vermelho");
            break;
        case 2:
            printf("Azul");
            break;
        case 3:
            printf("Verde");
            break;
        case 4:
            printf("Amarelo");
            break;
        case 5:
            printf("Roxo");
            break;
        case 6:
            printf("Laranja");
            break;
    }
}

void vitoriaderrota(Jogo jogdor, int ganhouounao, int terminouounao, int *position, Ranking *ranke, int *quantidaderank)
{
    //Caso o usuario chegue nas tentativas maximas e não consiga ter ganhado
    if((jogdor.tentativas == jogdor.tentativasmax) && (ganhouounao == 0))
    {
        printf("\nFim de jogo! Você não conseguiu descobrir a sequência. :(\n");
        printf("Tentativas: %d\n\n", jogdor.tentativas);

        printf("A sequência correta era:\n");
        imprimegabarito(jogdor);
        printf("\n\n");
    }

    if(ganhouounao == 1)
    {
        *position = colocarank(ranke, quantidaderank, jogdor.nome, jogdor.nivel, jogdor.tentativas);//Confirma se o jogador entrou no rank

        if((*position) != -1)//nao pode ser zero aqui porque 0 é o primeiro lugar (OBS: SE FOR != -1, SEMPRE VAI SER UM NUMERO ENTRE 0 E 9, SO OLHAR NA FUNÇÃO LA EM BAIXO
            printf("Você ficou em %dº lugar no ranking!\n", ((*position) + 1));//tem que ser mais um porque a posição no vetor começa em vetor[0]
        else
            printf("Você não entrou no top 10...\n");
    }
}

void pressionatecla(char TECLA)
{
    printf("\nPressione uma tecla para o voltar ao menu principal!\n");
    scanf("%c", &TECLA);
    limpar_buffer();
    printf("\n");
}
