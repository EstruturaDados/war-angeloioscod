#include <stdio.h>  //Necessário para reconhcer os comandos de saída e entrada (scanf/printf)
#include <stdlib.h> // Necessário para alocação dinâmica de memória (calloc/free)
#include <string.h> // Necessário para manipulação de strings 
#include <time.h>   // Necessário para a geração de numeros aleatórios. (srand, time)

// ============================================================================
//         PROJETO WAR ESTRUTURADO - SISTEMA DE ATAQUE COM MISSOES
// ============================================================================
// Matrícula: 20250212488
// Faculdade: Estácio
// Aluno: Ângelo Oliveira dos Santos
// Cuso: Análise e desenvolvimento de sistemas
// Desafio: nível mestre
// Estrutura que representa um território no jogo War
// ============================================================================

// Definição da estrutura Território usando struct
struct Territorio {
    char nome[30]; 
    char cor[10]; 
    int tropas; 
};

typedef struct Territorio Territorio;

// ============================================================================
// NOVO: Protótipos de função para sistema de missões
// ============================================================================
void inicializarMissoes(char* missoes[], int* totalMissoes);
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, Territorio* mapa, int tamanho);
void exibirMissao(const char* missao);
void liberarMissao(char* missao);

// Protótipos de função originais
void inicializarMapa(Territorio** mapa, int* numTerritorios);
void cadastrarTerritorios(Territorio* mapa, int numTerritorios);
void exibirMapa(const Territorio* mapa, int numTerritorios);
void faseDeAtaque(Territorio* mapa, int numTerritorios, char* missaoJogador);
void atacar(Territorio* atacante, Territorio* defensor);
int rolarDado();
void liberarMemoria(Territorio* mapa, char* missaoJogador);

int main() {
    // OBRIGATÓRIO: Utilize srand(time(NULL)) para obter aleatoriedade adequada.
    srand(time(NULL));
    
    // REQUISITO: Ponteiro para o array alocado dinamicamente
    Territorio* mapa = NULL;
    int numTerritorios = 0;
    
    // ============================================================================
    // NOVO: Sistema de missões do jogador
    // ============================================================================
    char* missoes[10];  // Vetor de strings para missões
    int totalMissoes = 0;
    char* missaoJogador = NULL;  // Missão será alocada dinamicamente

    printf("=== WAR ESTRUTURADO - SIMULACAO DE BATALHAS COM MISSOES ===\n\n");
    
    // NOVO: Inicializar e sortear missão para o jogador
    inicializarMissoes(missoes, &totalMissoes);
    atribuirMissao(&missaoJogador, missoes, totalMissoes);
    
    printf("SUA MISSAO SECRETA: ");
    exibirMissao(missaoJogador);
    printf("\n");

    // Chama a função para lidar com a alocação dinâmica.
    inicializarMapa(&mapa, &numTerritorios);

    if (mapa == NULL) {
        printf("Erro na alocacao de memoria. O programa sera encerrado.\n");
        liberarMissao(missaoJogador);
        return 1;
    }

    // NOVO: Passa a missão do jogador para a fase de ataque
    faseDeAtaque(mapa, numTerritorios, missaoJogador);
    
    // NOVO: Libera memória da missão junto com o mapa
    liberarMemoria(mapa, missaoJogador);
    
    printf("\n=== FIM DO PROGRAMA. MEMORIA LIBERADA COM SUCESSO. ===\n");
    
    return 0;
}

// ============================================================================
// NOVO: Implementação do sistema de missões
// ============================================================================

/**
 * Inicializa o vetor de missões estratégicas disponíveis
 * REQUISITO: Vetor de strings com pelo menos 5 descrições diferentes
 */
void inicializarMissoes(char* missoes[], int* totalMissoes) {
    // REQUISITO: Vetor com pelo menos 5 missões diferentes
    missoes[0] = "Destruir todas as tropas da cor vermelha";
    missoes[1] = "Conquistar 3 territorios consecutivos no mapa";
    missoes[2] = "Eliminar um exercito com mais de 10 tropas totais";
    missoes[3] = "Manter 5 tropas em todos os territorios proprios";
    missoes[4] = "Conquistar todos os territorios da primeira coluna";
    missoes[5] = "Reduzir o exercito inimigo principal a zero tropas";
    
    *totalMissoes = 6;  // Total de missões disponíveis
}

/**
 * Sorteia e atribui uma missão ao jogador usando alocação dinâmica
 * REQUISITO: Uso de ponteiros e alocação dinâmica com malloc
 * REQUISITO: Passagem por referência para a missão do jogador
 */
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    // Sorteia uma missão aleatória
    int indiceMissao = rand() % totalMissoes;
    
    // REQUISITO: Alocação dinâmica para armazenar a missão
    *destino = (char*)malloc(strlen(missoes[indiceMissao]) + 1);
    
    if (*destino != NULL) {
        // REQUISITO: Copia a missão usando strcpy
        strcpy(*destino, missoes[indiceMissao]);
    } else {
        printf("Erro na alocacao de memoria para a missao.\n");
    }
}

/**
 * Verifica se a missão do jogador foi cumprida
 * REQUISITO: Função que avalia condições de vitória baseada na missão
 * REQUISITO: Passagem da missão por valor (cópia)
 */
int verificarMissao(const char* missao, Territorio* mapa, int tamanho) {
    int i, j;
    int tropasVermelhas = 0;
    int territoriosConquistados = 0;
    int tropasTotaisInimigas = 0;
    int exercitoMaior = 0;
    int territoriosCom5Tropas = 0;
    int territoriosProprios = 0;
    
    // REQUISITO: Lógica de verificação baseada na descrição da missão
    if (strstr(missao, "vermelha") != NULL) {
        // Missão: Destruir todas as tropas da cor vermelha
        for (i = 0; i < tamanho; i++) {
            if (strcmp((mapa + i)->cor, "vermelho") == 0 || 
                strcmp((mapa + i)->cor, "vermelha") == 0) {
                tropasVermelhas += (mapa + i)->tropas;
            }
        }
        return (tropasVermelhas == 0);
    }
    else if (strstr(missao, "3 territorios consecutivos") != NULL) {
        // Missão: Conquistar 3 territórios consecutivos
        for (i = 0; i < tamanho - 2; i++) {
            if ((mapa + i)->tropas > 0 && 
                (mapa + i + 1)->tropas > 0 && 
                (mapa + i + 2)->tropas > 0) {
                return 1;  // Missão cumprida se há 3 territórios consecutivos com tropas
            }
        }
        return 0;
    }
    else if (strstr(missao, "10 tropas totais") != NULL) {
        // Missão: Eliminar um exército com mais de 10 tropas totais
        for (i = 0; i < tamanho; i++) {
            if ((mapa + i)->tropas >= 10) {
                exercitoMaior = 1;
                break;
            }
        }
        return (exercitoMaior == 0);  // Missão cumprida se não há exército com 10+ tropas
    }
    else if (strstr(missao, "5 tropas") != NULL) {
        // Missão: Manter 5 tropas em todos os territórios próprios
        for (i = 0; i < tamanho; i++) {
            if ((mapa + i)->tropas > 0) {  // Considera como próprio se tem tropas
                territoriosProprios++;
                if ((mapa + i)->tropas >= 5) {
                    territoriosCom5Tropas++;
                }
            }
        }
        return (territoriosProprios > 0 && territoriosProprios == territoriosCom5Tropas);
    }
    else if (strstr(missao, "primeira coluna") != NULL) {
        // Missão: Conquistar todos os territórios da primeira coluna (primeiros 3)
        int primeiros = (tamanho < 3) ? tamanho : 3;
        for (i = 0; i < primeiros; i++) {
            if ((mapa + i)->tropas > 0) {
                territoriosConquistados++;
            }
        }
        return (territoriosConquistados == primeiros);
    }
    else if (strstr(missao, "zero tropas") != NULL) {
        // Missão: Reduzir o exército inimigo principal a zero tropas
        for (i = 0; i < tamanho; i++) {
            tropasTotaisInimigas += (mapa + i)->tropas;
        }
        return (tropasTotaisInimigas == 0);  // REQUISITO: Oponente com 0 tropas
    }
    
    return 0;  // Missão não reconhecida
}

/**
 * Exibe a missão do jogador
 * REQUISITO: Passagem por valor para exibição
 */
void exibirMissao(const char* missao) {
    if (missao != NULL) {
        printf("%s\n", missao);
    }
}

/**
 * Libera a memória alocada para a missão
 * REQUISITO: Gerenciamento adequado da memória
 */
void liberarMissao(char* missao) {
    if (missao != NULL) {
        free(missao);
    }
}

// ============================================================================
// Funções originais do sistema (mantidas com pequenas adaptações)
// ============================================================================

/**
 *Aloca memória para o array Territorio dinamicamente..
 */
void inicializarMapa(Territorio** mapa, int* numTerritorios) {
    printf("Digite o numero total de territorios que serao cadastrados: ");
    if (scanf("%d", numTerritorios) != 1 || *numTerritorios <= 0) {
        printf("Entrada invalida. Usando o padrao de 5 territorios.\n");
        *numTerritorios = 5;
    }

    // OBRIGATÓRIO: Alocacao dinamica(usando calloc)
    // Aloca memória e define todos os bytes como zero.
    *mapa = (Territorio*) calloc(*numTerritorios, sizeof(Territorio));
    
    if (*mapa == NULL) {
        return;
    }
    
    cadastrarTerritorios(*mapa, *numTerritorios);
}

/**
 *Responsável pela entrada inicial de dados para os territórios.
 */
void cadastrarTerritorios(Territorio* mapa, int numTerritorios) {
    for(int i = 0; i < numTerritorios; i++) {
        // OBRIGATÓRIO: Uso de ponteiros (Aritmética de ponteiros para acessar elementos)
        Territorio* t = mapa + i; 

        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        
        printf("Digite o nome do territorio: ");
        scanf("%29s", t->nome);
        
        printf("Digite a cor do exercito: ");
        scanf("%9s", t->cor);
        
        printf("Digite a quantidade de tropas: ");
        // OBRIGATÓRIO: Uso de ponteiros (Acessando campos usando o operador de seta ->)
        scanf("%d", &(t->tropas));
    }
}

/**
 * Exibe o estado atual do mapa.
 */
void exibirMapa(const Territorio* mapa, int numTerritorios) {
    printf("\n============================================\n");
    printf("  MAPA DO MUNDO - ESTADO ATUAL  \n");
    printf("============================================\n");
    
    for(int i = 0; i < numTerritorios; i++) {
        // OBRIGATÓRIO: Uso de ponteiros (Acessando campos usando o operador de seta ->)
        const Territorio* t = mapa + i;
        
        printf("%d. %s (Exercito %s, Tropas: %d)\n", 
            i + 1, 
            t->nome, 
            t->cor, 
            t->tropas
        );
    }
    printf("--------------------------------------------\n");
}

/**
 * Simula o lançamento de um dado (de 1 a 6).
 */
int rolarDado() {
    return (rand() % 6) + 1;
}

/**
 * Obrigatório: Funcao atacar(Territorio* atacante, Territorio* defensor)
 * Implementa a simulação de batalha e a atualização de dados.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    // OBRIGATÓRIO: Atualização de dados (se o Atacante vencer)
    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        // OBRIGATÓRIO: Uso de ponteiros (Modificação dos dados do defensor)
        defensor->tropas -= 1; 

        // Verifique se há conquista
        if (defensor->tropas == 0) {
            printf("CONQUISTA! O territorio %s foi tomado pelo Exercito %s.\n", defensor->nome, atacante->cor);
            
            // OBRIGATÓRIO: Atualização de dados (Transferir proprietário/cor)
            strcpy(defensor->cor, atacante->cor); 
            
            // OBRIGATÓRIO: Atualização de dados (Transferir metade das tropas do atacante)
            int tropasTransferidas = atacante->tropas / 2;
            
            if (tropasTransferidas < 1) tropasTransferidas = 1;
            
            atacante->tropas -= tropasTransferidas;
            defensor->tropas = tropasTransferidas;
        }

    } else {
        // OBRIGATÓRIO: Atualização de dados (se o Atacante perder/empatar)
        printf("DERROTA DO ATAQUE! O atacante %s perdeu 1 tropa.\n", atacante->nome);
        atacante->tropas -= 1;
    }
}

/**
 * Controla o ciclo de ataque e a validação de entrada.
 * ATUALIZADO: Agora verifica missão ao final de cada turno
 */
void faseDeAtaque(Territorio* mapa, int numTerritorios, char* missaoJogador) {
    int escolhaAtacante, escolhaDefensor;
    int turno = 0;

    while (1) {
        turno++;
        
        // OBRIGATÓRIO: Exibição pós-ataque (o mapa é mostrado antes de cada escolha)
        exibirMapa(mapa, numTerritorios);
        
        printf("\n--- FASE DE ATAQUE (Turno %d) ---\n", turno);
        
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", numTerritorios);
        if (scanf("%d", &escolhaAtacante) != 1) {
            while (getchar() != '\n'); 
            continue; 
        }

        if (escolhaAtacante == 0) {
            break; 
        }

        if (escolhaAtacante < 1 || escolhaAtacante > numTerritorios) {
            printf("Territorio atacante invalido. Escolha de 1 a %d.\n", numTerritorios);
            continue;
        }
        
        printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
        if (scanf("%d", &escolhaDefensor) != 1) {
            while (getchar() != '\n');
            continue; 
        }
        
        if (escolhaDefensor < 1 || escolhaDefensor > numTerritorios) {
            printf("Territorio defensor invalido. Escolha de 1 a %d.\n", numTerritorios);
            continue;
        }
        
        // OBRIGATÓRIO: Uso de ponteiros (Calcula ponteiros para atacante e defensor)
        Territorio* atacante = mapa + (escolhaAtacante - 1);
        Territorio* defensor = mapa + (escolhaDefensor - 1);

        if (atacante == defensor) {
            printf("Erro: Atacante e Defensor devem ser territorios diferentes.\n");
            continue;
        }

        // OBRIGATÓRIO: Validar as escolhas (não pode atacar a própria cor)
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Erro: Nao e permitido atacar um territorio da sua propria cor (%s).\n", atacante->cor);
            continue;
        }

        if (atacante->tropas < 1) {
             printf("Erro: O territorio atacante deve ter no minimo 2 tropas para iniciar um ataque.\n");
             continue;
        }
        
        atacar(atacante, defensor);

        // Garante que o território não fique vazio após um ataque fracassado.
        if (atacante->tropas < 1) {
            atacante->tropas = 1;
        }
        
        // ============================================================================
        // NOVO: Verificação da missão ao final de cada turno
        // REQUISITO: Verificação silenciosa da condição de vitória
        // ============================================================================
        if (verificarMissao(missaoJogador, mapa, numTerritorios)) {
            printf("\n");
            printf("============================================\n");
            printf("   PARABENS! MISSAO CUMPRIDA!\n");
            printf("============================================\n");
            printf("Voce cumpriu sua missao: ");
            exibirMissao(missaoJogador);
            printf("VITORIA DO JOGADOR!\n");
            printf("============================================\n");
            break;
        }
    }
}

/**
 * OBRIGATÓRIO: Função void liberarMemória(Territorio* mapa)
 * ATUALIZADO: Agora também libera a memória da missão
 */
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    // REQUISITO: Liberação da memória do mapa
    if (mapa != NULL) {
        free(mapa);
    }
    
    // NOVO: Liberação da memória da missão
    liberarMissao(missaoJogador);
}