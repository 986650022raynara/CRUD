#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTATOS 1000
//NUMERAÇÃO DOS MENUS---------------------------------------------------------------------------------------------
typedef enum{
    Menu_Sair = -1,
    Menu_Principal,
    Menu_Cadastrar,
    Menu_Descadastrar,
    Menu_Listar_Todos,
} Telas;
//ESTRUTURA DE UM CONTATO-----------------------------------------------------------------------------------------
typedef struct {
    char Nome[128];
    char Telefone[64];
} Contatos;
//ESTRUTURA DA AGENDA---------------------------------------------------------------------------------------------
typedef struct {
    Telas menus;
    int Total_contatos;
    Contatos contato[MAX_CONTATOS];
} Agenda_Telefonica;


int main() {
    //DECLARAÇÃO DAS VARIÁVEIS------------------------------------------------------------------------------------
    Agenda_Telefonica agenda;
    int opcao;
    //ZERANDO A AGENDA--------------------------------------------------------------------------------------------
    memset(&agenda, 0x0, sizeof(agenda));
    //DEFININDO O MENU PRINCIPAL----------------------------------------------------------------------------------
    agenda.menus = Menu_Principal;
    //ABRINDO ARQUIVO BINÁRIO-------------------------------------------------------------------------------------
    FILE *memoria = fopen("contatos.bin","rb");
    //VERIFICANDO SE ARQUIVO FOI ABERTO---------------------------------------------------------------------------
    if(memoria == NULL){
        printf("Erro na leitura do arquivo ou arquivo nao existente!\n");
        printf("Voce deve criar o arquivo contatos.bin na primeira execucao!");
        return 1;
    }
    size_t lidos = fread(agenda.contato, sizeof(Contatos),MAX_CONTATOS,memoria);
    agenda.Total_contatos = (int)lidos;
    printf("%zd Contatos Armazenados!\n", lidos);
    system("pause");
    fclose(memoria);

    //EXECUÇÃO DO APLICATIVO-------------------------------------------------------------------------------------
    while (agenda.menus != Menu_Sair){
        switch (agenda.menus) {
            case Menu_Principal:
                system("cls");
                printf("---------------------------\n");
                printf("-----AGENDA TELEFONICA-----\n");
                printf("---------------------------\n");
                printf("\n1- Cadastrar Contato\n2- Excluir Contato\n3- Lista de Contatos\n0- Sair");
                printf("\nDigite a opcao: ");
                scanf("%d", &opcao);
                switch (opcao) {
                    case 0:
                        agenda.menus = Menu_Sair;
                        break;
                    case 1:
                        agenda.menus = Menu_Cadastrar;
                        break;
                    case 2:
                        agenda.menus = Menu_Descadastrar;
                        break;
                    case 3:
                        agenda.menus = Menu_Listar_Todos;
                        break;
                    default:
                        agenda.menus = 4;
                }
                break;
            case Menu_Cadastrar:
                system("cls");
                //Verificando Espaço na agenda..................................................
                if(agenda.Total_contatos >= MAX_CONTATOS){
                    printf("Agenda estah lotada, nao eh possivel adicionar mais contatos!\n");
                    system("pause");
                }
                //Entrando na tela de Cadastro..................................................
                else{
                    printf("----------------------------\n");
                    printf("------TELA DE CADASTRO------\n");
                    printf("----------------------------\n");
                    Contatos novo;
                    char opcao2 = 's';
                    //Esvaziando Buffer...................................................
                    int c = 0;
                    while ((c = getchar()) != '\n' && c != EOF){}
                    //Cadastrando dados...................................................
                    do{
                        memset(&novo,0x0,sizeof(novo));
                        system("cls");
                        printf("\nDigite um nome para o contato: ");
                        fgets(novo.Nome, 128,stdin);
                        int id = 0;
                        while(novo.Nome[id] != '\0' && novo.Nome[id] != '\n')
                            id++;
                        novo.Nome[id] = '\0';
                        printf("\nDigite o telefone do contato: ");
                        fgets(novo.Telefone,64,stdin);
                        int id1 = 0;
                        while(novo.Telefone[id1] != '\0' && novo.Telefone[id1] != '\n')
                            id1++;
                        novo.Telefone[id1] = '\0';
                        printf("\nTem certeza que deseja adicionar este contato?(s=sim/n=nao)\n");
                        opcao2 = getchar();
                        int c = 0;
                        while ((c = getchar()) != '\n' && c != EOF){}
                    } while (opcao2 == 'N' || opcao2 == 'n');
                    agenda.contato[agenda.Total_contatos] = novo;
                    agenda.Total_contatos++;
                    //Abrindo arquivo para gravar dados.....................................................
                    memoria = fopen("contatos.bin","w+b");
                    if(memoria == NULL){
                        printf("Erro ao abrir arquivo!");
                        return 0;
                    }
                    else{
                        //Gravando Contato no arquivo....................................
                        size_t salvos = fwrite(agenda.contato, sizeof(Contatos),agenda.Total_contatos,memoria);
                        //Verificando Erro ao gravar.....................................
                        if(salvos != (size_t)agenda.Total_contatos){
                            printf("Erro ao salvar contatos na memoria do arquivo contatos.bin!\n");
                            return 1;
                        }
                        //Fechando Arquivo...............................................
                        fclose(memoria);
                    }
                }
                agenda.menus = Menu_Principal;
                break;
            case Menu_Descadastrar:
                system("cls");
                if(agenda.Total_contatos == 0){
                    printf("Nao hah nenhum contato cadastrado para remover!\n");
                    system("pause");
                }
                else{
                    printf("----------------------------\n");
                    printf("------TELA DE EXCLUSAO------\n");
                    printf("----------------------------\n");

                    //Mostrando todos os contatos disponiveis....................................
                    for(int i =0;i<agenda.Total_contatos;i++){
                        printf("Contato %d:\n", i);
                        printf("\tNome: %s\n", agenda.contato[i].Nome);
                        printf("\tTelefone: %s\n", agenda.contato[i].Telefone);
                        printf("------------------------------------------------------------------------------\n");
                    }
                    //Recebendo número do contato a excluir......................................
                    int excluir = -1;
                    do{
                        printf("Digite o numero do caontato para excluir: ");
                        scanf("%d", &excluir);

                        //Esvaziando Buffer...................................................
                        int c = 0;
                        while ((c = getchar()) != '\n' && c != EOF){}
                    } while (excluir <0 || excluir > agenda.Total_contatos);
                    //Removendo Contato escolhido................................................
                    for(int i = excluir; i<agenda.Total_contatos-1;i++){
                        agenda.contato[i] = agenda.contato[i+1];
                    }
                    //Atualizando número total de contatos.......................................
                    agenda.Total_contatos--;
                    //Abrindo arquivo para atualizar.............................................
                    memoria = fopen("contatos.bin","w+b");
                    if(memoria == NULL){
                        printf("Erro ao abrir arquivo!");
                        return 0;
                    }
                    else{
                        //Regravando Contato no arquivo....................................
                        size_t salvos = fwrite(agenda.contato, sizeof(Contatos),agenda.Total_contatos,memoria);
                        //Verificando Erro ao regravar.....................................
                        if(salvos != (size_t)agenda.Total_contatos){
                            printf("Erro ao salvar contatos na memoria do arquivo contatos.bin!\n");
                            return 1;
                        }
                        //Fechando Arquivo...............................................
                        fclose(memoria);
                    }
                }
                agenda.menus = Menu_Principal;
                break;
            case Menu_Listar_Todos:
                system("cls");
                if(agenda.Total_contatos == 0){
                    printf("Nao hah nenhum contato cadastrado para listar!\n");
                    system("pause");
                }
                else{
                    printf("---------------------------\n");
                    printf("-----LISTANDO CONTATOS-----\n");
                    printf("---------------------------\n");
                    for(int i = 0;i<agenda.Total_contatos;i++){
                        printf("Contato %d:\n", i);
                        printf("\tNome: %s\n", agenda.contato[i].Nome);
                        printf("\tTelefone: %s\n", agenda.contato[i].Telefone);
                        printf("------------------------------------------------------------------------------\n");
                    }
                }
                system("pause");
                agenda.menus = Menu_Principal;
                break;
            default:
                system("cls");
                printf("\nOpcao Invalida!\n");
                system("pause");
                agenda.menus = Menu_Principal;
        }
    }

}