# SQL Plus C - Sistema de Gerenciamento de Unidades de Biblioteca

DBManager é um sistema desenvolvido em **C** que permite gerenciar unidades de atendimento de uma biblioteca, com conexão a um banco de dados **PostgreSQL**. O programa possibilita incluir, consultar, alterar e excluir informações de unidades de atendimento, garantindo controle eficiente das bibliotecas.

---

## Funcionalidades

- Inserir novas unidades de atendimento
- Consultar unidades cadastradas
- Alterar informações de unidades existentes
- Excluir unidades
- Interface simples via menu em terminal

---

## Guia de Uso Completo

Para mais detalhes sobre o funcionamento e exemplos de uso, consulte a apostila:

[Guia de Uso](https://drive.google.com/file/d/1zAqflQnOangghDUtBFIlkzl7MkdQtcIH/view)

---

## Pré-requisitos

Para executar o projeto, você precisa ter instalado:

1. **C Compiler** (GCC ou equivalente)
2. **PostgreSQL** instalado e em execução
3. Biblioteca **libpq-dev** (para conexão C com PostgreSQL)

Exemplo de instalação no Linux (Debian/Ubuntu):

```bash
sudo apt update
sudo apt install build-essential libpq-dev postgresql postgresql-contrib
