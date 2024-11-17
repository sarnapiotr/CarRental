#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <sqlite3.h>

using namespace std;

ofstream Report("raport.txt");
sqlite3* db;

void initializeDatabase(sqlite3*& db) {
	int result = sqlite3_open("carrental.db", &db);
	if (result != SQLITE_OK) {
		cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		exit(1);
	}

	const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            accountType TEXT NOT NULL,
            borrowedCar TEXT
        );
    )";

	char* errorMessage;
	result = sqlite3_exec(db, createTableQuery, 0, 0, &errorMessage);
	if (result != SQLITE_OK) {
		std::cerr << "Error creating table: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
	}
	else {
		std::cout << "Table `users` initialized successfully." << std::endl;
	}
}

void addUser(sqlite3* db, const string& username, const string& password, const string& accountType, const string& borrowedCar = "") {
	const char* insertQuery = R"(
        INSERT INTO users (username, password, accountType, borrowedCar)
        VALUES (?, ?, ?, ?);
    )";

	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, 0) != SQLITE_OK) {
		cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
		return;
	}

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, accountType.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, borrowedCar.c_str(), -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void listUsers(sqlite3* db) {
	const char* selectQuery = "SELECT * FROM users;";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0) != SQLITE_OK) {
		cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
		return;
	}

	cout << "\nUzytkownicy w bazie danych: \n";
	cout << "ID | Username | AccountType | BorrowedCar\n";
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		const char* accountType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		const char* borrowedCar = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

		cout << id << " | " << username << " | " << accountType << " | " << (borrowedCar ? borrowedCar : "None") << "\n";
	}

	sqlite3_finalize(stmt);
}

void closeDatabase(sqlite3* db) {
	sqlite3_close(db);
}

void clearScreen() {
	system("cls");
}

void showMainMenu() {
	clearScreen();
	cout << "=== Wynajem samochodow ===\n";
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

void showCarManagementMenu() {
	clearScreen();
	cout << "=== Zarzadzanie samochodami ===\n";
	cout << "1. Dodaj\n";
	cout << "2. Usun\n";
	cout << "3. Edytuj\n";
	cout << "4. Powrot\n";
	cout << "Wybierz opcje: ";
}

void addToReport(string text) {
	Report << text << "\n";
}

void addUserActionToReport(string user, string action, string description = "") {
	Report << user << " " << action << " " << description << "\n";
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
		showCarManagementMenu();
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
				addToReport("Model samochodu zostala zaktualizowana z: " + temp + " na: " + cars[id].model);
				break;
			}
			case 3:
			{
				cout << "Podaj nowy rok: ";
				string temp = cars[id].year;
				cin >> cars[id].year;
				cout << "Rok samochodu zostal zaktualizowany\n";
				addToReport("Rok samochodu zostala zaktualizowana z: " + temp + " na: " + cars[id].year);
				break;
			}
			case 4:
				cout << "Edycja anulowana\n";
				addToReport("Edycja samochodu anulowana");
				system("pause");
				return;
			}

			ofstream outputFile(carDatabaseFile, ios::trunc);
			for (const auto& car : cars)
			{
				outputFile << car.brand << " " << car.model << " " << car.year << " " << car.status << "\n";
			}
			outputFile.close();

			cout << "Samochod zostal pomyslnie zaktualizowany w bazie danych\n";
			system("pause");
			break;
		}
	}
};

void adminMenu() {
	int adminChoice = 0;
	CarManagementClass carManagementObject;
	while (true) {
		showAdminMenu();
		cin >> adminChoice;

		if (cin.fail() || adminChoice < 1 || adminChoice > 4) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
			addToReport("ERROR: Błąd wyboru");
			system("pause");
			continue;
		}

		switch (adminChoice) {
		case 1:
			cout << "Zarzadzanie uzytkownikami\n";
			addToReport("-> Zarzadzanie uzytkownikami");
			listUsers(db);
			system("pause");
			break;
		case 2:
			cout << "Pobieranie raportu\n";
			getReport();
			break;
		case 3:
			carManagementObject.carManagement();
			break;
		case 4:
			cout << "Wylogowano z konta admina\n";
			addUserActionToReport("admin", "Wylogował się");
			system("pause");
			return;
		}
	}
}

void logowanie() {
	string login, haslo;

	while (true) {
		clearScreen();
		addToReport("-> Logowanie");
		cout << "=== Logowanie ===\n";
		cout << "Podaj login: ";
		cin >> login;
		cout << "Podaj haslo: ";
		cin >> haslo;

		if (login == "admin" && haslo == "admin") {
			addUserActionToReport(login, "Zalogował się");
			adminMenu();
			return;
		}
		else {
			int choice;
			clearScreen();
			addToReport("ERROR: Błędny login lub hasło");
			cout << "Bledny login lub haslo\n";
			cout << "1. Sprobuj ponownie\n";
			cout << "2. Powrot do menu glownego\n";
			cout << "Wybierz opcje: ";
			cin >> choice;

			if (cin.fail() || (choice != 1 && choice != 2)) { // Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
				addToReport("ERROR: Błąd wyboru");
				system("pause");
				continue;
			}

			if (choice == 2)
				return;
		}
	}
}

void rejestracja() {
	clearScreen();
	string nowyLogin, noweHaslo;
	addToReport("-> Rejestracja");
	cout << "=== Rejestracja ===\n";
	cout << "Podaj nowy login: ";
	cin >> nowyLogin;
	cout << "Podaj nowe haslo: ";
	cin >> noweHaslo;

	if (nowyLogin != "admin" && noweHaslo != "admin") {
		addUser(db, nowyLogin, noweHaslo, "user");
		cout << "Utworzono nowego uzytkownika pomyslnie\n";
	}
	else {
		cout << "Nie mozna utworzyc kolejnego konta administratora\n";
	}
	
	int wybor;

	while (true) {
		cout << "1. Powrot do menu glownego\n";
		cout << "Wybierz opcje: ";
		cin >> wybor;

		if (cin.fail() || wybor != 1) { // Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clearScreen();
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
			addToReport("ERROR: Błąd wyboru");
		}
		else {
			break;
		}
	}
}

int main() {
	initializeDatabase(db);

	int mainMenuChoice = 0;
	while (true) {
		showMainMenu();
		cin >> mainMenuChoice;

		if (cin.fail() || mainMenuChoice < 1 || mainMenuChoice > 3) { // Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
			addToReport("ERROR: Błąd wyboru");
			system("pause");
			continue;
		}

		switch (mainMenuChoice) {
		case 1:
			logowanie();
			break;
		case 2:
			rejestracja();
			break;
		case 3:
			cout << "Wyjscie\n";
			addToReport("Wyjśćie");
			getReport();
			closeDatabase(db);
			return 0;
		}
	}
}
