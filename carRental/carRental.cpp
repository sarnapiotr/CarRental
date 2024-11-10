#include <iostream>
#include <string>

using namespace std;

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
	cout << "1. Opcja administracyjna\n";
	cout << "2. Wyloguj\n";
	cout << "Wybierz opcje: ";
}

void adminMenu() {
	int adminChoice = 0;
	while (true) {
		showAdminMenu();
		cin >> adminChoice;

		if (cin.fail() || adminChoice < 1 || adminChoice > 2) {//Kontrola błędu przy niepoprawym wyborze
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Nieprawidlowy wybor, sprobuj ponownie.\n";
			system("pause");
			continue;
		}

		switch (adminChoice) {
			case 1:
				cout << "Wykonano operacje administracyjna\n";
				system("pause");
				break;
			case 2:
				cout << "Wylogowano\n";
				system("pause");
				return;
			}
	}
}

void logowanie() {
	string login, haslo;
	bool loggedIn = false;

	while (!loggedIn) {
		clearScreen();
		cout << "=== Logowanie ===\n";
		cout << "Podaj login: ";
		cin >> login;
		cout << "Podaj haslo: ";
		cin >> haslo;

		if (login == "admin" && haslo == "admin") {
			loggedIn = true;
			adminMenu();
		}
		else {
			int choice;
			clearScreen();
			cout << "Bledny login lub haslo.\n";
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

			if (choice == 2) {
				return;
			};
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