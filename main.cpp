#include <iostream>
#include <locale.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct Produto {
    string nome;
    int quantidade;
    float valor;
    string dataCriacao;
    string horaCriacao;

    Produto(string nome, int quantidade, float valor) {
        this->nome = nome;
        this->quantidade = quantidade;
        this->valor = valor;

        // Obtém o tempo atual em segundos desde o "Epoch" e converte para uma struct tm
        time_t tempo = time(nullptr);
        tm* tempoAtual = localtime(&tempo);

        // Usa ostringstream para capturar a data e a hora formatadas
        ostringstream ossData, ossHora;
        ossData << put_time(tempoAtual, "%d/%m/%Y");
        ossHora << put_time(tempoAtual, "%H:%M");

        // Armazena as strings formatadas nas variáveis correspondentes
        dataCriacao = ossData.str();
        horaCriacao = ossHora.str();
    }

};

vector<Produto> produtos;

void LimparConsole() {
    system("cls");
}

void MenuPrincipal(); // Declaração antecipada devido ao escopo

void VoltarMenu() {
    cout << "\nPressione qualquer tecla para voltar ao menu inicial...";
    system("pause > nul");
    MenuPrincipal();
}

void CadastrarProduto() {
    LimparConsole();
    string nomeDigitado;
    int quantidadeDigitada;
    float valorDigitado;

    cout << "--- Cadastro de produtos ---\n";
    cout << "\nDigite o nome do produto: ";
    cin >> nomeDigitado;
    for(size_t i = 0; i < produtos.size(); i++) {
        if(nomeDigitado == produtos[i].nome){
            cout << "\nProduto \"" << produtos[i].nome <<"\" já cadastrado.\n";
            cout << "Digite a quantidade recebida: ";
            cin >> quantidadeDigitada;
            produtos[i].quantidade += quantidadeDigitada;
            cout << "Digite o valor de venda: ";
            cin >> valorDigitado;
            produtos[i].valor = valorDigitado;
            cout << "\nProduto cadastrado com sucesso!\n";
            VoltarMenu();
        }
    }
    cout << "Digite a quantidade recebida: ";
    cin >> quantidadeDigitada;
    cout << "Digite o valor de venda: ";
    cin >> valorDigitado;
    Produto novoProduto(nomeDigitado, quantidadeDigitada, valorDigitado);
    produtos.push_back(novoProduto);
    cout << "\nProduto cadastrado com sucesso!\n";
    VoltarMenu();
}

void VenderProduto() {
    LimparConsole();
    cout << "Teste";
}

void ListarProdutos() {
    LimparConsole();
    cout << "--- Listar Produtos ---\n";
    for (int i = 0; i < produtos.size(); i++) {
        cout << "Nome: " << produtos[i].nome 
             << ", Quantidade: " << produtos[i].quantidade 
             << ", Valor: R$" << produtos[i].valor 
             << ", Data de Criação: " << produtos[i].dataCriacao 
             << ", Hora de Criação: " << produtos[i].horaCriacao 
             << endl;
    }
    VoltarMenu();
}

void EncerrarExpediente() {
    LimparConsole();
    cout << "Teste";
}

void MenuPrincipal() {
    LimparConsole();
    int escolha;
    bool escolhaValida = true;
    cout << "--- Sistema de Supermercados ---\n";
    cout << "\n[1] Cadastrar Produto";
    cout << "\n[2] Vender Produto";
    cout << "\n[3] Listar Produtos";
    cout << "\n[4] Encerrar expediente\n";
    cout << "\nDigite a opção desejada: ";

    do {
        cin >> escolha;
        switch (escolha) {
            case 1:
                CadastrarProduto();
                break;
            case 2:
                VenderProduto();
                break;
            case 3:
                ListarProdutos();
                break;
            case 4:
                EncerrarExpediente();
                break;
            default:
                cout << "Opção inválida digite novamente: ";
                escolhaValida = false;
                break;
        }
    } while (!escolhaValida);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    MenuPrincipal();
}