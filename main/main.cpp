#include <cstdio>  // fgets, sscanf, printf, scanf, stdin
#include <cstdlib> // system
#include <cstring>  // strcspn
#include <cctype>  // isdigit
#include <cstring> // strcmp, strcasecmp, strstr

struct Veiculo {
    char placa[8];
    char marca[20];
    char modelo[20];
    int ano;
};

struct Cliente {
    int id;
    char nome[31];
    char telefone[11];
    char cpf_cnpj[14];
    Veiculo veiculo;
    Cliente *prox;
};

struct ListaClientes {
    Cliente *cliente;
    ListaClientes *prox;
};

struct FilaAtendimento {
    Cliente *cliente;
    int numeroAtendimento;
    FilaAtendimento *prox;
};

struct PilhaHistorico {
    Cliente *cliente;
    char status[30]; 
    PilhaHistorico *prox;
};

// Ponteiros para estruturas
Cliente *inicioLista = NULL;
int idAtualCliente = 1;
FilaAtendimento *frenteFila = NULL;
FilaAtendimento *trasFila = NULL;
int numeroAtualAtendimento = 1;
PilhaHistorico *topoPilha = NULL;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void safe_fgets(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin)) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

bool apenasNumeros(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

int obterOpcaoMenu() {
    char buffer[10];
    int opcao;
    while (true) {
        printf("________________________________________________________\n");
        printf("<====> SISTEMA DE ATENDIMENTO DA OFICINA BUGZERO <====>\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Listar Todos os Cadastros\n");
        printf("3 - Consultar Cliente por Nome\n");
        printf("4 - Editar Cadastro - por ID\n");
        printf("5 - Remover Cadastro de Cliente\n");
        printf("------------------------------------------\n");
        printf("6 - Fila de Atendimento Atual\n");
        printf("7 - Adicionar Cliente a Fila - por ID\n");
        printf("8 - Priorizar Cliente na Fila\n");
        printf("9 - Concluir Atendimento - por ID\n");
        printf("10 - Remover Cliente da Fila - por ID\n");
        printf("------------------------------------------\n");
        printf("11 - Historico de Atendimentos\n");
        printf("12 - Limpar Historico\n");
        printf("------------------------------------------\n");
        printf("0 - Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &opcao) == 1) {
                return opcao;
            }
        }
        printf("\nEntrada invalida. Por favor, digite um numero.\n");
        system("pause");
        system("cls");
    }
}

void inserirCliente() {
    Cliente *novoCliente = new Cliente;
    novoCliente->id = idAtualCliente++;
    novoCliente->prox = NULL;

    printf("Nome: ");
    safe_fgets(novoCliente->nome, 31);

    // Validar telefone
    while (true) {
        printf("Telefone (apenas numeros): ");
        safe_fgets(novoCliente->telefone, 11);
        if (strlen(novoCliente->telefone) == 10 || strlen(novoCliente->telefone) == 11) {
            if (apenasNumeros(novoCliente->telefone)) {
                break;
            } else {
                printf("Telefone deve conter apenas numeros.\n");
            }
        } else {
            printf("Telefone deve ter 10 ou 11 digitos.\n");
        }
    }

    // Validar CPF/CNPJ
    while (true) {
        printf("CPF/CNPJ (apenas numeros): ");
        safe_fgets(novoCliente->cpf_cnpj, 14);
        int len = strlen(novoCliente->cpf_cnpj);
        if (len == 11 || len == 14) {
            if (apenasNumeros(novoCliente->cpf_cnpj)) {
                break;
            } else {
                printf("CPF/CNPJ deve conter apenas numeros.\n");
            }
        } else {
            printf("CPF deve ter 11 digitos e CNPJ 14 digitos.\n");
        }
    }

    printf("Placa: ");
    safe_fgets(novoCliente->veiculo.placa, 9);

    printf("Marca: ");
    safe_fgets(novoCliente->veiculo.marca, 20);

    printf("Modelo: ");
    safe_fgets(novoCliente->veiculo.modelo, 20);

    // Validar ano
    int anoTemp;
    char bufferAno[10];
    while (true) {
        printf("Ano: ");
        if (fgets(bufferAno, sizeof(bufferAno), stdin)) {
            bufferAno[strcspn(bufferAno, "\n")] = '\0';
            if (sscanf(bufferAno, "%d", &anoTemp) == 1) {
                if (anoTemp > 1800 && anoTemp <= 3000) {
                    break;
                } else {
                    printf("Ano invalido, digite um ano correto.\n");
                }
            } else {
                printf("Entrada invalida, digite apenas numeros.\n");
            }
        }
    }
    novoCliente->veiculo.ano = anoTemp;

    if (inicioLista == NULL) {
        inicioLista = novoCliente;
    } else {
        Cliente *aux = inicioLista;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novoCliente;
    }

    // Adiciona automaticamente ao atendimento
    FilaAtendimento *novoAtendimento = new FilaAtendimento;
    novoAtendimento->cliente = novoCliente;
    novoAtendimento->numeroAtendimento = numeroAtualAtendimento++;
    novoAtendimento->prox = NULL;

    if (frenteFila == NULL) {
        frenteFila = novoAtendimento;
        trasFila = novoAtendimento;
    } else {
        trasFila->prox = novoAtendimento;
        trasFila = novoAtendimento;
    }

    printf("\nCliente inserido com sucesso e adicionado a fila de atendimento!!!\n");
    system("pause");
}

void listarClientes() {
    Cliente *aux = inicioLista;
    if (aux == NULL) {
        printf("\nLista de clientes vazia!\n");
    } else {
        printf("\n=== LISTA DE CLIENTES ===\n");
        while (aux != NULL) {
            printf("ID: %d\n", aux->id);
            printf("Nome: %s\n", aux->nome);
            printf("Telefone: %s\n", aux->telefone);
            printf("CPF/CNPJ: %s\n", aux->cpf_cnpj);
            printf("Veiculo: %s %s (%s)\n", aux->veiculo.marca, aux->veiculo.modelo, aux->veiculo.placa);
            printf("----------------------------\n");
            aux = aux->prox;
        }
    }
    system("pause");
}

void consultarClientePorNome() {
    if (inicioLista == NULL) {
        printf("Lista de clientes vazia.\n");
        system("pause");
        return;
    }

    char nomeBusca[100];
    printf("Digite o nome (ou parte do nome) do cliente para buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; // remove '\n'

    bool encontrado = false;
    Cliente* aux = inicioLista;
    while (aux != NULL) {
        
        char nomeClienteLower[100];
        char nomeBuscaLower[100];
        strcpy(nomeClienteLower, aux->nome);
        strcpy(nomeBuscaLower, nomeBusca);

        for (int i = 0; nomeClienteLower[i]; i++) nomeClienteLower[i] = tolower(nomeClienteLower[i]);
        for (int i = 0; nomeBuscaLower[i]; i++) nomeBuscaLower[i] = tolower(nomeBuscaLower[i]);

        if (strstr(nomeClienteLower, nomeBuscaLower)) {
            printf("\nID: %d\n", aux->id);
            printf("Nome: %s\n", aux->nome);
            printf("Telefone: %s\n", aux->telefone);
            printf("Veiculo: %s %s (%s)\n", aux->veiculo.marca, aux->veiculo.modelo, aux->veiculo.placa);
            encontrado = true;
        }

        aux = aux->prox;
    }

    if (!encontrado) {
        printf("Nenhum cliente encontrado com esse nome.\n");
    }

    system("pause");
}

void editarCliente() {
    int idEditar;
    printf("Digite o ID do cliente que deseja editar: "); //por ID
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &idEditar) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    Cliente *aux = inicioLista;
    while (aux != NULL && aux->id != idEditar) {
        aux = aux->prox;
    }

    if (aux == NULL) {
        printf("Cliente com ID %d nao encontrado.\n", idEditar);
        system("pause");
        return;
    }

    printf("Editando cliente ID %d:\n", idEditar);

    printf("Nome atual: %s\nNovo nome (enter para manter): ", aux->nome);
    char entrada[31];
    safe_fgets(entrada, 31);
    if (strlen(entrada) > 0) {
        strncpy(aux->nome, entrada, 31);
    }

    while (true) {
        printf("Telefone atual: %s\nNovo telefone (apenas numeros, 10 ou 11 digitos, ou enter para manter): ", aux->telefone);
        safe_fgets(entrada, 11);
        if (strlen(entrada) == 0) break;
        if ((strlen(entrada) == 10 || strlen(entrada) == 11) && apenasNumeros(entrada)) {
            strncpy(aux->telefone, entrada, 11);
            break;
        }
        printf("Telefone invalido.\n");
    }

    while (true) {
        printf("CPF/CNPJ atual: %s\nNovo CPF/CNPJ (11 ou 14 numeros, ou enter para manter): ", aux->cpf_cnpj);
        safe_fgets(entrada, 14);
        int len = strlen(entrada);
        if (len == 0) break;
        if ((len == 11 || len == 14) && apenasNumeros(entrada)) {
            strncpy(aux->cpf_cnpj, entrada, 14);
            break;
        }
        printf("CPF/CNPJ invalido.\n");
    }

    printf("Placa atual: %s\nNova placa (enter para manter): ", aux->veiculo.placa);
    safe_fgets(entrada, 8);
    if (strlen(entrada) > 0) {
        strncpy(aux->veiculo.placa, entrada, 8);
    }

    printf("Marca atual: %s\nNova marca (enter para manter): ", aux->veiculo.marca);
    safe_fgets(entrada, 20);
    if (strlen(entrada) > 0) {
        strncpy(aux->veiculo.marca, entrada, 20);
    }

    printf("Modelo atual: %s\nNovo modelo (enter para manter): ", aux->veiculo.modelo);
    safe_fgets(entrada, 20);
    if (strlen(entrada) > 0) {
        strncpy(aux->veiculo.modelo, entrada, 20);
    }

    char bufferAno[10];
    int anoTemp;
    while (true) {
        printf("Ano atual: %d\nNovo ano (enter para manter): ", aux->veiculo.ano);
        safe_fgets(bufferAno, 10);
        if (strlen(bufferAno) == 0) break;
        if (sscanf(bufferAno, "%d", &anoTemp) == 1 && anoTemp > 1800 && anoTemp <= 3000) {
            aux->veiculo.ano = anoTemp;
            break;
        }
        printf("Ano invalido.\n");
    }

    printf("Cliente editado com sucesso!!!\n");
    system("pause");
}

// Remover cliente da lista (e da fila de atendimento)
void removerClienteLista() {
    if (inicioLista == NULL) {
        printf("Lista vazia!\n");
        system("pause");
        return;
    }
    int idRemover;
    printf("Digite o ID do cliente para remover da lista: ");
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &idRemover) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    Cliente *ant = NULL;
    Cliente *aux = inicioLista;
    while (aux != NULL && aux->id != idRemover) {
        ant = aux;
        aux = aux->prox;
    }
    if (aux == NULL) {
        printf("Cliente nao encontrado.\n");
        system("pause");
        return;
    }
    
    if (ant == NULL) {
        inicioLista = aux->prox;
    } else {
        ant->prox = aux->prox;
    }
    
    FilaAtendimento *antFila = NULL;
    FilaAtendimento *auxFila = frenteFila;

    while (auxFila != NULL) {
        if (auxFila->cliente->id == idRemover) {
            FilaAtendimento *paraRemover = auxFila;
            
            if (antFila == NULL) { 
                frenteFila = auxFila->prox;
                auxFila = frenteFila;
            } else {
                antFila->prox = auxFila->prox;
                auxFila = antFila->prox;
            }
            
            if (paraRemover == trasFila) {
                trasFila = antFila;
            }
            
            PilhaHistorico *novoHist = new PilhaHistorico;
            novoHist->cliente = paraRemover->cliente;
            strcpy(novoHist->status, "Removido junto com cadastro!");
            novoHist->prox = topoPilha;
            topoPilha = novoHist;

            delete paraRemover;
        } else {
            antFila = auxFila;
            auxFila = auxFila->prox;
        }
    }

    delete aux;

    printf("Cliente removido da lista e da fila de atendimento.\n");
    system("pause");
}

void listarFila() {
    if (frenteFila == NULL) {
        printf("\nFila de atendimento vazia!\n");
    } else {
        printf("\n=== FILA DE ATENDIMENTO ===\n");
        FilaAtendimento *aux = frenteFila;
        int pos = 1;
        while (aux != NULL) {
            printf("Numero na fila: %dº\n", pos);
            printf("ID Cliente: %d\n", aux->cliente->id);
            printf("Nome: %s\n", aux->cliente->nome);
            printf("Telefone: %s\n", aux->cliente->telefone);
            printf("Veiculo: %s - %s\n", aux->cliente->veiculo.modelo, aux->cliente->veiculo.placa);
            printf("--------------------------\n");
            aux = aux->prox;
            pos++;
        }
    }
    system("pause");
}

void adicionarClienteFilaPorID() {
    if (inicioLista == NULL) {
        printf("Nenhum cliente cadastrado!\n");
        system("pause");
        return;
    }

    int id;
    printf("Digite o ID do cliente que deseja adicionar a fila: ");
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &id) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    // Verificar se cliente já está na fila
    FilaAtendimento *verifica = frenteFila;
    while (verifica != NULL) {
        if (verifica->cliente->id == id) {
            printf("Cliente ja esta na fila de atendimento.\n");
            system("pause");
            return;
        }
        verifica = verifica->prox;
    }

    // Buscar cliente na lista de clientes cadastrados
    Cliente *aux = inicioLista;
    Cliente *clienteEncontrado = NULL;

    while (aux != NULL) {
        if (aux->id == id) {
            clienteEncontrado = aux;
            break;
        }
        aux = aux->prox;
    }

    if (clienteEncontrado == NULL) {
        printf("Cliente com ID %d nao encontrado!\n", id);
        system("pause");
        return;
    }

    FilaAtendimento *novo = new FilaAtendimento;
    novo->cliente = clienteEncontrado;
    novo->numeroAtendimento = numeroAtualAtendimento++;
    novo->prox = NULL;

    if (frenteFila == NULL) {
        frenteFila = novo;
        trasFila = novo;
    } else {
        trasFila->prox = novo;
        trasFila = novo;
    }

    printf("Cliente %s adicionado a fila com sucesso!\n", clienteEncontrado->nome);
    system("pause");
}


void removerClienteFila() {
    if (frenteFila == NULL) {
        printf("Fila de atendimento vazia!\n");
        system("pause");
        return;
    }

    int id;
    printf("Digite o ID do cliente para remover da fila: ");
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &id) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    FilaAtendimento *anterior = NULL;
    FilaAtendimento *atual = frenteFila;

    while (atual != NULL && atual->cliente->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente com ID %d nao encontrado na fila.\n", id);
        system("pause");
        return;
    }

    if (anterior == NULL) {
        frenteFila = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == trasFila) {
        trasFila = anterior;
    }

    PilhaHistorico *novoHist = new PilhaHistorico;
    novoHist->cliente = atual->cliente;
    strcpy(novoHist->status, "Excluido da fila");
    novoHist->prox = topoPilha;
    topoPilha = novoHist;

    printf("Cliente %s removido da fila e registrado no historico!\n", atual->cliente->nome);
    delete atual;
    system("pause");
}


void priorizarCliente() {
    if (frenteFila == NULL) {
        printf("Fila vazia.\n");
        system("pause");
        return;
    }

    int idPrioridade;
    printf("Digite o ID do cliente a ser priorizado: ");
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &idPrioridade) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    if (frenteFila->cliente->id == idPrioridade) {
        printf("Cliente ja esta em primeiro na fila!\n");
        system("pause");
        return;
    }

    FilaAtendimento *anterior = frenteFila;
    FilaAtendimento *atual = frenteFila->prox;

    while (atual != NULL && atual->cliente->id != idPrioridade) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente com ID %d nao encontrado na fila.\n", idPrioridade);
        system("pause");
        return;
    }

    anterior->prox = atual->prox;
    if (atual == trasFila) {
        trasFila = anterior;
    }

    atual->prox = frenteFila;
    frenteFila = atual;

    printf("Cliente com ID %d foi priorizado na fila.\n", idPrioridade);
    system("pause");
}

void concluirAtendimentoPorID() {
    if (frenteFila == NULL) {
        printf("Fila de atendimento vazia!\n");
        system("pause");
        return;
    }

    int id;
    printf("Digite o ID do cliente para concluir atendimento: ");
    char buffer[10];
    if (!(fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &id) == 1)) {
        printf("Entrada invalida.\n");
        system("pause");
        return;
    }

    FilaAtendimento *anterior = NULL;
    FilaAtendimento *atual = frenteFila;

    while (atual != NULL && atual->cliente->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Cliente com ID %d nao esta na fila.\n", id);
        system("pause");
        return;
    }

    if (anterior == NULL) {
        frenteFila = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == trasFila) {
        trasFila = anterior;
    }

    PilhaHistorico *novoHist = new PilhaHistorico;
    novoHist->cliente = atual->cliente;
    strcpy(novoHist->status, "Concluido");
    novoHist->prox = topoPilha;
    topoPilha = novoHist;

    printf("Atendimento do cliente %s concluido!\n", atual->cliente->nome);
    delete atual;
    system("pause");
}

void exibirHistorico() {
    if (topoPilha == NULL) {
        printf("Historico vazio!\n");
    } else {
        printf("\n=== HISTORICO DE ATENDIMENTOS ===\n");
        PilhaHistorico *aux = topoPilha;
        while (aux != NULL) {
            printf("ID Cliente: %d\n", aux->cliente->id);
            printf("Nome: %s\n", aux->cliente->nome);
            printf("Telefone: %s\n", aux->cliente->telefone);
            printf("Status: %s\n", aux->status); 
            printf("--------------------------\n");
            aux = aux->prox;
        }
    }
    system("pause");
}


void limparHistorico() {
    PilhaHistorico *atual = topoPilha;
    while (atual != NULL) {
        PilhaHistorico *temp = atual;
        atual = atual->prox;
        delete temp;
    }
    topoPilha = NULL;
    printf("Historico limpo com sucesso!\n");
    system("pause");
}

 int main() {
    int opcao;
    do {
        system("cls");
        opcao = obterOpcaoMenu();

        switch (opcao) {
            case 1:
                inserirCliente();
                break;
            case 2:
                listarClientes();
                break;
            case 3:
                consultarClientePorNome();
                break;
            case 4:
                listarClientes(); // Para mostrar a lista antes de editar
                editarCliente();
                break;
            case 5:
                removerClienteLista();
                break;
            case 6:
                listarFila();
                break;
            case 7:
                adicionarClienteFilaPorID();
                break;
            case 8:
                priorizarCliente();
                break;
            case 9:
                concluirAtendimentoPorID();
                break;
            case 10:
                removerClienteFila();
                break;
            case 11:
                exibirHistorico();
                break;
            case 12:
                limparHistorico();
                break;
            case 0:
                printf("Fechando sistema. Ate a proxima!!!\n");
                break;
            default:
                printf("Opcao invalida.\n");
                system("pause");
        }
    } while (opcao != 0);

    return 0;
}