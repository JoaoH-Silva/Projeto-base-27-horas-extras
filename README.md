# Projeto-base-27-horas-extras
README - Sistema de Controle de Funcionários e Horas Extras
---------------
Este projeto em C implementa um sistema completo para gerenciar **funcionários** e seus **registros de horas trabalhadas**, permitindo calcular salários com base em horas normais e horas extras, além de funcionalidades administrativas como login, edição e exportação de dados.


Principais Recursos
-------------------
- 🔐 **Login de administrador** com credenciais armazenadas em `admin.txt`.
- 👤 **Cadastro, listagem, edição e remoção de funcionários**.
- ⏱️ **Registro de horas trabalhadas** (normais e extras).
- 🔎 **Busca por nome e matrícula**.
- 💰 **Cálculo automático de salário** com base nas horas registradas.
- 📄 **Exportação dos funcionários para CSV**.
- 🗑️ Remoção automática de registros quando um funcionário é apagado.
- 📁 Armazenamento persistente em arquivos `.dat` (binários).


Arquivos Utilizados
-------------------
- `admin.txt` → Armazena usuário e senha do administrador.
- `funcionarios.dat` → Banco de dados binário de funcionários.
- `registros.dat` → Banco de dados binário de registros de horas.
- `funcionarios_export.csv` → Arquivo gerado na exportação dos dados.


Como Usar
---------
### Compilar
```
gcc sistema.c -o sistema
```
### Executar
```
./sistema
```
(Sistema operacional Windows: `sistema.exe`)


### Primeira Execução
Se o arquivo `admin.txt` não existir, o programa irá solicitar que você cadastre:
1. Um nome de usuário
2. Uma senha


Esses dados serão necessários para acessar o sistema nas próximas execuções.


Estrutura do admin.txt
----------------------
Linha 1: usuário (ex.: `admin`)
Linha 2: senha (ex.: `1234`)


Funcionalidades do Menu
-----------------------
1. Cadastrar funcionário
2. Listar funcionários
3. Buscar funcionário por nome
4. Registrar horas trabalhadas
5. Listar registros de horas
6. Consultar salário de um funcionário
7. **Buscar por matrícula**
8. Apagar funcionário
9. Limpar todos registros
10. Exportar funcionários em CSV
11. Alterar login do administrador
0. Sair


Regras de Negócio
-----------------
- Matrículas duplicadas não são permitidas.
- Ao excluir um funcionário, todos seus registros são removidos.
- O salário é calculado considerando:
- Horas normais → valorHora
- Horas extras → valorHora * 1.5


Melhorias Possíveis
-------------------
- Validação de formato de data (DD/MM/AAAA).
- Suporte a múltiplos administradores.
- Versão com interface gráfica (GTK/WPF).
- Banco SQLite no lugar de arquivos binários.


Licença
-------
Este projeto pode ser utilizado e modificado livremente. Se publicar no GitHub, considere dar os créditos 🙂


*/
