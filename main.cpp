#include <iostream>
#include <locale>
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
    string unidadeMedida;
    string dataCriacao;
    string horaCriacao;

    Produto() {} // construtor vazio

    Produto(string nome, int quantidade, float valor, string unidadeMedida) {
        this->nome = nome;
        this->quantidade = quantidade;
        this->valor = valor;
        this->unidadeMedida = unidadeMedida;

        // obt�m o tempo atual em segundos desde o "Epoch" e converte para uma struct tm
        time_t tempo = time(nullptr);
        tm* tempoAtual = localtime(&tempo);

        // usa ostringstream para capturar a data e a hora formatadas
        ostringstream ossData, ossHora;
        ossData << put_time(tempoAtual, "%d/%m/%Y");
        ossHora << put_time(tempoAtual, "%H:%M");

        // armazena as strings formatadas nas vari�veis correspondentes
        dataCriacao = ossData.str();
        horaCriacao = ossHora.str();
    }
};

vector<Produto> produtos;

void CarregarProdutos() {
    ifstream leitor("dados.csv");

    // l� as informa��es de todos os produtos no arquivo e armazena na lista
    while(leitor.good()) {
        Produto tempProduto;
        getline(leitor, tempProduto.nome, ',');
        leitor >> tempProduto.quantidade; // m�todo diferente por se tratar de um int
        leitor.ignore(); // pula a v�rgula 
        leitor >> tempProduto.valor; // m�todo diferente por se tratar de um float
        leitor.ignore(); // pula a v�rgula
        getline(leitor, tempProduto.unidadeMedida, ',');
        getline(leitor, tempProduto.dataCriacao, ',');
        getline(leitor, tempProduto.horaCriacao, '\n');
        
        produtos.push_back(tempProduto); // adiciona na lista de produtos
    }

    leitor.close();

}

void AtualizarArquivo() {
    ofstream escritor("dados.csv");

    // percorre toda a lista de produtos e os reescreve no arquivo de dados
    for(size_t i = 0; i < produtos.size(); i++) {
        escritor << produtos[i].nome << "," << produtos[i].quantidade << "," <<  produtos[i].valor << "," << produtos[i].unidadeMedida << "," << produtos[i].dataCriacao << "," << produtos[i].horaCriacao << "\n";
    }
}

void LimparConsole() {
    system("cls");
}

void MenuPrincipal(); // declara��o antecipada devido ao escopo

void VoltarMenu() {
    cout << "\nPressione qualquer tecla para voltar ao menu inicial...";
    system("pause > nul");
    LimparConsole();
    MenuPrincipal();
}

void CadastrarProduto() {
    LimparConsole();
    string nomeDigitado;
    int quantidadeDigitada;
    float valorDigitado;
    string medidaDigitada;

    cout << "--- Cadastro de produtos ---\n";
    cout << "\nDigite o nome do produto: ";
    cin >> nomeDigitado;
    for (auto &c : nomeDigitado) c = toupper(c);
    for(size_t i = 0; i < produtos.size(); i++) {
        if(nomeDigitado == produtos[i].nome){
            cout << "\nProduto \"" << produtos[i].nome <<"\" j� cadastrado.\n";
            // Valida��o da quantidade recebida (maior que 0)
            do {
                cout << "Digite a quantidade recebida (em " << produtos[i].unidadeMedida << "): ";
                cin >> quantidadeDigitada;
                if (quantidadeDigitada <= 0) {
                    cout << "Quantidade inv�lida! Deve ser maior que 0." << endl;
                }
            } while (quantidadeDigitada <= 0);

            // Valida��o do valor de venda (maior que 0)
            do {
                cout << "Digite o valor de venda (por " << produtos[i].unidadeMedida << "): "; 
                cin >> valorDigitado;
                if (valorDigitado <= 0) {
                    cout << "Valor inv�lido! Deve ser maior que 0." << endl;
                }
            } while (valorDigitado <= 0);
            AtualizarArquivo();
            cout << "\nProduto atualizado com sucesso!\n";
            VoltarMenu();
        }
    }
    do {
        cout << "Digite a unidade de medida (KG ou UN - Unit�rio): ";
        cin >> medidaDigitada;
        for (auto &c : medidaDigitada) c = toupper(c); // Converte para mai�sculo
        if (medidaDigitada != "KG" && medidaDigitada != "UN") {
            cout << "Unidade inv�lida! Digite apenas KG ou UN." << endl;
        }
    } while (medidaDigitada != "KG" && medidaDigitada != "UN");

    // Valida��o da quantidade recebida (maior que 0)
    do {
        cout << "Digite a quantidade recebida (em " << medidaDigitada << "): ";
        cin >> quantidadeDigitada;
        if (quantidadeDigitada <= 0) {
            cout << "Quantidade inv�lida! Deve ser maior que 0." << endl;
        }
    } while (quantidadeDigitada <= 0);

    // Valida��o do valor de venda (maior que 0)
    do {
        cout << "Digite o valor de venda (por " << medidaDigitada << "): "; 
        cin >> valorDigitado;
        if (valorDigitado <= 0) {
            cout << "Valor inv�lido! Deve ser maior que 0." << endl;
        }
    } while (valorDigitado <= 0);

    Produto novoProduto(nomeDigitado, quantidadeDigitada, valorDigitado, medidaDigitada);
    produtos.push_back(novoProduto);
    AtualizarArquivo();
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
    cout << left << setw(20) << "Nome" << setw(10) << "Qtd." << setw(10) << "Pre�o" << setw(10) << "Medida" <<  setw(20) << "Data/Hora Cria��o" << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    for (int i = 0; i < produtos.size() ; ++i) {
        cout << left << setw(20) << produtos[i].nome << setw(10) << produtos[i].quantidade << setw(10) << produtos[i].valor << setw(10) << produtos[i].unidadeMedida <<  setw(20) << produtos[i].dataCriacao + " �s " + produtos[i].horaCriacao << endl;
    }
    VoltarMenu();
}

void EncerrarExpediente() {
    LimparConsole();
    cout << "Teste";
}

void MenuPrincipal() {
    int escolha;

    cout << "--- Sistema de Supermercados ---\n";
    cout << "\n[1] Cadastrar Produto";
    cout << "\n[2] Vender Produto";
    cout << "\n[3] Listar Produtos";
    cout << "\n[4] Encerrar expediente\n";
    cout << "\nDigite a op��o desejada: ";

    do {
        if (!(cin >> escolha)) { // verifica se a entrada for inv�lida
            cin.clear(); // limpa o erro
            cin.ignore(1000, '\n'); // ignora a entrada inv�lida
            cout << "Op��o inv�lida digite novamente: ";
            continue; // continua o loop sem parar
        }

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
                cout << "Op��o inv�lida digite novamente: ";
                break;
        }
    } while (true); // continua indefinidamente at� que seja encerrado pela fun��o
}

int main() {
    setlocale(LC_ALL,"");
    CarregarProdutos();
    MenuPrincipal();
}