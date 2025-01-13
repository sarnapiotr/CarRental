#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>

using namespace std;

ofstream Report("raport.txt");

struct User {
    string username;
    string password;
    string accountType;  // Typ konta (np. "user" lub "admin")
    string borrowedCar;
};

vector<User> users;

void saveUsers() {
    ofstream userFile("users.txt", ios::trunc);
    for (const auto& user : users) {
        userFile << user.username << "," << user.password << "," << user.accountType << "," << user.borrowedCar << "\n";
    }
    userFile.close();
}

void addUser(const string& username, const string& password, const string& accountType, const string& borrowedCar = "") {
    users.push_back({ username, password, accountType, borrowedCar });
    saveUsers();
}
void loadUsers() {
    ifstream userFile("users.txt");
    string line;
    bool adminExists = false;
    while (getline(userFile, line)) {
        stringstream ss(line);
        User user;
        getline(ss, user.username, ',');
        getline(ss, user.password, ',');
        getline(ss, user.accountType, ',');
        getline(ss, user.borrowedCar, ',');
        users.push_back(user);

        // Sprawdzamy, czy już istnieje użytkownik admin
        if (user.username == "admin") {
            adminExists = true;
        }
    }
    userFile.close();

    // Jeśli użytkownik admin nie istnieje, tworzony jest domyślny admin
    if (!adminExists) {
        addUser("admin", "admin", "admin");
    }
}

bool userExists(const string& username) {
    for (const auto& user : users) {
        if (user.username == username) {
            return true;
        }
    }
    return false;
}

bool authenticateUser(const string& username, const string& password, string& accountType) {
    for (const auto& user : users) {
        bool check = userExists(username);
        if (check) {
            if (user.username == username && user.password == password) {
                accountType = user.accountType;
                return true;
            }
        }
        else {
            cout << "Taki użytkownik już istnieje!!\n";
        }

    }
    return false;
}

void listUsers() {
    cout << "\nUzytkownicy w pliku: \n";
    cout << "Username | AccountType | BorrowedCar\n";
    for (const auto& user : users) {
        cout << user.username << " | " << user.accountType << " | " << (user.borrowedCar.empty() ? "None" : user.borrowedCar) << "\n";
    }
}

void clearScreen() {
    system("cls");
}

void showMainMenu() {
    clearScreen();
    cout << "=== Wynajem samochodow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!===\n";
    cout << "1. Logowanie\n";
    cout << "2. Rejestracja\n";
    cout << "3. Wyjscie\n";
    cout << "Wybierz opcje: ";
}

void showAdminMenu() {
    clearScreen();
    cout << "=== Menu Admina ===\n";
    cout << "1. Zarzadzanie uzytkownikami\n";
    cout << "2. Pobieranie raportu\n";
    cout << "3. Zarzadzanie samochodami\n";
    cout << "4. Wyloguj\n";
    cout << "Wybierz opcje: ";
}

void showUserMenu() {
    clearScreen();
    cout << "=== Panel Uzytkownika ===\n";
    cout << "1. Wypozycz samochod\n";
    cout << "2. Zwróć samochod\n";
    cout << "3. Wyloguj\n";
    cout << "Wybierz opcje: ";
}

void addToReport(string text) {
    Report << text << "\n";
}

void getReport() {
    Report.close();
}

class Car {
public:
    string brand;
    string model;
    string year;
    bool status;

    Car(string brand, string model, string year, bool status)
        : brand(brand), model(model), year(year), status(status) {}

    string toString() const {
        return brand + " " + model + " " + year + " " + (status ? "1" : "0");
    }

    static Car fromString(const string& str) {
        string brand, model, year;
        bool status;
        istringstream iss(str);
        iss >> brand >> model >> year >> status;
        return Car(brand, model, year, status);
    }
};

class CarManagementClass {
private:
    const string carDatabaseFile = "cars.txt";

    vector<Car> readCarsFromFile() {
        vector<Car> cars;
        ifstream file(carDatabaseFile);
        string line;
        while (getline(file, line))
        {
            cars.push_back(Car::fromString(line));
        }
        file.close();
        return cars;
    }

    void writeCarsToFile(const vector<Car>& cars) {
        ofstream file(carDatabaseFile);
        for (const Car& car : cars)
        {
            file << car.toString() << endl;
        }
        file.close();
    }

public:
    void carManagement() {
        int choice = 0;
        clearScreen();
        cout << "=== Zarzadzanie samochodami ===\n";
        cout << "1. Dodaj\n";
        cout << "2. Usun\n";
        cout << "3. Edytuj\n";
        cout << "4. Powrot\n";
        cout << "Wybierz opcje: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
            addToReport("ERROR: Błąd wyboru");
            system("pause");
            return;
        }

        switch (choice) {
        case 1:
            this->carManagementAdd();
            break;
        case 2:
            this->carManagementDelete();
            break;
        case 3:
            this->carManagementEdit();
            break;
        case 4:
            return;
        }
    }

    void carManagementAdd() {
        clearScreen();
        string brand, model, year;
        addToReport("-> Dodawanie samochodu");
        cout << "=== Dodaj samochod ===\n";
        cout << "Podaj marke:\n";
        cin >> brand;
        cout << "Podaj model:\n";
        cin >> model;
        cout << "Podaj rok:\n";
        cin >> year;

        int choice = 0;
        cout << "Czy chcesz dodac samochod: " << brand << " " << model << " " << year << "?\n";
        cout << "1. Dodaj\n";
        cout << "2. Anuluj\n";

        while (true) {
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 2) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
                addToReport("ERROR: Błąd wyboru");
                system("pause");
                continue;
            }

            if (choice == 1) {
                vector<Car> cars = readCarsFromFile();
                cars.push_back(Car(brand, model, year, false));
                writeCarsToFile(cars);
                addToReport("Dodawanie samochodu: " + brand + " " + model + " " + year);
                cout << "Samochod zostal dodany\n";
            }
            else {
                addToReport("Dodawanie samochodu anulowane");
                cout << "Dodawanie anulowane\n";
            }
            break;
        }
        system("pause");
    }

    void carManagementDelete() {
        clearScreen();
        addToReport("-> Usuwanie samochodu");
        cout << "=== Usun samochod ===\n";

        vector<Car> cars = readCarsFromFile();
        if (cars.empty()) {
            cout << "Brak samochodow do usuniecia\n";
            addToReport("Brak samochodów do usunięcia");
            system("pause");
            return;
        }

        for (size_t i = 0; i < cars.size(); ++i) {
            cout << i << ". Marka: " << cars[i].brand << ", Model: " << cars[i].model << ", Rok: " << cars[i].year << "\n";
        }

        int id;
        cout << "Podaj ID samochodu do usuniecia: ";
        while (true) {
            cin >> id;

            if (cin.fail() || id < 0 || id >= cars.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy ID, sprobuj ponownie\n";
                addToReport("ERROR: Nieprawidłowy ID");
                system("pause");
                continue;
            }

            cout << "Czy na pewno chcesz usunac samochod: " << cars[id].brand << " " << cars[id].model << " " << cars[id].year << "?\n";
            cout << "1. Tak\n";
            cout << "2. Anuluj\n";
            int confirmChoice;
            cin >> confirmChoice;

            if (confirmChoice == 1) {
                addToReport("Usunięcie samochodu: " + cars[id].brand + " " + cars[id].model + " " + cars[id].year);
                cars.erase(cars.begin() + id);
                writeCarsToFile(cars);
                cout << "Samochod zostal usuniety\n";
            }
            else {
                addToReport("Usuwanie samochodu anulowane");
                cout << "Usuwanie anulowane\n";
            }
            system("pause");
            break;
        }
    }

    void carManagementEdit() {
        clearScreen();
        addToReport("-> Edytowanie samochodu");
        cout << "=== Edytuj samochod ===\n";

        vector<Car> cars;
        ifstream inputFile(carDatabaseFile);
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string brand, model, year;
            bool status;
            ss >> brand >> model >> year >> status;
            cars.emplace_back(brand, model, year, status);
        }
        inputFile.close();

        if (cars.empty()) {
            cout << "Brak samochodow do edytowania\n";
            addToReport("Brak samochodów do edytowania");
            system("pause");
            return;
        }

        for (size_t i = 0; i < cars.size(); ++i) {
            cout << i << ". Marka: " << cars[i].brand << ", Model: " << cars[i].model << ", Rok: " << cars[i].year << "\n";
        }

        int id;
        cout << "Podaj ID samochodu do edytowania: ";
        while (true) {
            cin >> id;

            if (cin.fail() || id < 0 || id >= cars.size()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy ID, sprobuj ponownie\n";
                addToReport("ERROR: Nieprawidłowy ID");
                system("pause");
                continue;
            }

            int editChoice;
            cout << "Wybierz co chcesz edytowac:\n";
            cout << "1. Marka\n";
            cout << "2. Model\n";
            cout << "3. Rok\n";
            cout << "4. Anuluj\n";
            cin >> editChoice;

            if (cin.fail() || editChoice < 1 || editChoice > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nieprawidlowy wybor, sprobuj ponownie.\n";
                addToReport("ERROR: Błąd wyboru");
                system("pause");
                continue;
            }

            switch (editChoice) {
            case 1: {
                cout << "Podaj nowa marke: ";
                string temp = cars[id].brand;
                cin >> cars[id].brand;
                cout << "Marka samochodu zostala zaktualizowana\n";
                addToReport("Marka samochodu zostala zaktualizowana z: " + temp + " na: " + cars[id].brand);
                break;
            }
            case 2: {
                cout << "Podaj nowy model: ";
                string temp = cars[id].model;
                cin >> cars[id].model;
                cout << "Model samochodu zostal zaktualizowany\n";
                addToReport("Model samochodu zostal zaktualizowany z: " + temp + " na: " + cars[id].model);
                break;
            }
            case 3: {
                cout << "Podaj nowy rok: ";
                string temp = cars[id].year;
                cin >> cars[id].year;
                cout << "Rok samochodu zostal zaktualizowany\n";
                addToReport("Rok samochodu zostal zaktualizowany z: " + temp + " na: " + cars[id].year);
                break;
            }
            case 4:
                break;
            }
            break;
        }

        writeCarsToFile(cars);
        system("pause");
    }
};

int main() {
    loadUsers();
    string loggedInUserType = "";
    string loggedInUsername = "";

    int choice;
    while (true) {
        showMainMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
            system("pause");
            continue;
        }

        if (choice == 1) {
            // Logowanie
            clearScreen();
            string username, password;
            cout << "Podaj login: ";
            cin >> username;
            cout << "Podaj haslo: ";
            cin >> password;

            if (authenticateUser(username, password, loggedInUserType)) {
                cout << "Zalogowano pomyslnie jako: " << username << "\n";
                loggedInUsername = username;

                if (loggedInUserType == "admin") {
                    // Panel administratora
                    while (true) {
                        int adminChoice = 0;
                        showAdminMenu();
                        cin >> adminChoice;

                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
                            system("pause");
                            continue;
                        }

                        if (adminChoice == 1) {
                            listUsers();
                            system("pause");
                        }
                        else if (adminChoice == 2) {
                            cout << "Pobieranie raportu...\n";
                            getReport();
                            system("pause");
                        }
                        else if (adminChoice == 3) {
                            CarManagementClass cm;
                            cm.carManagement();
                        }
                        else if (adminChoice == 4) {
                            cout << "Wylogowano.\n";
                            break;
                        }
                        else {
                            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
                        }
                    }
                }
                else if (loggedInUserType == "user") {
                    // Panel użytkownika
                    while (true) {
                        int userChoice = 0;
                        showUserMenu();
                        cin >> userChoice;

                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
                            system("pause");
                            continue;
                        }

                        if (userChoice == 1) {
                            cout << "Wypożyczenie samochodu (funkcja w trakcie implementacji).\n";
                            system("pause");
                        }
                        else if (userChoice == 2) {
                            cout << "Zwrócenie samochodu (funkcja w trakcie implementacji).\n";
                            system("pause");
                        }
                        else if (userChoice == 3) {
                            cout << "Wylogowano.\n";
                            break;
                        }
                        else {
                            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
                        }
                    }
                }
            }
            else {
                cout << "Błędny login lub hasło. Spróbuj ponownie.\n";
                system("pause");
            }
        }
        else if (choice == 2) {
            // Rejestracja
            clearScreen();
            string username, password;
            cout << "Podaj login: ";
            cin >> username;
            cout << "Podaj haslo: ";
            cin >> password;

            if (userExists(username)) {
                cout << "Użytkownik o podanej nazwie już istnieje.\n";
            }
            else {
                addUser(username, password, "user");
                cout << "Zarejestrowano użytkownika: " << username << "\n";
            }
            system("pause");
        }
        else if (choice == 3) {
            cout << "Dziękujemy za skorzystanie z programu. Do zobaczenia!\n";
            break;
        }
        else {
            cout << "Niepoprawny wybór. Spróbuj ponownie.\n";
            system("pause");
        }
    }

    return 0;
}
