#include <iostream>
#include <locale>
#include <locale.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Produto {
    string nome;
    float quantidade;
    float valor;
    string unidadeMedida;
    string dataCriacao;
    string horaCriacao;

    Produto() {} // construtor vazio

    Produto(string nome, float quantidade, float valor, string unidadeMedida) {
        this->nome = nome;
        this->quantidade = quantidade;
        this->valor = valor;
        this->unidadeMedida = unidadeMedida;

        // obtem o tempo atual em segundos desde o "Epoch" e converte para uma struct tm
        time_t tempo = time(nullptr);
        tm* tempoAtual = localtime(&tempo);

        // usa ostringstream para capturar a data e a hora formatadas
        ostringstream ossData, ossHora;
        ossData << put_time(tempoAtual, "%d/%m/%Y");
        ossHora << put_time(tempoAtual, "%H:%M");

        // armazena as strings formatadas nas variaveis correspondentes
        dataCriacao = ossData.str();
        horaCriacao = ossHora.str();
    }
};

vector<Produto> produtos;

void CarregarProdutos() {
    ifstream leitor("dados.csv");

    // lê as informacões de todos os produtos no arquivo e armazena na lista
    while(leitor.good()) {
        Produto tempProduto;
        string tempQuantidade, tempValor;

        // lê cada linha e verifica se nao esta vazia
        if (!getline(leitor, tempProduto.nome, ';')) break;
        if (!getline(leitor, tempQuantidade, ';')) break;
        if (!getline(leitor, tempValor, ';')) break;
        if (!getline(leitor, tempProduto.unidadeMedida, ';')) break;
        if (!getline(leitor, tempProduto.dataCriacao, ';')) break;
        if (!getline(leitor, tempProduto.horaCriacao, '\n')) break;

        // verifica se as strings sao validas antes de converter
        try {
            tempProduto.quantidade = stof(tempQuantidade);
            tempProduto.valor = stof(tempValor);
        } catch (const invalid_argument& e) {
            cout << "Erro de conversao: " << e.what() << endl;
            continue;  // pular para o proximo produto
        } catch (const out_of_range& e) {
            cout << "Valor fora do intervalo: " << e.what() << endl;
            continue;  // pular para o proximo produto
        }

        produtos.push_back(tempProduto); // adiciona na lista de produtos

    }

    leitor.close();

}

void AtualizarArquivo() {
    ofstream escritor("dados.csv");

    // percorre toda a lista de produtos e os reescreve no arquivo de dados
    for(size_t i = 0; i < produtos.size(); i++) {
        escritor << produtos[i].nome << ";" << produtos[i].quantidade << ";" <<  produtos[i].valor << ";" << produtos[i].unidadeMedida << ";" << produtos[i].dataCriacao << ";" << produtos[i].horaCriacao << "\n";
    }
}

void LimparConsole() {
    system("cls");
}

void MenuPrincipal(); // declaracao antecipada devido ao escopo

void VoltarMenu() {
    cout << "\nPressione qualquer tecla para voltar ao menu inicial...";
    system("pause > nul");
    LimparConsole();
    MenuPrincipal();
}

void VerificaEntrada() {
    if (cin.fail()) { // Se a entrada for inválida
        cout << "Entrada inválida! Por favor, digite um número válido." << endl;
        cin.clear(); // Limpa o estado de erro
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora a entrada errada
        //continue; // Retorna ao início do laço
    }
}

void CadastrarProduto() {
    LimparConsole();
    string nomeDigitado;
    float quantidadeDigitada;
    float valorDigitado;
    string medidaDigitada;

    cout << "--- Cadastro de produtos ---\n";
    cout << "\nDigite o nome do produto: ";
    cin >> nomeDigitado;
    for (auto &c : nomeDigitado) c = toupper(c); // transforma todas as letras em mausculo
    for(size_t i = 0; i < produtos.size(); i++) {
        if(nomeDigitado == produtos[i].nome){
            cout << "\nProduto \"" << produtos[i].nome <<"\" ja cadastrado.\n";
            // Validacao da quantidade recebida (maior que 0)
            do {
                cout << "Digite a quantidade recebida (em " << produtos[i].unidadeMedida << "): ";
                cin >> quantidadeDigitada;
                
                // verifica se digitou string
                if (cin.fail()) {
                    cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
                    continue; // retorna ao inicio do laco
                }
                
                if (quantidadeDigitada <= 0) {
                    cout << "Quantidade invalida! Deve ser maior que 0." << endl;
                }
                produtos[i].quantidade += quantidadeDigitada;
            } while (quantidadeDigitada <= 0);

            // Validacao do valor de venda (maior que 0)
            do {
                cout << "Digite o valor de venda (por " << produtos[i].unidadeMedida << "): "; 
                cin >> valorDigitado;

                // verifica se digitou string
                if (cin.fail()) {
                    cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
                    continue; // retorna ao inicio do laco
                }

                if (valorDigitado <= 0) {
                    cout << "Valor invalido! Deve ser maior que 0." << endl;
                }
                produtos[i].valor = valorDigitado;
            } while (valorDigitado <= 0);
            AtualizarArquivo();
            cout << "\nProduto atualizado com sucesso!\n";
            VoltarMenu();
        }
    }
    do {
        cout << "Digite a unidade de medida (KG ou UN - Unitario): ";
        cin >> medidaDigitada;
        for (auto &c : medidaDigitada) c = toupper(c); // Converte para maiusculo
        if (medidaDigitada != "KG" && medidaDigitada != "UN") {
            cout << "Unidade invalida! Digite apenas KG ou UN." << endl;
        }
    } while (medidaDigitada != "KG" && medidaDigitada != "UN");

    // Validacao da quantidade recebida (maior que 0)
    do {
        cout << "Digite a quantidade recebida (em " << medidaDigitada << "): ";
        cin >> quantidadeDigitada;

        // verifica se digitou string
        if (cin.fail()) {
            cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
            continue; // retorna ao inicio do laco
        }

        if (quantidadeDigitada <= 0) {
            cout << "Quantidade invalida! Deve ser maior que 0." << endl;
        }
    } while (quantidadeDigitada <= 0);

    // Validacao do valor de venda (maior que 0)
    do {
        cout << "Digite o valor de venda (por " << medidaDigitada << "): "; 
        cin >> valorDigitado;

        // verifica se digitou string
        if (cin.fail()) {
            cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
            continue; // retorna ao inicio do laco
        }

        if (valorDigitado <= 0) {
            cout << "Valor invalido! Deve ser maior que 0." << endl;
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
    string nomeDigitado;
    int opcaoEscolha;
    int numParcelas;
    float quantidadeDigitada;
    float valorTotal = 0;
    float valorParcelas;

    cout << "--- Venda de produtos ---\n";
    do {
        cout << "\nDigite o nome do produto: ";
        cin >> nomeDigitado;
        for (auto &c : nomeDigitado) c = toupper(c); // transforma todas as letras em maiusculo
        for (size_t i = 0; i < produtos.size(); i++) {
            if (nomeDigitado == produtos[i].nome) {
                string textoParenteses;
                produtos[i].unidadeMedida == "UN" ? textoParenteses = "(por unidade): " : textoParenteses = "(em quilogramas): ";
                do {
                    cout << "Digite a quantidade " << textoParenteses; 
                    cin >> quantidadeDigitada;

                    // verifica se digitou string
                    if (cin.fail()) {
                        cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
                        continue; // retorna ao inicio do laco
                    }

                    if (quantidadeDigitada <= 0) {
                    cout << "Valor invalido! Deve ser maior que 0." << endl;
                    }
                    else if (quantidadeDigitada > produtos[i].quantidade) {
                        cout << "Quantidade em estoque insuficiente." << endl;
                        cout << "Saldo do produto: " << produtos[i].quantidade << endl; 
                    }
                    produtos[i].quantidade -= quantidadeDigitada;
                    valorTotal = valorTotal + (quantidadeDigitada * produtos[i].valor);
                } while (quantidadeDigitada <= 0 || quantidadeDigitada > produtos[i].quantidade);
            }
        }
        cout << "\n[1] Finalizar venda";
        cout << "\n[2] Continuar vendendo\n";
        do {
            cout << "\nDigite a opcao desejada: ";
            cin >> opcaoEscolha;

            // verifica se digitou string
            if (cin.fail()) {
                cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
                continue; // retorna ao inicio do laco
            }

            if (opcaoEscolha != 1 && opcaoEscolha != 2) {
                cout << "Entrada invalida! Digite apenas 1 ou 2." << endl;
            }

        } while (opcaoEscolha != 1 && opcaoEscolha != 2);

        switch (opcaoEscolha) {
            case 1:
                cout << "Valor total da venda: R$ " << valorTotal << endl;
                cout << "\nA vista (1 parcela) \t5% de desconto";
                cout << "\nEm ate 3x \t\tsem juros";
                cout << "\nEm ate 12x \t\t10% de juros\n";
                do {
                    cout << "\nDigite a quatidade de parcelas: ";
                    cin >> numParcelas;

                    // verifica se digitou string
                    if (cin.fail()) {
                        cout << "Entrada invalida! Por favor, digite um numero valido." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignora a entrada errada
                        continue; // retorna ao inicio do laco
                    }

                    if (numParcelas == 1) {
                        cout << "Valor a pagar: R$ " << valorTotal - (valorTotal * 0.05);
                        VoltarMenu();
                    }
                    else if (numParcelas > 1 && numParcelas <= 12) {
                        if (numParcelas == 2 || numParcelas == 3) {
                            valorParcelas = valorTotal / numParcelas;
                            cout << "\nParcelas a pagar: \n";
                            for (int i = 1; i <= numParcelas; i++) {
                                cout << i << "ª Parcela: R$" << valorParcelas << " - Vencimento: xx/xx/xxxx\n";
                            }
                            VoltarMenu();
                        }
                        if (numParcelas > 3 || numParcelas < 13) {
                            valorTotal = valorTotal * 1.1;
                            valorParcelas = valorTotal / numParcelas;
                            cout << "\nParcelas a pagar: ";
                            for (int i = 1; i <= numParcelas; i++) {
                                cout << i << "ª Parcela: R$" << valorParcelas << " - Vencimento: xx/xx/xxxx\n";
                            }
                            VoltarMenu();
                        }
                    }
                    else {
                        cout << "Numero de parcelas invalido. Tente novamente.";
                    }
                } while (numParcelas == 0 && numParcelas > 12);
                break;
            case 2:
                break;
        }
    } while (opcaoEscolha != 1);
}

void ListarProdutos() {
    LimparConsole();
    cout << "--- Listar Produtos ---\n" << endl;
    cout << left << setw(20) << "Nome" << setw(10) << "Qtd." << setw(10) << "Medida" << setw(10) << "Preco" <<  setw(20) << "Data/Hora Criacao" << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    for (int i = 0; i < produtos.size() ; ++i) {
        cout << left << setw(20) << produtos[i].nome << setw(10) << produtos[i].quantidade << setw(10) << produtos[i].unidadeMedida << setw(10) << produtos[i].valor <<  setw(20) << produtos[i].dataCriacao + " as " + produtos[i].horaCriacao << endl;
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
    cout << "\nDigite a opcao desejada: ";

    do {
        if (!(cin >> escolha)) { // verifica se a entrada for invalida
            cin.clear(); // limpa o erro
            cin.ignore(1000, '\n'); // ignora a entrada invalida
            cout << "Opcao invalida digite novamente: ";
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
                cout << "Opcao invalida digite novamente: ";
                break;
        }
    } while (true); // continua indefinidamente ate que seja encerrado pela funcao
}

int main() {
    CarregarProdutos();
    MenuPrincipal();
}