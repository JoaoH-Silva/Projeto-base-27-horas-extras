#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPLOYEES_FILE "funcionarios.dat"
#define EMPLOYEES_TXT "funcionarios.txt"
#define RECORDS_FILE "registros.dat"
#define TEMP_FILE "temp.dat"

#define MAX_INPUT 100

typedef struct {
    char nome[50];
    int matricula;
    char funcao[50];
    float valorHora;
} Funcionario;

typedef struct {
    int matricula;
    char data[11];
    float horas;
    char status[15];
} Registro;

void exibirMenu();
void carregarCredenciais(char *usuario, char *senha);
void salvarCredenciais(const char *usuario, const char *senha);
int autenticar();
void cadastrarFuncionario();
void listarFuncionarios();
void registrarHoras();
void listarRegistros();
void calcularSalarioFuncionario();
void buscarFuncionarioPorNome();
void buscarFuncionarioPorMatricula();
void editarFuncionario();
void apagarFuncionario();
void exportarCSV();

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerLinha(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin)) {
        size_t ln = strlen(buffer) - 1;
        if (buffer[ln] == '\n') buffer[ln] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

void carregarCredenciais(char *usuario, char *senha) {
    FILE *f = fopen("admin.txt", "r");
    if (!f) {
        printf("Nenhuma credencial encontrada. Vamos criar um admin.\n");
        lerLinha("Usuário admin (ex: admin): ", usuario, 50);
        lerLinha("Senha admin: ", senha, 50);
        salvarCredenciais(usuario, senha);
        printf("Credenciais salvas em admin.txt\n");
        return;
    }

    if (!fgets(usuario, 50, f)) usuario[0] = '\0';
    else {
        size_t ln = strlen(usuario) - 1;
        if (usuario[ln] == '\n') usuario[ln] = '\0';
    }

    if (!fgets(senha, 50, f)) senha[0] = '\0';
    else {
        size_t ln = strlen(senha) - 1;
        if (senha[ln] == '\n') senha[ln] = '\0';
    }

    fclose(f);
}

void salvarCredenciais(const char *usuario, const char *senha) {
    FILE *f = fopen("admin.txt", "w");
    if (!f) {
        printf("Erro ao salvar credenciais em admin.txt\n");
        return;
    }
    fprintf(f, "%s\n%s\n", usuario, senha);
    fclose(f);
}

int autenticar() {
    char usuario[50], senha[50];
    char uInput[50], sInput[50];
    carregarCredenciais(usuario, senha);

    int tentativas = 0;
    while (tentativas < 3) {
        lerLinha("Usuário: ", uInput, sizeof(uInput));
        lerLinha("Senha: ", sInput, sizeof(sInput));

        if (strcmp(uInput, usuario) == 0 && strcmp(sInput, senha) == 0) {
            printf("\nAcesso concedido!\n\n");
            return 1;
        } else {
            printf("Usuário ou senha incorretos. Tentativas restantes: %d\n", 2 - tentativas);
            tentativas++;
        }
    }

    printf("\nAcesso negado. Programa será encerrado.\n");
    return 0;
}

void exibirMenu() {
    printf("=============================================\n");
    printf("      SISTEMA DE GESTÃO DE FUNCIONÁRIOS\n");
    printf("=============================================\n");
    printf("1 - Cadastrar Funcionário\n");
    printf("2 - Listar Funcionários\n");
    printf("3 - Registrar Horas Trabalhadas\n");
    printf("4 - Listar Registros de Horas\n");
    printf("5 - Calcular Salário (por matrícula)\n");
    printf("6 - Buscar Funcionário por Nome\n");
    printf("7 - Buscar Funcionário por Matrícula\n");
    printf("8 - Editar Funcionário\n");
    printf("9 - Apagar Funcionário\n");
    printf("10 - Exportar Funcionários para CSV\n");
    printf("11 - Alterar credenciais admin\n");
    printf("0 - Sair\n");
    printf("=============================================\n");
    printf("Escolha uma opção: ");
}

void salvarFuncionarioTXT(Funcionario f) {
    FILE *txtFile = fopen(EMPLOYEES_TXT, "a");
    if (!txtFile) return;
    fprintf(txtFile, "Nome: %s\n", f.nome);
    fprintf(txtFile, "Matrícula: %d\n", f.matricula);
    fprintf(txtFile, "Função: %s\n", f.funcao);
    fprintf(txtFile, "Valor/Hora: R$ %.2f\n", f.valorHora);
    fprintf(txtFile, "-------------------------------\n");
    fclose(txtFile);
}

int matriculaExiste(int matricula) {
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    if (!file) return 0;
    Funcionario f;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.matricula == matricula) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void cadastrarFuncionario() {
    Funcionario f;
    printf("\n--- CADASTRO DE FUNCIONÁRIO ---\n");
    lerLinha("Nome: ", f.nome, sizeof(f.nome));
    char buffer[MAX_INPUT];
    lerLinha("Matrícula (número): ", buffer, sizeof(buffer));
    f.matricula = atoi(buffer);

    if (f.matricula <= 0) {
        printf("Matrícula inválida. Cadastro cancelado.\n");
        return;
    }

    if (matriculaExiste(f.matricula)) {
        printf("Já existe funcionário com essa matrícula. Cadastro cancelado.\n");
        return;
    }

    lerLinha("Função/Cargo: ", f.funcao, sizeof(f.funcao));
    lerLinha("Valor da hora (ex: 25.50): ", buffer, sizeof(buffer));
    f.valorHora = (float)atof(buffer);

    FILE *file = fopen(EMPLOYEES_FILE, "ab");
    if (!file) {
        printf("Erro ao abrir arquivo de funcionários.\n");
        return;
    }
    fwrite(&f, sizeof(Funcionario), 1, file);
    fclose(file);

    salvarFuncionarioTXT(f);
    printf("Funcionário cadastrado com sucesso!\n");
}

void listarFuncionarios() {
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    if (!file) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    Funcionario f;
    printf("\n--- LISTA DE FUNCIONÁRIOS ---\n");
    printf("%-25s | %-10s | %-20s | %-10s\n", "Nome", "Matrícula", "Função", "Valor/Hora");
    printf("---------------------------------------------------------------\n");
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        printf("%-25s | %-10d | %-20s | R$ %.2f\n", f.nome, f.matricula, f.funcao, f.valorHora);
    }
    fclose(file);
}

void registrarHoras() {
    Registro r;
    char buffer[MAX_INPUT];
    lerLinha("Matrícula: ", buffer, sizeof(buffer));
    r.matricula = atoi(buffer);
    if (r.matricula <= 0 || !matriculaExiste(r.matricula)) {
        printf("Matrícula inválida ou não cadastrada. Registro cancelado.\n");
        return;
    }
    lerLinha("Data (dd/mm/aaaa): ", r.data, sizeof(r.data));
    lerLinha("Horas trabalhadas (ex: 8.5): ", buffer, sizeof(buffer));
    r.horas = (float)atof(buffer);
    strcpy(r.status, "Concluído");

    FILE *file = fopen(RECORDS_FILE, "ab");
    if (!file) {
        printf("Erro ao abrir arquivo de registros.\n");
        return;
    }
    fwrite(&r, sizeof(Registro), 1, file);
    fclose(file);
    printf("Registro salvo com sucesso!\n");
}

void listarRegistros() {
    FILE *file = fopen(RECORDS_FILE, "rb");
    if (!file) {
        printf("Nenhum registro de horas encontrado.\n");
        return;
    }
    Registro r;
    printf("\n--- REGISTROS DE HORAS ---\n");
    printf("%-10s | %-12s | %-6s | %-10s\n", "Matrícula", "Data", "Horas", "Status");
    printf("-----------------------------------------------\n");
    while (fread(&r, sizeof(Registro), 1, file)) {
        printf("%-10d | %-12s | %-6.2f | %-10s\n", r.matricula, r.data, r.horas, r.status);
    }
    fclose(file);
}

void calcularSalarioFuncionario() {
    char buffer[MAX_INPUT];
    lerLinha("Digite a matrícula do funcionário: ", buffer, sizeof(buffer));
    int matricula = atoi(buffer);
    if (matricula <= 0) {
        printf("Matrícula inválida.\n");
        return;
    }

    FILE *funcFile = fopen(EMPLOYEES_FILE, "rb");
    FILE *regFile = fopen(RECORDS_FILE, "rb");
    if (!funcFile) {
        printf("Nenhum funcionário cadastrado.\n");
        if (regFile) fclose(regFile);
        return;
    }

    Funcionario f;
    int encontrado = 0;
    while (fread(&f, sizeof(Funcionario), 1, funcFile)) {
        if (f.matricula == matricula) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado.\n");
        fclose(funcFile);
        if (regFile) fclose(regFile);
        return;
    }

    float totalHoras = 0.0f;
    if (regFile) {
        Registro r;
        rewind(regFile);
        while (fread(&r, sizeof(Registro), 1, regFile)) {
            if (r.matricula == matricula) totalHoras += r.horas;
        }
        fclose(regFile);
    }

    float totalSalario = totalHoras * f.valorHora;
    printf("\n--- CÁLCULO DE SALÁRIO ---\n");
    printf("Funcionário: %s\n", f.nome);
    printf("Matrícula: %d\n", f.matricula);
    printf("Função: %s\n", f.funcao);
    printf("Valor/Hora: R$ %.2f\n", f.valorHora);
    printf("Total de Horas: %.2f h\n", totalHoras);
    printf("Salário Total: R$ %.2f\n", totalSalario);

    fclose(funcFile);
}

void buscarFuncionarioPorNome() {
    char nomeBusca[50];
    lerLinha("Digite o nome a buscar: ", nomeBusca, sizeof(nomeBusca));
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    if (!file) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    Funcionario f;
    int encontrado = 0;
    printf("\n--- RESULTADO DA BUSCA (nome) ---\n");
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        char nomeLower[50], buscaLower[50];
        for (size_t i = 0; i < sizeof(nomeLower); i++) nomeLower[i] = '\0';
        for (size_t i = 0; i < sizeof(buscaLower); i++) buscaLower[i] = '\0';
        strncpy(nomeLower, f.nome, sizeof(nomeLower)-1);
        strncpy(buscaLower, nomeBusca, sizeof(buscaLower)-1);
        for (int i = 0; nomeLower[i]; i++) nomeLower[i] = (char)tolower(nomeLower[i]);
        for (int i = 0; buscaLower[i]; i++) buscaLower[i] = (char)tolower(buscaLower[i]);

        if (strstr(nomeLower, buscaLower)) {
            printf("Nome: %s | Matrícula: %d | Função: %s | Valor/Hora: R$ %.2f\n", f.nome, f.matricula, f.funcao, f.valorHora);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("Nenhum funcionário encontrado com esse nome.\n");
    fclose(file);
}

void buscarFuncionarioPorMatricula() {
    char buffer[MAX_INPUT];
    lerLinha("Digite a matrícula a buscar: ", buffer, sizeof(buffer));
    int matricula = atoi(buffer);
    if (matricula <= 0) {
        printf("Matrícula inválida.\n");
        return;
    }
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    if (!file) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    Funcionario f;
    int encontrado = 0;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.matricula == matricula) {
            printf("\n--- FUNCIONÁRIO ENCONTRADO ---\n");
            printf("Nome: %s\n", f.nome);
            printf("Matrícula: %d\n", f.matricula);
            printf("Função: %s\n", f.funcao);
            printf("Valor/Hora: R$ %.2f\n", f.valorHora);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("Funcionário não encontrado.\n");
    fclose(file);
}

void editarFuncionario() {
    char buffer[MAX_INPUT];
    lerLinha("Digite a matrícula do funcionário a editar: ", buffer, sizeof(buffer));
    int matricula = atoi(buffer);
    if (matricula <= 0) {
        printf("Matrícula inválida.\n");
        return;
    }
    FILE *file = fopen(EMPLOYEES_FILE, "rb+");
    if (!file) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    Funcionario f;
    int encontrado = 0;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.matricula == matricula) {
            encontrado = 1;
            printf("\nDados atuais:\n");
            printf("Nome: %s\n", f.nome);
            printf("Função: %s\n", f.funcao);
            printf("Valor/Hora: R$ %.2f\n", f.valorHora);

            lerLinha("Novo nome (enter para manter): ", buffer, sizeof(buffer));
            if (strlen(buffer) > 0) strncpy(f.nome, buffer, sizeof(f.nome)-1);

            lerLinha("Nova função/cargo (enter para manter): ", buffer, sizeof(buffer));
            if (strlen(buffer) > 0) strncpy(f.funcao, buffer, sizeof(f.funcao)-1);

            lerLinha("Novo valor da hora (enter para manter): ", buffer, sizeof(buffer));
            if (strlen(buffer) > 0) f.valorHora = (float)atof(buffer);

            fseek(file, - (long)sizeof(Funcionario), SEEK_CUR);
            fwrite(&f, sizeof(Funcionario), 1, file);

            printf("Dados atualizados com sucesso!\n");
            break;
        }
    }
    if (!encontrado) printf("Funcionário não encontrado.\n");
    fclose(file);
}

void apagarFuncionario() {
    char buffer[MAX_INPUT];
    lerLinha("Digite a matrícula do funcionário a apagar: ", buffer, sizeof(buffer));
    int matricula = atoi(buffer);
    if (matricula <= 0) {
        printf("Matrícula inválida.\n");
        return;
    }
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    FILE *temp = fopen(TEMP_FILE, "wb");
    if (!file || !temp) {
        printf("Erro ao abrir arquivos.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }
    Funcionario f;
    int encontrado = 0;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.matricula != matricula) fwrite(&f, sizeof(Funcionario), 1, temp);
        else encontrado = 1;
    }
    fclose(file);
    fclose(temp);
    remove(EMPLOYEES_FILE);
    rename(TEMP_FILE, EMPLOYEES_FILE);

    FILE *rfile = fopen(RECORDS_FILE, "rb");
    FILE *rtemp = fopen("reg_temp.dat", "wb");
    if (rfile && rtemp) {
        Registro reg;
        while (fread(&reg, sizeof(Registro), 1, rfile)) {
            if (reg.matricula != matricula) fwrite(&reg, sizeof(Registro), 1, rtemp);
        }
        fclose(rfile);
        fclose(rtemp);
        remove(RECORDS_FILE);
        rename("reg_temp.dat", RECORDS_FILE);
    }

    if (encontrado) printf("Funcionário removido com sucesso (e registros relacionados).\n");
    else printf("Funcionário não encontrado.\n");
}

void exportarCSV() {
    FILE *file = fopen(EMPLOYEES_FILE, "rb");
    if (!file) {
        printf("Nenhum funcionário cadastrado.\n");
        return;
    }
    FILE *csv = fopen("funcionarios_export.csv", "w");
    if (!csv) {
        printf("Erro ao criar CSV.\n");
        fclose(file);
        return;
    }
    fprintf(csv, "Nome,Matrícula,Função,ValorHora\n");
    Funcionario f;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        fprintf(csv, "\"%s\",%d,\"%s\",%.2f\n", f.nome, f.matricula, f.funcao, f.valorHora);
    }
    fclose(file);
    fclose(csv);
    printf("Exportado para funcionarios_export.csv\n");
}

int main() {
    if (!autenticar()) return 0;

    int opc = -1;
    char escolha[MAX_INPUT];
    while (1) {
        exibirMenu();
        if (!fgets(escolha, sizeof(escolha), stdin)) break;
        opc = atoi(escolha);
        switch (opc) {
            case 1: cadastrarFuncionario(); break;
            case 2: listarFuncionarios(); break;
            case 3: registrarHoras(); break;
            case 4: listarRegistros(); break;
            case 5: calcularSalarioFuncionario(); break;
            case 6: buscarFuncionarioPorNome(); break;
            case 7: buscarFuncionarioPorMatricula(); break;
            case 8: editarFuncionario(); break;
            case 9: apagarFuncionario(); break;
            case 10: exportarCSV(); break;
            case 11: {
                char usuario[50], senha[50];
                lerLinha("Novo usuário admin: ", usuario, sizeof(usuario));
                lerLinha("Nova senha admin: ", senha, sizeof(senha));
                salvarCredenciais(usuario, senha);
                printf("Credenciais atualizadas.\n");
                break;
            }
            case 0:
                printf("Saindo... Até mais!\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
        printf("\n");
    }
    return 0;
}