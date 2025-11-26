# Sistema Digital de Controle de Horas Extras — Base27

Este projeto faz parte do Projeto Integrador em parceria com a **startup Base27**, cujo objetivo é desenvolver um sistema digital para substituir o processo manual de controle de horas extras utilizado pelas empresas.  
Esta versão é um **protótipo funcional em Linguagem C**, focado na lógica interna do sistema.

## Funcionalidades do Sistema
- Login de administrador (dados armazenados em `admin.txt`)
- Cadastro, listagem, busca e remoção de funcionários
- Registro de horas normais e extras
- Cálculo automático de salário (horas extras = valorHora * 1.5)
- Busca por nome e matrícula
- Exportação dos funcionários para CSV
- Armazenamento persistente em arquivos binários (`funcionarios.dat` e `registros.dat`)
- Remoção automática dos registros ao excluir um funcionário

## Arquivos do Projeto
- `admin.txt` — credenciais do administrador  
- `funcionarios.dat` — banco de dados de funcionários  
- `registros.dat` — banco de dados de registros de horas  
- `funcionarios_export.csv` — exportação dos dados  
- `sistema.c` — código principal do projeto  
## Autor
**João Henrique Alves Silva**
**Matrícula: 202305582**

**Murilo Rezende Coutinho**
**Matrícula: 202306296**
*/
