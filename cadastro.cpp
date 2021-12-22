#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <map>

class Account
{
protected:
    int idCliente;
    float balance;
    std::string clienteId;
    std::string tipoConta; //poupança ou corrente
public:
    Account(int id, std::string clienteId)
    {
        this->idCliente = id;
        this->clienteId = clienteId;
    }

    virtual void monthlyAccount() = 0;

    void withdraw(float value)
    {
        if (value > balance)
        {
            std::cout << "Saldo insuficiente" << std::endl;
        }
        else
        {
            balance -= value;
        }
    }

    void deposit(float value)
    {
        balance += value;
    }

    void transfer(float value, Account &account)
    {
        if (value > balance)
        {
            std::cout << "Saldo insuficiente" << std::endl;
        }
        else
        {
            balance -= value;
            account.deposit(value);
        }
    }

    std::string toString()
    {
        return "Cliente: " + clienteId + " Conta: " + tipoConta + " Saldo: " + std::to_string(balance);
    }

    int getIdCliente()
    {
        return idCliente;
    }

    float getBalance()
    {
        return balance;
    }

    std::string getClienteId()
    {
        return clienteId;
    }

    std::string getTipoConta()
    {
        return tipoConta;
    }
};

class CheckingAccount : public Account
{
public:
    CheckingAccount(int id, std::string clienteId) : Account(id, clienteId)
    {
        tipoConta = "Corrente";
    }

    void monthlyAccount()
    {
        balance -= 20;
    }
};

class SavingsAccount : public Account
{
public:
    SavingsAccount(int id, std::string clienteId) : Account(id, clienteId)
    {
        tipoConta = "Poupança";
        this->idCliente = id;
        this->clienteId = clienteId;
        balance = 0;
    }

    void monthlyAccount()
    {
        balance += balance * 0.01;
    }
};

class Client
{
private:
    std::string clientId;
    std::list<std::shared_ptr<Account>> contas;

public:
    Client(std::string clientId)
    {
        this->clientId = clientId;
    }

    void addAccount(std::shared_ptr<Account> account)
    {
        contas.push_back(account);
    }

    std::string toString()
    {
        std::string str = "Cliente: " + clientId + " Contas: ";
        for (auto it = contas.begin(); it != contas.end(); it++)
        {
            str += (*it)->toString() + " ";
        }
        return str;
    }

    std::string getClientId()
    {
        return clientId;
    }

    void setClientId(std::string clientId)
    {
        this->clientId = clientId;
    }

    std::list<std::shared_ptr<Account>> getContas()
    {
        return contas;
    }

    void setContas(std::list<std::shared_ptr<Account>> contas)
    {
        this->contas = contas;
    }
};

class Bank
{
private:
    std::map<int, std::shared_ptr<Client>> clientes;
    std::map<int, std::shared_ptr<Account>> contas;
    int nextAccountId = 0;
    std::shared_ptr<Account> getAccount(int id);

public:
    Bank();
    void addClient(std::string clientId)
    {
        clientes[nextAccountId] = std::make_shared<Client>(clientId);
        nextAccountId++;
    }

    void withdraw(int idConta, float value)
    {
        getAccount(idConta)->withdraw(value);
    }

    void deposit(int idConta, float value)
    {
        getAccount(idConta)->deposit(value);
    }

    void transfer(int idContaOrigem, int idContaDestino, float value)
    {
        getAccount(idContaOrigem)->transfer(value, *getAccount(idContaDestino));
    }

    void monthlyAccount();

    std::string toString()
    {
        std::string str = "";
        for (auto cliente : clientes)
        {
            str += cliente.second->toString();
        }
        return str;
    }
};

void help()
{
    std::cout << "Comandos disponíveis:" << std::endl;
    std::cout << "help - mostra esta tela" << std::endl;
    std::cout << "addClient - adiciona um cliente" << std::endl;
    std::cout << "deposit - deposita um valor em uma conta" << std::endl;
    std::cout << "withdraw - retira um valor de uma conta" << std::endl;
    std::cout << "transfer - transfere um valor de uma conta para outra" << std::endl;
    std::cout << "monthlyAccount - atualiza as contas" << std::endl;
    std::cout << "exit - sai do programa" << std::endl;
}

int main()
{
    Bank b;
    std::string command;
    while (true)
    {
        std::cout << "$";
        std::cin >> command;
        if (command == "help")
        {
            help();
        }
        else if (command == "addClient")
        {
            std::string clientId;
            std::cin >> clientId;
            b.addClient(clientId);
        }
        else if (command == "deposit")
        {
            int idConta;
            std::cin >> idConta;
            float value;
            std::cin >> value;
            b.deposit(idConta, value);
        }
        else if (command == "withdraw")
        {
            int idConta;
            std::cin >> idConta;
            float value;
            std::cin >> value;
            b.withdraw(idConta, value);
        }
        else if (command == "transfer")
        {
            int idContaOrigem;
            std::cin >> idContaOrigem;
            int idContaDestino;
            std::cin >> idContaDestino;
            float value;
            std::cin >> value;
            b.transfer(idContaOrigem, idContaDestino, value);
        }
        else if (command == "monthlyAccount")
        {
            b.monthlyAccount();
        }
        else if (command == "show")
        {
            std::cout << b.toString() << std::endl;
        }
        else if (command == "exit")
        {
            break;
        }
    }
    return 0;
}
