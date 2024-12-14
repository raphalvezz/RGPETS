#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class Dono;
class Veterinario;
class AppointmentSystem;
class AppointmentSystem;


void showMainMenu();
void showCadastroMenu();
void showLoginMenu();
void handleCadastro(std::map<std::string, std::shared_ptr<Dono>> &Donos,
                    std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios);
void handleLogin(std::map<std::string, std::shared_ptr<Dono>> &Donos,
                 std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios,
                 AppointmentSystem &appointmentSystem);
void cadastrarDono(std::map<std::string, std::shared_ptr<Dono>> &Donos);
void cadastrarVeterinario(std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios);
void menuDono(const std::shared_ptr<Dono> &Dono, AppointmentSystem &appointmentSystem, std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios);
void menuVeterinario(const std::shared_ptr<Veterinario> &vet);


class Pet {
public:
    std::string name;
    std::string species;
    std::string breed;
    int age;

    Pet(const std::string& name, const std::string& species, const std::string& breed, int age)
        : name(name), species(species), breed(breed), age(age) {}

    void displayInfo() const {
        std::cout << "Nome: " << name << ", Espécie: " << species
                  << ", Raça: " << breed << ", Idade: " << age << " anos" << std::endl;
    }
};

// Classe para gerenciar compromissos do veterinário
class VetSchedule {
private:
    std::map<std::string, std::vector<std::string>> schedule;

public:
    void adicionarCompromisso(const std::string& date, const std::string& description) {
        schedule[date].push_back(description);
        std::cout << "Compromisso adicionado para " << date << ": " << description << std::endl;
    }

    void removerCompromisso(const std::string& date, const std::string& description) {
        if (schedule.find(date) == schedule.end()) {
            std::cout << "Nenhum compromisso encontrado para a data: " << date << std::endl;
            return;
        }

        auto& compromissos = schedule[date];
        auto it = std::find(compromissos.begin(), compromissos.end(), description);

        if (it != compromissos.end()) {
            compromissos.erase(it);
            std::cout << "Compromisso removido: " << description << " na data " << date << std::endl;

            if (compromissos.empty()) {
                schedule.erase(date);
            }
        } else {
            std::cout << "Compromisso não encontrado para a data: " << date << std::endl;
        }
    }

    void visualizarCompromissos(const std::string& date) const {
        if (schedule.find(date) == schedule.end()) {
            std::cout << "Nenhum compromisso encontrado para a data: " << date << std::endl;
            return;
        }

        std::cout << "Compromissos para " << date << ":" << std::endl;
        for (const auto& compromisso : schedule.at(date)) {
            std::cout << "- " << compromisso << std::endl;
        }
    }

    void visualizarAgendaCompleta() const {
        if (schedule.empty()) {
            std::cout << "Nenhum compromisso agendado." << std::endl;
            return;
        }

        std::cout << "Agenda completa:" << std::endl;
        for (const auto& entry : schedule) {
            std::cout << "Data: " << entry.first << std::endl;
            for (const auto& compromisso : entry.second) {
                std::cout << "- " << compromisso << std::endl;
            }
        }
    }
};

// Classe para gerenciar notificações
class NotificationSystem {
private:
    std::map<std::string, std::vector<std::string>> notifications;

public:
    void addNotification(const std::string& userId, const std::string& message) {
        notifications[userId].push_back(message);
    }

    void viewNotifications(const std::string& userId) const {
        if (notifications.find(userId) == notifications.end()) {
            std::cout << "Nenhuma notificação para o usuário: " << userId << std::endl;
            return;
        }

        std::cout << "Notificações para " << userId << ":" << std::endl;
        for (const auto& notification : notifications.at(userId)) {
            std::cout << "- " << notification << std::endl;
        }
    }
};


// Classe para donos de pets
class Dono {
public:
    std::string CPF;
    std::string name;
    std::string password;
    std::string cidade;
    std::map<std::string, std::shared_ptr<Pet>> pets; // Map para armazenar os pets do dono

    Dono(const std::string& CPF, const std::string& name, const std::string& password)
        : CPF(CPF), name(name), password(password) {}

    bool verifyPassword(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    void displayInfo() const {
        std::cout << "Dono CPF: " << CPF << ", Nome: " << name << std::endl;
        if (!pets.empty()) {
            std::cout << "Pets cadastrados:" << std::endl;
            for (const auto& pet : pets) {
                pet.second->displayInfo();
            }
        } else {
            std::cout << "Nenhum pet cadastrado." << std::endl;
        }
    }

    void cadastrarPet(const std::string& petId, const std::string& name, const std::string& species,
                      const std::string& breed, int age) {
        pets[petId] = std::make_shared<Pet>(name, species, breed, age);
        std::cout << "Pet " << name << " cadastrado com sucesso!" << std::endl;
    }
    void editarDados(const std::string& novoNome, const std::string& novoTelefone, const std::string& novaCidade) {
        if (!novoNome.empty()) name = novoNome;
        if (!novaCidade.empty()) cidade = novaCidade;
        std::cout << "Dados do dono atualizados com sucesso!" << std::endl;
        displayInfo();
    }
    // Corrigir para aceitar apenas dois parâmetros
    void editarDados(const std::string& novoNome, const std::string& novaCidade) {
        if (!novoNome.empty()) {
            name = novoNome;
        }

        if (!novaCidade.empty()) {
            cidade = novaCidade;
        }

        std::cout << "Dados do dono atualizados com sucesso!" << std::endl;
        displayInfo();
    }

};

// Classe para veterinários
class Veterinario {
public:
    std::string CFMV;
    std::string name;
    std::string specialty;
    std::string password;
    VetSchedule schedule;

    Veterinario(const std::string& CFMV, const std::string& name, const std::string& specialty, const std::string& password)
        : CFMV(CFMV), name(name), specialty(specialty), password(password) {}


    bool verifyPassword(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    void displayInfo() const {
        std::cout << "Veterinário CFMV: " << CFMV << ", Nome: " << name << ", Especialidade: " << specialty << std::endl;
    }
    void editarDados(const std::string& novoNome, const std::string& novaEspecialidade) {
        if (!novoNome.empty()) name = novoNome;
        if (!novaEspecialidade.empty()) specialty = novaEspecialidade;

        std::cout << "Dados do veterinário atualizados com sucesso!" << std::endl;
        displayInfo();
    }
    void visualizarAgenda() const {
        schedule.visualizarAgendaCompleta();
    }

    void adicionarCompromisso(const std::string& date, const std::string& description) {
        schedule.adicionarCompromisso(date, description);
    }
};

// Função para buscar veterinários
void buscarVeterinarios(const std::map<std::string, std::shared_ptr<Veterinario>>& Veterinarios) {
    if (Veterinarios.empty()) {
        std::cout << "Nenhum veterinário cadastrado.\n";
        return;
    }

    std::cout << "Veterinários disponíveis:\n";
    for (const auto& [id, vet] : Veterinarios) {
        vet->displayInfo();
    }
}



// Classe para sistema de agendamentos
class AppointmentSystem {
private:
    VetSchedule schedule;
    NotificationSystem& notificationSystem;

public:
    // Construtor correto com referência ao NotificationSystem
    AppointmentSystem(NotificationSystem& notificationSystem) : notificationSystem(notificationSystem) {}

    // Método para agendar consulta
    void agendarConsulta(const std::string& DonoId, const std::string& CFMV, const std::string& petName,
                         const std::string& date, const std::string& time) {
        std::string description = "Consulta para " + petName + " às " + time;
        schedule.adicionarCompromisso(date, description);
        notificationSystem.addNotification(DonoId, "Consulta agendada para " + date + " às " + time);
        notificationSystem.addNotification(CFMV, "Nova consulta: " + description);
        std::cout << "Consulta agendada com sucesso!\n";
    }

    // Método para aceitar consulta
    void aceitarConsulta(const std::string& CFMV, const std::string& date, const std::string& description) {
        std::cout << "Consulta em " << date << " aceita pelo veterinário " << CFMV << ".\n";
        notificationSystem.addNotification(CFMV, "Consulta confirmada: " + description);
    }
};

// Função para marcar consulta
void marcarConsulta(AppointmentSystem& appointmentSystem, const std::map<std::string, std::shared_ptr<Veterinario>>& Veterinarios, const std::string& DonoId) {
    if (Veterinarios.empty()) {
        std::cout << "Nenhum veterinário disponível para marcar consulta.\n";
        return;
    }

    std::string CFMV, petName, date, time;

    std::cout << "Digite o CFMV do veterinário para a consulta:\n";
    buscarVeterinarios(Veterinarios);
    std::cin >> CFMV;

    if (Veterinarios.find(CFMV) == Veterinarios.end()) {
        std::cout << "Veterinário não encontrado.\n";
        return;
    }

    std::cout << "Digite o nome do seu pet: ";
    std::cin >> petName;
    std::cout << "Digite a data da consulta (formato AAAA-MM-DD): ";
    std::cin >> date;
    std::cout << "Digite o horário da consulta (formato HH:MM): ";
    std::cin >> time;

    appointmentSystem.agendarConsulta(DonoId, CFMV, petName, date, time);
}

void showMainMenu() {
    std::cout << "\n=== Bem-vindo ao Sistema ===\n";
    std::cout << "1. Cadastro\n";
    std::cout << "2. Login\n";
    std::cout << "3. Sair\n";
    std::cout << "Escolha uma opção: ";
}

void showCadastroMenu() {
    std::cout << "\n=== Cadastro ===\n";
    std::cout << "1. Cadastrar Dono de Pet\n";
    std::cout << "2. Cadastrar Veterinário\n";
    std::cout << "3. Voltar ao Menu Principal\n";
    std::cout << "Escolha uma opção: ";
}




// Exibição do menu de login
void showLoginMenu() {
    std::cout << "\n=== Login ===\n";
    std::cout << "1. Login como Dono de Pet\n";
    std::cout << "2. Login como Veterinário\n";
    std::cout << "3. Voltar ao Menu Principal\n";
    std::cout << "Escolha uma opção: ";
}
void handleCadastro(std::map<std::string, std::shared_ptr<Dono>> &Donos,
                    std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios) {
    int cadastroOption;
    do {
        showCadastroMenu();
        std::cin >> cadastroOption;

        switch (cadastroOption) {
            case 1: // Cadastrar Dono
                cadastrarDono(Donos);
                break;
            case 2: // Cadastrar Veterinário
                cadastrarVeterinario(Veterinarios);
                break;
            case 3: // Voltar ao menu principal
                std::cout << "Voltando ao menu principal...\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (cadastroOption != 3);
}

// Lida com o processo de login
void handleLogin(std::map<std::string, std::shared_ptr<Dono>> &Donos,
                 std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios,
                 AppointmentSystem &appointmentSystem) {
    int loginOption;
    do {
        showLoginMenu();
        std::cin >> loginOption;

        switch (loginOption) {
            case 1: {
                std::string CPF;
                std::cout << "Digite seu CPF de Dono: ";
                std::cin >> CPF;

                if (Donos.find(CPF) != Donos.end()) {
                    menuDono(Donos[CPF], appointmentSystem, Veterinarios);
                } else {
                    std::cout << "Dono não encontrado.\n";
                }
                break;
            }
            case 2: {
                std::string CFMV, password;
                std::cout << "Digite seu CFMV de Veterinário: ";
                std::cin >> CFMV;
                std::cout << "Digite sua Senha: ";
                std::cin >> password;

                if (Veterinarios.find(CFMV) != Veterinarios.end() && Veterinarios[CFMV]->verifyPassword(password)) {
                    menuVeterinario(Veterinarios[CFMV]);
                } else {
                    std::cout << "CFMV ou senha incorretos.\n";
                }
                break;
            }
            case 3: {
                std::string CFMV;
                std::cout << "Digite seu CFMV de Veterinário: ";
                std::cin >> CFMV;

                if (Veterinarios.find(CFMV) != Veterinarios.end()) {
                    menuVeterinario(Veterinarios[CFMV]);
                } else {
                    std::cout << "Veterinário não encontrado.\n";
                }
                break;
            }
            case 4:
                std::cout << "Voltando ao menu principal...\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (loginOption != 3);
}


void mostrarMenuDono() {
    std::cout << "\n=== Menu Dono de Pet ===\n";
    std::cout << "1. Cadastrar Pet\n";
    std::cout << "2. Ver Dados do Dono e Pets\n";
    std::cout << "3. Editar Dados do Dono\n"; // Opção de editar dados
    std::cout << "4. Buscar Veterinários\n";
    std::cout << "5. Solicitar Consulta\n";
    std::cout << "6. Falar com Veterinário\n";
    std::cout << "7. Sair\n";
    std::cout << "Escolha uma opção: ";
}


void mostrarMenuVeterinario() {
    std::cout << "\n=== Menu Veterinário ===\n";
    std::cout << "1. Editar Dados do Veterinário\n"; // Opção de editar dados
    std::cout << "2. Ver Agenda\n";
    std::cout << "3. Aceitar ou Recusar Consultas\n";
    std::cout << "4. Falar com Dono do Pet\n";
    std::cout << "5. Sair\n";
    std::cout << "Escolha uma opção: ";
}
// Função para cadastrar um dono
void cadastrarDono(std::map<std::string, std::shared_ptr<Dono>> &Donos) {
    std::string CPF, name, password, cidade;

    std::cout << "Digite o CPF do Dono: ";
    std::cin >> CPF;
    std::cout << "Digite o Nome do Dono: ";
    std::cin >> name;
    std::cout << "Digite a Senha do Dono: ";
    std::cin >> password;
    std::cout << "Digite a Cidade: ";
    std::cin >> cidade;

    // Criar o novo dono com os dados fornecidos
    Donos[CPF] = std::make_shared<Dono>(CPF, name, password);
    Donos[CPF]->cidade = cidade;

    std::cout << "Dono cadastrado com sucesso!" << std::endl;
}

void editarDadosDono(const std::shared_ptr<Dono> &Dono) {
    std::string novoNome, novaCidade;

    std::cout << "Digite o novo nome (ou pressione Enter para manter o atual): ";
    std::cin.ignore();
    std::getline(std::cin, novoNome);

    std::cout << "Digite a nova cidade (ou pressione Enter para manter o atual): ";
    std::getline(std::cin, novaCidade);
        
    if (!novoNome.empty()) {
        Dono->name = novoNome;
    }

    if (!novaCidade.empty()) {
        Dono->cidade = novaCidade; 
    }

    Dono->editarDados(novoNome, novaCidade);
}
// Função para editar dados de um pet
void editarDadosPet(const std::shared_ptr<Dono> &Dono) {
    if (Dono->pets.empty()) {
        std::cout << "Nenhum pet cadastrado para editar." << std::endl;
        return;
    }

    std::string petId;
    std::cout << "Digite o CFMV do pet que deseja editar: ";
    std::cin >> petId;

    if (Dono->pets.find(petId) != Dono->pets.end()) {
        auto &pet = Dono->pets[petId];
        std::string novoNome, novaEspecie, novaRaca;
        int novaIdade;

        std::cout << "Digite o novo nome do pet (ou pressione Enter para manter o atual): ";
        std::cin.ignore();
        std::getline(std::cin, novoNome);
        if (!novoNome.empty()) pet->name = novoNome;

        std::cout << "Digite a nova espécie do pet (ou pressione Enter para manter o atual): ";
        std::getline(std::cin, novaEspecie);
        if (!novaEspecie.empty()) pet->species = novaEspecie;

        std::cout << "Digite a nova raça do pet (ou pressione Enter para manter o atual): ";
        std::getline(std::cin, novaRaca);
        if (!novaRaca.empty()) pet->breed = novaRaca;

        std::cout << "Digite a nova idade do pet (ou pressione Enter para manter o atual): ";
        std::string idadeInput;
        std::getline(std::cin, idadeInput);
        if (!idadeInput.empty()) novaIdade = std::stoi(idadeInput);
        else novaIdade = pet->age;
        pet->age = novaIdade;

        std::cout << "Dados do pet atualizados com sucesso!" << std::endl;
        pet->displayInfo();
    } else {
        std::cout << "Pet com CFMV " << petId << " não encontrado." << std::endl;
    }
}
// Função para solicitar consulta
void solicitarConsulta(AppointmentSystem &appointmentSystem, const std::string &DonoId) {
    appointmentSystem.agendarConsulta(DonoId, "vet1", "Buddy", "2024-12-10", "10:00 AM");
    std::cout << "Consulta agendada com sucesso!" << std::endl;
}
// Função para cadastrar um veterinário
void cadastrarVeterinario(std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios) {
    std::string CFMV, name, specialty, password;

    std::cout << "Digite o CFMV do Veterinário: ";
    std::cin >> CFMV;
    std::cout << "Digite o Nome do Veterinário: ";
    std::cin >> name;
    std::cout << "Digite a Especialidade: ";
    std::cin >> specialty;
    std::cout << "Digite a Senha do Veterinário: ";
    std::cin >> password;

    Veterinarios[CFMV] = std::make_shared<Veterinario>(CFMV, name, specialty, password);
    std::cout << "Veterinário cadastrado com sucesso!" << std::endl;
}
void cadastrarPetMenu(const std::shared_ptr<Dono>& Dono) {
    std::string petId, petName, species, breed;
    int age;

    std::cout << "Digite o ID do Pet: ";
    std::cin >> petId;
    std::cout << "Digite o Nome do Pet: ";
    std::cin >> petName;
    std::cout << "Digite a Espécie do Pet: ";
    std::cin >> species;
    std::cout << "Digite a Raça do Pet: ";
    std::cin >> breed;
    std::cout << "Digite a Idade do Pet: ";
    std::cin >> age;

    Dono->cadastrarPet(petId, petName, species, breed, age);
}

void editarDadosDonoMenu(const std::shared_ptr<Dono>& Dono) {
    std::string novoNome, novaCidade;

    std::cout << "Digite o novo nome (ou pressione Enter para manter o atual): ";
    std::cin.ignore();
    std::getline(std::cin, novoNome);

    std::cout << "Digite a nova cidade (ou pressione Enter para manter a atual): ";
    std::getline(std::cin, novaCidade);

   
    if (!novoNome.empty()) {
        Dono->name = novoNome;
    }

    if (!novaCidade.empty()) {
        Dono->cidade = novaCidade;
    }

    std::cout << "Dados do dono atualizados com sucesso!" << std::endl;
    Dono->displayInfo();
}




void menuDono(const std::shared_ptr<Dono> &Dono, AppointmentSystem &appointmentSystem, std::map<std::string, std::shared_ptr<Veterinario>> &Veterinarios){
    int DonoOption;

    do {
        mostrarMenuDono();
        std::cin >> DonoOption;

        switch (DonoOption) {
            case 1:
                cadastrarPetMenu(Dono);
                break;
            case 2:
                editarDadosDono(Dono);
                break;
            case 3:
                editarDadosPet(Dono);
                break;
            case 4:
                buscarVeterinarios(Veterinarios);
                    break;
            case 5:
                solicitarConsulta(appointmentSystem, Dono->CPF);
                break;
            case 7:
                std::cout << "Saindo do menu do dono." << std::endl;
                break;
            default:
                std::cout << "Opção inválida. Tente novamente." << std::endl;
        }
    } while (DonoOption != 7);
}

void editarDadosVeterinario(const std::shared_ptr<Veterinario> &vet) {
    std::string novoNome, novaEspecialidade;

    std::cout << "Digite o novo nome (ou pressione Enter para manter o atual): ";
    std::cin.ignore();
    std::getline(std::cin, novoNome);

    std::cout << "Digite a nova especialidade (ou pressione Enter para manter a atual): ";
    std::getline(std::cin, novaEspecialidade);

    vet->editarDados(novoNome, novaEspecialidade);
}


void visualizarAgendaVeterinario(const std::shared_ptr<Veterinario> &vet) {
    vet->visualizarAgenda();
}


void comunicarComDono() {
    std::string donoCpf, mensagem;

    std::cout << "Digite o CPF do dono: ";
    std::cin >> donoCpf;
    std::cout << "Digite a mensagem para o dono: ";
    std::cin.ignore();
    std::getline(std::cin, mensagem);

    std::cout << "Mensagem enviada para o dono " << donoCpf << ": " << mensagem << std::endl;
}





void menuVeterinario(const std::shared_ptr<Veterinario> &vet) {
    int vetOption;

    do {
        mostrarMenuVeterinario();
        std::cin >> vetOption;

        switch (vetOption) {
            case 1: 
                editarDadosVeterinario(vet);
                break;
            case 2: 
                visualizarAgendaVeterinario(vet);
                break;
            case 4: 
                comunicarComDono();
                break;
            case 5:
                std::cout << "Saindo do menu do veterinário." << std::endl;
                break;
            default:
                std::cout << "Opção inválida. Tente novamente." << std::endl;
        }
    } while (vetOption != 5);
}


int main() {
    std::map<std::string, std::shared_ptr<Dono>> Donos;
    std::map<std::string, std::shared_ptr<Veterinario>> Veterinarios;
    NotificationSystem notificationSystem;
    AppointmentSystem appointmentSystem(notificationSystem);

    int mainOption;

    do {
        showMainMenu();
        std::cin >> mainOption;

        switch (mainOption) {
            case 1:
                handleCadastro(Donos, Veterinarios);
                break;
            case 2:
                handleLogin(Donos, Veterinarios, appointmentSystem);
                break;
            case 3:
                std::cout << "Sistema encerrado.\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (mainOption != 3);

    return 0;
}
