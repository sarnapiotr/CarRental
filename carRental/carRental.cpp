#include <iostream>
#include <string>
#include <vector>

using namespace std;

void adminMenu();

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

void showCarManagmentMenu() {
	clearScreen();
	cout << "=== Zarzadzanie samochodami ===\n";
	cout << "1. Dodaj\n";
	cout << "2. Usun\n";
	cout << "3. Edytuj\n";
	cout << "4. Powrot\n";
	cout << "Wybierz opcje: ";
}

class Car {
public:
	string brand;
	string model;
	string year;

	Car(string brand, string model, string year) : brand(brand), model(model), year(year) {}

	string getBrand() {
		return this->brand;
	}

	string getModel() {
		return this->model;
	}

	string getYear() {
		return this->year;
	}
};

class CarManagmentClass {
private:
	vector<Car> cars;

public:
	void carManagment() {
		int choice = 0;

		showCarManagmentMenu();
		cin >> choice;

		if (cin.fail() || choice < 1 || choice > 4) {//Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
			system("pause");
			return;
		}

		switch (choice) {
		case 1:
			this->carManagmentAdd();
			break;
		case 2:
			this->carManagmentDelete();
			break;
		case 3:
			this->carManagmentEdit();
			break;
		case 4:
			return;
		}
	}

	void carManagmentAdd() {
		clearScreen();
		string brand, model, year;
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

			if (cin.fail() || choice < 1 || choice > 2) {//Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
				system("pause");
				continue;
			}

			if (choice == 1) {
				cars.push_back(Car(brand, model, year));
				cout << "Samochod zostal dodany\n";
			}
			else {
				cout << "Dodawanie anulowane\n";
			}
			break;
		}
		system("pause");
	}

	void carManagmentDelete() {
		clearScreen();
		cout << "=== Usun samochod ===\n";

		if (cars.empty()) {
			cout << "Brak samochodow do usuniecia\n";
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

			if (cin.fail() || id < 0 || id >= cars.size()) {//Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy ID, sprobuj ponownie\n";
				system("pause");
				continue;
			}

			cout << "Czy na pewno chcesz usunac samochod: " << cars[id].brand << " " << cars[id].model << " " << cars[id].year << "?\n";
			cout << "1. Tak\n";
			cout << "2. Anuluj\n";
			int confirmChoice;
			cin >> confirmChoice;

			if (confirmChoice == 1) {
				cars.erase(cars.begin() + id);
				cout << "Samochod zostal usuniety\n";
			}
			else {
				cout << "Usuwanie anulowane\n";
			}
			system("pause");
			break;
		}
	}

	void carManagmentEdit() {
		clearScreen();
		cout << "=== Edytuj samochod ===\n";

		if (cars.empty()) {
			cout << "Brak samochodow do edytowania\n";
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

			if (cin.fail() || id < 0 || id >= cars.size()) {//Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy ID, sprobuj ponownie\n";
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

			if (cin.fail() || editChoice < 1 || editChoice > 4) {//Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy wybor, sprobuj ponownie.\n";
				system("pause");
				continue;
			}

			switch (editChoice) {
			case 1: {
				cout << "Podaj nowa marke: ";
				cin >> cars[id].brand;
				cout << "Marka samochodu zostala zaktualizowana\n";
				break;
			}
			case 2: {
				cout << "Podaj nowy model: ";
				cin >> cars[id].model;
				cout << "Model samochodu zostal zaktualizowany\n";
				break;
			}
			case 3: {
				cout << "Podaj nowy rok: ";
				cin >> cars[id].year;
				cout << "Rok samochodu zostal zaktualizowany\n";
				break;
			}
			case 4:
				cout << "Edycja anulowana\n";
				break;
			}
			system("pause");
			break;
		}
	}
};

void adminMenu() {
	int adminChoice = 0;
	CarManagmentClass carManagmentObject;
	while (true) {
		showAdminMenu();
		cin >> adminChoice;

		if (cin.fail() || adminChoice < 1 || adminChoice > 4) {//Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
			system("pause");
			continue;
		}

		switch (adminChoice) {
		case 1:
			cout << "Zarzadzanie uzytkownikami\n";
			system("pause");
			break;
		case 2:
			cout << "Pobieranie raportu\n";
			system("pause");
			break;
		case 3:
			carManagmentObject.carManagment();
			break;
		case 4:
			cout << "Wylogowano z konta admina\n";
			system("pause");
			return;
		}
	}
}

void logowanie() {
	string login, haslo;

	while (true) {
		clearScreen();
		cout << "=== Logowanie ===\n";
		cout << "Podaj login: ";
		cin >> login;
		cout << "Podaj haslo: ";
		cin >> haslo;

		if (login == "admin" && haslo == "admin") {
			adminMenu();
			return;
		}
		else {
			int choice;
			clearScreen();
			cout << "Bledny login lub haslo\n";
			cout << "1. Sprobuj ponownie\n";
			cout << "2. Powrot do menu glownego\n";
			cout << "Wybierz opcje: ";
			cin >> choice;

			if (cin.fail() || (choice != 1 && choice != 2)) {//Kontrola błędu przy niepoprawym wyborze
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
				system("pause");
				continue;
			}

			if (choice == 2) return;
		}
	}
}

void rejestracja() {
	clearScreen();
	string nowyLogin, noweHaslo;

	cout << "=== Rejestracja ===\n";
	cout << "Podaj nowy login: ";
	cin >> nowyLogin;
	cout << "Podaj nowe haslo: ";
	cin >> noweHaslo;

	cout << "Utworzono nowego uzytkownika pomyslnie\n";
	int wybor;

	while (true) {
		cout << "1. Powrot do menu glownego\n";
		cout << "Wybierz opcje: ";
		cin >> wybor;

		if (cin.fail() || wybor != 1) {//Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clearScreen();
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
		}
		else {
			break;
		}
	}
}

int main() {
	int mainMenuChoice = 0;
	while (true) {
		showMainMenu();
		cin >> mainMenuChoice;

		if (cin.fail() || mainMenuChoice < 1 || mainMenuChoice > 3) {//Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie\n";
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
			return 0;
		}
	}
}
