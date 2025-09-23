#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

PGconn *conn;

int incluir() {
    int codigo, telefone, matricula;
    char nome[51], endereco[201];

    printf("Digite o código da unidade: ");
    scanf("%d", &codigo);

    printf("Digite o nome da unidade: ");

    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    while (getchar() != '\n'); // limpa o buffer

    printf("Digite o endereço: ");
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = 0;

    printf("Digite o telefone (somente números): ");
    scanf("%d", &telefone);
    while (getchar() != '\n'); // limpa o buffer
    
    printf("Digite a matrícula da bibliotecária responsável: ");
    scanf("%d", &matricula);
    while (getchar() != '\n'); // limpa o buffer

    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO Tab_Unidades_Atendimento "
             "(codigo, nome, endereco, telefone, bibliotecaria_responsavel) "
             "VALUES (%d, '%s', '%s', %d, %d);",
             codigo, nome, endereco, telefone, matricula);

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erro ao inserir: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 1;
    }

    printf("Unidade inserida com sucesso.\n");
    PQclear(res);

    return 0;
}

int consultar() {
    const char *query = "SELECT * FROM Tab_Unidades_Atendimento";
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Erro ao consultar: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 1;
    }

    int n_rows = PQntuples(res);

    if (n_rows == 0) {
        printf("Nenhuma unidade encontrada.\n");
    } else {
        // Cabeçalho formatado
        printf("\n%-6s %-20s %-40s %-15s %-10s\n", 
               "codigo", "nome", "endereco", "telefone", "bibliotecaria");

        // Imprime cada linha com espaçamento fixo
        for (int i = 0; i < n_rows; i++) {
            printf("%-6s %-20s %-40s %-15s %-10s\n",
                   PQgetvalue(res, i, 0),
                   PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 2),
                   PQgetvalue(res, i, 3),
                   PQgetvalue(res, i, 4));
        }
    }

    PQclear(res);
    return 0;
}

int excluir() {
    int codigo;
    printf("Digite o código da unidade a ser excluída: ");
    scanf("%d", &codigo);
    while (getchar() != '\n'); // limpa o buffer

    char query[256];
    snprintf(query, sizeof(query),
             "DELETE FROM Tab_Unidades_Atendimento WHERE codigo = %d;", codigo);

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erro ao excluir: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 1;
    }

    if (PQcmdTuples(res) && atoi(PQcmdTuples(res)) == 0) {
        printf("Nenhuma unidade com o código %d foi encontrada.\n", codigo);
    } else {
        printf("Unidade excluída com sucesso.\n");
    }

    PQclear(res);

    return 0;
}

int alterar() {
    int codigo, telefone, matricula;
    char nome[51], endereco[201];

    printf("Digite o código da unidade a ser alterada: ");
    scanf("%d", &codigo);
    while (getchar() != '\n'); // limpa o buffer

    printf("Digite o novo nome da unidade: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o novo endereço: ");
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = 0;

    printf("Digite o novo telefone (somente números): ");
    scanf("%d", &telefone);
    while (getchar() != '\n');

    printf("Digite a nova matrícula da bibliotecária responsável: ");
    scanf("%d", &matricula);
    while (getchar() != '\n');

    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE Tab_Unidades_Atendimento "
        "SET nome = '%s', endereco = '%s', telefone = %d, bibliotecaria_responsavel = %d "
        "WHERE codigo = %d;",
        nome, endereco, telefone, matricula, codigo);

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erro ao alterar: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 1;
    }

    if (PQcmdTuples(res) && atoi(PQcmdTuples(res)) == 0) {
        printf("Nenhuma unidade com o código %d foi encontrada.\n", codigo);
    } else {
        printf("Unidade alterada com sucesso.\n");
    }

    PQclear(res);
    return 0;
}


int menu() {
    int opc, r;
    system("clear");
    printf("\n----- DBmanager -----\n");
    printf("1 - Incluir unidade\n");
    printf("2 - Consultar unidade(s)\n");
    printf("3 - Excluir unidade\n");
    printf("4 - Alterar unidade\n");
    printf("5 - Sair\n");
    printf("Selecione uma opção: ");
    scanf("%d", &opc);

    switch (opc) {
        case 1:
            r = incluir();
            break;
        case 2:
            r = consultar();
            break;
        case 3:
            r = excluir();
            break;
        case 4:
            r = alterar();
            break;
        case 5:
            printf("Saindo...");
            break;
        default:
            printf("Opção inválida!\n");
    }

    printf("Pressione ENTER para continuar...");
    while (getchar() != '\n'); // limpa o buffer
    getchar(); // espera o ENTER real

    return 0;
}

int main() {
    const char *conninfo = "dbname=db_biblioteca user=postgres password=admin123456 host=localhost port=5432";
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Erro ao conectar ao banco: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    printf("Conexão estabelecida com sucesso.\n\n");

    printf("Pressione ENTER para continuar...");
    getchar(); // espera o ENTER real

    int opcao = 0;
    while (opcao != 5) {
        opcao = menu();
    }

    PQfinish(conn);
    return 0;
}
