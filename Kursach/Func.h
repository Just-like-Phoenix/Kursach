#include "Libriaries.h"

class Game;
class MatchCSGO;
class MatchValorant;
class Bet;
class betList;
class User;

Menu* mainMenu;
betList* Start = NULL, * Finish = NULL;
betList* Start2 = NULL, * Finish2 = NULL;

class Game {
public:
	Game() {
		ifstream file;
		file.open("marge.txt", ios_base::in);
		if (file.is_open()) file >> marge;
		else {
			file.close();
			ofstream f;
			f.open("marge.txt", ios_base::app);
			marge = 6;
			f << marge;
			f.close();
		}
		file.close();
	}
	int marge;
	string game;
	string ValorantTeams[10] = { "Sentinels", "TeamLiquid", "VisionStrikers", "GambitEsports", "TeamEnvy", "PaperRex", "Acend", "X10CRIT", "100Thieves", "Australs" };
	string CSGOTeams[10] = { "NaVi", "G2", "Gambit", "Heroic", "Vitality", "NiP", "Virtus.pro", "FazeClan", "Entropiq", "Furia" };
	virtual void setMatch() = 0;
	void setMarge(int num) {
		marge = num;
		if (marge > 100) marge = 100;
		else if (marge < 0) marge = 0;
	}
	virtual int getMarge() = 0;
};

class MatchCSGO : public Game {
	double diff = 0;
public:
	string team1, team2;
	double ratio1, ratio2;
	int bank;
	int id;

	int getMarge() { return Game::marge; }
	void setMatch() {
		Game::game = "CS:GO";
		int choise1, choise2;

		for (int i = 0; i < 10; i++) {
			cout << (i + 1) << ". " << Game::CSGOTeams[i] << endl;
		}
		cout << endl << "Выберите первую команду: ";
		cin >> choise1;
		team1 = Game::CSGOTeams[choise1 - 1];

		system("cls");

		int num = 1;
		for (int i = 0; i < 10; i++) {
			if (i != choise1 - 1) {
				cout << (num) << ". " << Game::CSGOTeams[i] << endl;
				num++;
			}
		}
		cout << endl << "Выберите вторую команду: ";
		cin >> choise2;
		if (choise2 == choise1) choise2 += 1;
		team2 = Game::CSGOTeams[choise2];


		diff = ((10 - choise1) * 10 - (10 - choise2) * 10) / 4;
		ratio1 = 100 / ((50 + diff) + ((50 + diff) / 100) * MatchCSGO::Game::marge);
		ratio2 = 100 / ((50 - diff) + ((50 - diff) / 100) * MatchCSGO::Game::marge);

		bank = 500 + rand() % 10000;
		id = 100000 + rand() % 1000000;
	}
};

class MatchValorant : public Game {
	double diff = 0;
public:
	string team1, team2;
	double ratio1, ratio2;
	int bank;
	int id;

	int getMarge() { return Game::marge; }
	void setMatch() {
		Game::game = "Valorant";
		int choise1, choise2;

		for (int i = 0; i < 10; i++) {
			cout << (i + 1) << ". " << Game::ValorantTeams[i] << endl;
		}
		cout << endl << "Выберите первую команду: ";
		cin >> choise1;
		team1 = Game::ValorantTeams[choise1 - 1];

		system("cls");

		int num = 1;
		for (int i = 0; i < 10; i++) {
			if (i != choise1 - 1) {
				cout << (num) << ". " << Game::ValorantTeams[i] << endl;
				num++;
			}
		}
		cout << endl << "Выберите вторую команду: ";
		cin >> choise2;
		if (choise2 == choise1) choise2 += 1;
		team2 = Game::ValorantTeams[choise2];


		diff = ((10 - choise1) * 10 - (10 - choise2) * 10) / 4;
		ratio1 = 100 / ((50 + diff) + ((50 + diff) / 100) * Game::marge);
		ratio2 = 100 / ((50 - diff) + ((50 - diff) / 100) * Game::marge);

		bank = 500 + rand() % 10000;
		id = 100000 + rand() % 1000000;
	}
};

class Bet : public MatchValorant, public MatchCSGO {

public:
	double ratio1, ratio2;
	string team1, team2;
	double bank;
	int id;
	int team;

	void AddCSGOBet() {
		ofstream file;
		file.open("CSGObets.txt", ios_base::app);

		MatchCSGO::setMatch();

		file << MatchCSGO::ratio1 << " ";
		file << MatchCSGO::team1 << " ";
		file << MatchCSGO::team2 << " ";
		file << MatchCSGO::ratio2 << " ";
		file << MatchCSGO::bank << " ";
		file << MatchCSGO::id << " ";

		system("cls");
		cout << "Добавлено!";

		file.close();
	}
	void AddValorantBet() {
		ofstream file;
		file.open("Valorantbets.txt", ios_base::app);

		MatchValorant::setMatch();

		file << MatchValorant::ratio1 << " ";
		file << MatchValorant::team1 << " ";
		file << MatchValorant::team2 << " ";
		file << MatchValorant::ratio2 << " ";
		file << MatchValorant::bank << " ";
		file << MatchValorant::id << " ";

		system("cls");
		cout << "Добавлено!";

		file.close();
	}
	int Marge() { return MatchCSGO::getMarge(); }

	friend ostream& operator<<(ostream& out, const Bet& bet);
};

class betList {

public:
	betList* next;
	betList* prev;
	Bet bet;
	int count;

	void set(double _ratio1, double _ratio2, string _team1, string _team2, int _bank) {
		bet.ratio1 = _ratio1; bet.ratio2 = _ratio2;
		bet.team1 = _team1; bet.team2 = _team2;
		bet.bank = _bank;
	}
};

void delList(betList* start) {
	if (start != NULL) {
		delList(start->next);
		delete start;
	}
	else return;
}
void makeBetList(string fileName) {
	double ratio1, ratio2;
	string team1, team2;
	int bank;
	int id;
	int i = 1;
	betList* tmp = NULL;

	ifstream file;
	file.open(fileName, ios_base::in);
	while (file >> ratio1) {
		file >> team1;
		file >> team2;
		file >> ratio2;
		file >> bank;
		file >> id;
		tmp = new betList;
		if (Start == NULL) {
			Start = tmp;
			tmp->next = NULL;
			tmp->count = i;
			tmp->bet.ratio1 = ratio1;
			tmp->bet.team1 = team1;
			tmp->bet.team2 = team2;
			tmp->bet.ratio2 = ratio2;
			tmp->bet.bank = bank;
			tmp->bet.id = id;
			tmp->prev = NULL;
			Finish = tmp;
		}
		else {
			Finish->next = tmp;
			tmp->count = i;
			tmp->bet.ratio1 = ratio1;
			tmp->bet.team1 = team1;
			tmp->bet.team2 = team2;
			tmp->bet.ratio2 = ratio2;
			tmp->bet.bank = bank;
			tmp->bet.id = id;
			tmp->prev = Finish;
			Finish = tmp;
		}
		tmp = tmp->next;
		i++;
	}

	file.close();
}
void makeHisList(string fileName) {
	double ratio1, ratio2;
	string team1, team2;
	int bank;
	int id, junk;
	int i = 1;
	betList* tmp = NULL;

	ifstream file;
	file.open(fileName, ios_base::in);
	while (file >> ratio1) {
		file >> team1;
		file >> team2;
		file >> ratio2;
		file >> id;
		file >> bank;
		file >> junk;
		tmp = new betList;
		if (Start == NULL) {
			Start = tmp;
			tmp->next = NULL;
			tmp->count = i;
			tmp->bet.ratio1 = ratio1;
			tmp->bet.team1 = team1;
			tmp->bet.team2 = team2;
			tmp->bet.ratio2 = ratio2;
			tmp->bet.bank = bank;
			tmp->bet.id = id;
			tmp->bet.team = junk;
			tmp->prev = NULL;
			Finish = tmp;
		}
		else {
			Finish->next = tmp;
			tmp->count = i;
			tmp->bet.ratio1 = ratio1;
			tmp->bet.team1 = team1;
			tmp->bet.team2 = team2;
			tmp->bet.ratio2 = ratio2;
			tmp->bet.bank = bank;
			tmp->bet.id = id;
			tmp->bet.team = junk;
			tmp->prev = Finish;
			Finish = tmp;
		}
		tmp = tmp->next;
		i++;
	}

	file.close();
}

class User : public Bet {
	string name, fname;
	bool UserReg, AdminReg;

	string crypt(string log, string pass) {

		string result;
		int difference = rand() % 10;
		int i = 0;

		while (log[i] != '\0') {
			log[i] += difference;
			i++;
		}

		result += log + ' ' + to_string(difference) + ' ';
		i = 0;
		difference = 1 + rand() % 9;

		while (pass[i] != '\0') {
			pass[i] += difference;
			i++;
		}

		result += pass + ' ' + to_string(difference);

		return result;
	}
	string decrypt(string str) {
		int difference1, difference2;
		string log, pass, result;
		int i = 0;

		while (str[i] != ' ') {
			log += str[i];
			i++;
		}
		difference1 = (int)str[i + 1] - 48;

		i += 3;

		while (str[i] != ' ') {
			pass += str[i];
			i++;
		}
		difference2 = (int)str[i + 1] - 48;

		for (int j = 0; j < log.length(); j++) log[j] -= difference1;
		for (int j = 0; j < pass.length(); j++) pass[j] -= difference2;

		result += log + ' ' + pass;

		return result;
	}
	void write(string str) {
		ofstream file;

		file.open("logging.txt", ios_base::app);

		file << str;
		file << ' ';

		file.close();
	}
	bool AdminAuth(string example) {
		ifstream file;

		file.open("admin.txt", ios_base::in);


		bool running = true;
		bool exist = false;

		while (running)
		{
			string str, result;
			for (int i = 0; i < 4; i++) {
				if (!(file >> str)) {
					running = false;
					break;
				}
				else result += str + ' ';

				if (i == 3) {
					result[result.length() - 1] = '\0';
					if (decrypt(result) == example) exist = true;
				}
			}
		}

		file.close();

		if (exist) return true;
		else return false;
	}
	bool UserAuth(string example) {
		ifstream file;

		file.open("logging.txt", ios_base::in);


		bool running = true;
		bool exist = false;

		while (running)
		{
			string str, result;
			for (int i = 0; i < 4; i++) {
				if (!(file >> str)) {
					running = false;
					break;
				}
				else result += str + ' ';

				if (i == 3) {
					result[result.length() - 1] = '\0';
					if (decrypt(result) == example) exist = true;
				}
			}
		}

		file.close();

		if (exist) return true;
		else return false;
	}
public:
	User() { UserReg = false; AdminReg = false; };
	void Registration() {
		string log, pass;

		cout << "Придумайте логин: ";
		cin >> log;
		cout << "Придумайте пароль: ";
		cin >> pass;

		if (AdminAuth(log + ' ' + pass) || UserAuth(log + ' ' + pass)) {
			cout << "Такое имя уже существует";
		}
		else {
			write(crypt(log, pass));
			system("cls");
			cout << "Регистрация прошла успешно!";
			setName(log);
		}
	}
	void Authorization() {
		string log, pass;

		cout << "Введите логин: ";
		cin >> log;
		cout << "Введите пароль: ";
		cin >> pass;


		if (AdminAuth(log + ' ' + pass)) {
			AdminReg = true;
			cout << "Авторизация прошла успешно!";
			setName(log);
		}
		else {
			if (UserAuth(log + ' ' + pass)) {
				UserReg = true;
				cout << "Авторизация прошла успешно!";
				setName(log);
			}
			else cout << "Неверный логин или пароль!";
		}
	}
	int AdminOrUser() {
		if (AdminReg) return 1;
		else if (UserReg) return 2;
	}
	void setName(string log) {
		name = log;
		fname = name + ".txt";
	}
	void setMarge() {
		int marge;
		cout << "Введите маржу конторы: ";
		cin >> marge;
		Bet::MatchCSGO::Game::setMarge(marge);
		Bet::MatchValorant::Game::setMarge(marge);

		ofstream file;
		file.open("marge.txt", ios_base::trunc);
		file << marge;
		file.close();

		makeBetList("CSGObets.txt");
		betList* tmp = Start;

		ofstream fil;

		fil.open("CSGObets.txt", ios_base::trunc);

		while (tmp != NULL) {
			double x;
			x = (100 / tmp->bet.ratio1) / (1 + ((100 / tmp->bet.ratio1) + (100 / tmp->bet.ratio2) - 100) * 0.01);
			tmp->bet.ratio1 = 100 / (x + ((x / 100) * marge));

			x = (100 / tmp->bet.ratio2) / (1 + ((100 / tmp->bet.ratio1) + (100 / tmp->bet.ratio2) - 100) * 0.01);
			tmp->bet.ratio2 = 100 / (x + ((x / 100) * marge));

			fil << tmp->bet.ratio1 << " ";
			fil << tmp->bet.team1 << " ";
			fil << tmp->bet.team2 << " ";
			fil << tmp->bet.ratio2 << " ";
			fil << tmp->bet.bank << " ";
			fil << tmp->bet.id << " ";

			tmp = tmp->next;
		}
		fil.close();

		delList(Start);
		Start = Finish = NULL;

		makeBetList("Valorantbets.txt");
		tmp = Start;

		ofstream f;

		f.open("Valorantbets.txt", ios_base::trunc);

		while (tmp != NULL) {
			double x;
			x = (100 / tmp->bet.ratio1) / (1 + ((100 / tmp->bet.ratio1) + (100 / tmp->bet.ratio2) - 100) * 0.01);
			tmp->bet.ratio1 = 100 / (x + ((x / 100) * marge));

			x = (100 / tmp->bet.ratio2) / (1 + ((100 / tmp->bet.ratio1) + (100 / tmp->bet.ratio2) - 100) * 0.01);
			tmp->bet.ratio2 = 100 / (x + ((x / 100) * marge));

			f << tmp->bet.ratio1 << " ";
			f << tmp->bet.team1 << " ";
			f << tmp->bet.team2 << " ";
			f << tmp->bet.ratio2 << " ";
			f << tmp->bet.bank << " ";
			f << tmp->bet.id << " ";

			tmp = tmp->next;
		}
		f.close();

		delList(Start);
		Start = Finish = NULL;
	}
	void income() {
		double inc = 0;

		makeBetList("CSGObets.txt");
		betList* tmp = Start;

		while (tmp != NULL) {
			inc += tmp->bet.bank;
			tmp = tmp->next;
		}

		delList(Start);
		Start = Finish = NULL;

		makeBetList("Valorantbets.txt");
		tmp = Start;

		while (tmp != NULL) {
			inc += tmp->bet.bank;
			tmp = tmp->next;
		}

		delList(Start);
		Start = Finish = NULL;

		int mr = Bet::Marge();
		double difff = (double)mr / 100;
		inc = inc * difff;
		cout << "Прибыль с текущих активных ставок будет составлять: " << setprecision(5) << inc << " $";
	}
	void MakeACSBet() {
		makeBetList("CSGObets.txt");
		betList* tmp = Start;
		int num = 1;

		cout << "   +====================+====================+====================+====================+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
		cout << "+==+====================+====================+====================+====================+\n";

		while (tmp != NULL) {
			cout << "|" << num << " " << tmp->bet;
			num++;
			tmp = tmp->next;
		}

		cout << "\nВведите номер: ";
		cin >> num;
		if (Finish->count < num || Start->count > num) {
			system("cls");
			cout << "Такого номера нет";
		}
		else {
			tmp = Start;
			while (tmp != NULL) {
				if (tmp->count == num) {
					int money;
					cout << "Сколько вы хотите поставить ($): ";
					cin >> money;
					cout << "На какую команду (1/2): ";
					cin >> num;
					if (num > 2) num = 2;
					else if (num < 1) num = 1;

					ofstream file;
					file.open(fname, ios_base::app);
					file << tmp->bet.ratio1 << " ";
					file << tmp->bet.team1 << " ";
					file << tmp->bet.team2 << " ";
					file << tmp->bet.ratio2 << " ";
					file << tmp->bet.id << " ";
					file << -money << " ";
					file << num << " ";
					file.close();
					break;
				}
				else {
					tmp = tmp->next;
				}
			}
		}
		delList(Start);
		Start = Finish = NULL;
	}
	void MakeAValBet() {
		makeBetList("Valorantbets.txt");
		betList* tmp = Start;
		int num = 1;

		cout << "   +====================+====================+====================+====================+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
		cout << "+==+====================+====================+====================+====================+\n";

		while (tmp != NULL) {
			cout << "|" << num << " " << tmp->bet;
			num++;
			tmp = tmp->next;
		}

		cout << "\nВведите номер: ";
		cin >> num;
		if (Finish->count < num || Start->count > num) {
			system("cls");
			cout << "Такого номера нет";
		}
		else {
			tmp = Start;
			while (tmp != NULL) {
				if (tmp->count == num) {
					int money;
					cout << "Сколько вы хотите поставить ($): ";
					cin >> money;
					cout << "На какую команду (1/2): ";
					cin >> num;
					if (num > 2) num = 2;
					else if (num < 1) num = 1;

					ofstream file;
					file.open(fname, ios_base::app);
					file << tmp->bet.ratio1 << " ";
					file << tmp->bet.team1 << " ";
					file << tmp->bet.team2 << " ";
					file << tmp->bet.ratio2 << " ";
					file << tmp->bet.id << " ";
					file << -money << " ";
					file << num << " ";
					file.close();
					break;
				}
				else {
					tmp = tmp->next;
				}
			}
		}
		delList(Start);
		Start = Finish = NULL;
	}
	void History() {
		makeHisList("Completed.txt");
		betList* tmp2 = Start;


		betList* Start2 = NULL;
		betList* Finish2 = NULL;
		double ratio1, ratio2;
		string team1, team2;
		int bank;
		int id, junk;
		int i = 1;
		betList* tmp = NULL;

		ifstream file;
		file.open(fname, ios_base::in);
		while (file >> ratio1) {
			file >> team1;
			file >> team2;
			file >> ratio2;
			file >> id;
			file >> bank;
			file >> junk;
			tmp = new betList;
			if (Start2 == NULL) {
				Start2 = tmp;
				tmp->next = NULL;
				tmp->count = i;
				tmp->bet.ratio1 = ratio1;
				tmp->bet.team1 = team1;
				tmp->bet.team2 = team2;
				tmp->bet.ratio2 = ratio2;
				tmp->bet.bank = bank;
				tmp->bet.id = id;
				tmp->bet.team = junk;
				tmp->prev = NULL;
				Finish2 = tmp;
			}
			else {
				Finish2->next = tmp;
				tmp->count = i;
				tmp->bet.ratio1 = ratio1;
				tmp->bet.team1 = team1;
				tmp->bet.team2 = team2;
				tmp->bet.ratio2 = ratio2;
				tmp->bet.bank = bank;
				tmp->bet.id = id;
				tmp->bet.team = junk;
				tmp->prev = Finish;
				Finish2 = tmp;
			}
			tmp = tmp->next;
			i++;
		}

		file.close();

		tmp = Start2;

		while (tmp2 != NULL) {
			while (tmp != NULL){
				if (tmp->bet.id == tmp2->bet.id && tmp->bet.team == tmp2->bet.team) {
					if (tmp->bet.team == 1) {
						tmp->bet.bank = (double)tmp->bet.bank * tmp->bet.ratio1 * (-1);
					}
					else if (tmp->bet.team == 2) {
						tmp->bet.bank = (double)(tmp->bet.bank * tmp->bet.ratio2) * (-1);
					}
					break;
				}
				tmp = tmp->next;
			}
			tmp2 = tmp2->next;
		}

		cout << "   +====================+====================+====================+====================+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
		cout << "+==+====================+====================+====================+====================+\n";

		tmp = Start2;
		while (tmp != NULL) {
			cout << "|" << tmp->count << " " << tmp->bet;
			cout << "|" << setw(86) << (int)tmp->bet.bank << "|\n";
			cout << "+==+====================+====================+====================+====================+\n";
			tmp = tmp->next;
		}

		ofstream f;
		f.open(fname, ios_base::trunc);
		tmp= Start2;
		while (tmp != NULL) {
			f << tmp->bet.ratio1 << " ";
			f << tmp->bet.team1 << " ";
			f << tmp->bet.team2 << " ";
			f << tmp->bet.ratio2 << " ";
			f << tmp->bet.id << " ";
			f << tmp->bet.bank << " ";
			f << tmp->bet.team << " ";
			tmp = tmp->next;
		}
		file.close();

		/*delList(Start);
		Start = Finish = NULL;
		delList(Start2);
		Start2 = Finish2 = NULL;*/
	}
};

User user;

ostream& operator<<(ostream& out, const Bet& bet) {
	if (user.AdminOrUser() == 1) {
		out << setprecision(3) << "|" << setw(20) << bet.ratio1 << "|" << setw(20) << bet.team1 << "|" << setw(20) << bet.team2 << "|" << setw(20) << bet.ratio2 << "|" << setw(10) << (int)bet.bank << "|" << endl;
		out << "+==+====================+====================+====================+====================+==========+" << endl;
	}
	else {
		out << setprecision(3) << "|" << setw(20) << bet.ratio1 << "|" << setw(20) << bet.team1 << "|" << setw(20) << bet.team2 << "|" << setw(20) << bet.ratio2 << "|" << endl;
		out << "+==+====================+====================+====================+====================+" << endl;
	}
	return out;
};

void auth() {
	user.Authorization();
}
void reg() {
	user.Registration();
}

void addCSGO() {
	user.AddCSGOBet();
}
void addValorant() {
	user.AddValorantBet();
}

void GetValorantBets() {
	ifstream file;
	file.open("Valorantbets.txt", ios_base::in);
	betList* tmp = new betList;
	double ratio1, ratio2;
	string team1, team2;
	int bank;
	int id;
	int i = 1;

	if (user.AdminOrUser() == 1) {
		cout << "   +====================+====================+====================+====================+==========+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |   Банк   |\n";
		cout << "+==+====================+====================+====================+====================+==========+\n";

		while ((file >> ratio1)) {
			file >> team1;
			file >> team2;
			file >> ratio2;
			file >> bank;
			file >> id;
			tmp->set(ratio1, ratio2, team1, team2, bank);
			cout << "|" << i << " " << tmp->bet;
			i++;
		}
	}
	else {
		cout << "   +====================+====================+====================+====================+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
		cout << "+==+====================+====================+====================+====================+\n";

		while ((file >> ratio1)) {
			file >> team1;
			file >> team2;
			file >> ratio2;
			file >> bank;
			file >> id;
			tmp->set(ratio1, ratio2, team1, team2, bank);
			cout << "|" << i << " " << tmp->bet;
			i++;
		}
	}

	delete tmp;
	file.close();
}
void GetCSGOBets() {
	ifstream file;
	file.open("CSGObets.txt", ios_base::in);
	betList* tmp = new betList;
	double ratio1, ratio2;
	string team1, team2;
	int bank;
	int id;
	int i = 1;

	if (user.AdminOrUser() == 1) {
		cout << "   +====================+====================+====================+====================+==========+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |   Банк   |\n";
		cout << "+==+====================+====================+====================+====================+==========+\n";

		while ((file >> ratio1)) {
			file >> team1;
			file >> team2;
			file >> ratio2;
			file >> bank;
			file >> id;
			tmp->set(ratio1, ratio2, team1, team2, bank);
			cout << "|" << i << " " << tmp->bet;
			i++;
		}
	}
	else {
		cout << "   +====================+====================+====================+====================+\n";
		cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
		cout << "+==+====================+====================+====================+====================+\n";

		while ((file >> ratio1)) {
			file >> team1;
			file >> team2;
			file >> ratio2;
			file >> bank;
			file >> id;
			tmp->set(ratio1, ratio2, team1, team2, bank);
			cout << "|" << i << " " << tmp->bet;
			i++;
		}
	}

	delete tmp;
	file.close();
}

void delCSGOBets() {
	makeBetList("CSGObets.txt");
	betList* tmp = Start;
	int num;

	cout << "   +====================+====================+====================+====================+\n";
	cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
	cout << "+==+====================+====================+====================+====================+\n";

	while (tmp != NULL) {
		cout << "|" << tmp->count << " " << tmp->bet;
		tmp = tmp->next;
	}

	cout << "\nВведите номер: ";
	cin >> num;
	if (Finish->count < num || Start->count > num) {
		system("cls");
		cout << "Такого номера нет";
	}
	else {
		tmp = Start;
		while (tmp != NULL) {
			if (tmp->count == num) {
				if (tmp->prev == NULL && tmp->next == NULL) {
					Start = Finish = NULL;
					delete tmp;
					break;
				}
				else if (tmp->prev == NULL) {
					tmp->next->prev = NULL;
					Start = tmp->next;
					delete tmp;
					break;
				}
				else if (tmp->next == NULL) {
					tmp->prev->next = NULL;
					Finish = tmp->prev;
					delete tmp;
					break;
				}
				else {
					tmp->prev->next = tmp->next;
					tmp->next->prev = tmp->prev;
					delete tmp;
					break;
				}
			}
			else {
				tmp = tmp->next;
			}
		}
	}


	ofstream file;
	file.open("CSGObets.txt", ios_base::trunc);
	tmp = Start;
	while (tmp != NULL) {
		file << tmp->bet.ratio1 << " ";
		file << tmp->bet.team1 << " ";
		file << tmp->bet.team2 << " ";
		file << tmp->bet.ratio2 << " ";
		file << tmp->bet.bank << " ";
		file << tmp->bet.id << " ";
		tmp = tmp->next;
	}

	file.close();

	tmp = Start;
	delList(Start);

	Start = Finish = NULL;
}
void delValorantBets() {
	makeBetList("Valorantbets.txt");
	betList* tmp = Start;
	int num;

	cout << "   +====================+====================+====================+====================+\n";
	cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |\n";
	cout << "+==+====================+====================+====================+====================+\n";

	while (tmp != NULL) {
		cout << "|" << tmp->count << " " << tmp->bet;
		tmp = tmp->next;
	}

	cout << "\nВведите номер: ";
	cin >> num;
	if (Finish->count < num || Start->count > num) {
		system("cls");
		cout << "Такого номера нет";
	}
	else {
		tmp = Start;
		while (tmp != NULL) {
			if (tmp->count == num) {
				if (tmp->prev == NULL && tmp->next == NULL) {
					Start = Finish = NULL;
					delete tmp;
					break;
				}
				else if (tmp->prev == NULL) {
					tmp->next->prev = NULL;
					Start = tmp->next;
					delete tmp;
					break;
				}
				else if (tmp->next == NULL) {
					tmp->prev->next = NULL;
					Finish = tmp->prev;
					delete tmp;
					break;
				}
				else {
					tmp->prev->next = tmp->next;
					tmp->next->prev = tmp->prev;
					delete tmp;
					break;
				}
			}
			else {
				tmp = tmp->next;
			}
		}
	}


	ofstream file;
	file.open("Valorantbets.txt", ios_base::trunc);
	tmp = Start;
	while (tmp != NULL) {
		file << tmp->bet.ratio1 << " ";
		file << tmp->bet.team1 << " ";
		file << tmp->bet.team2 << " ";
		file << tmp->bet.ratio2 << " ";
		file << tmp->bet.bank << " ";
		file << tmp->bet.id << " ";
		tmp = tmp->next;
	}

	file.close();

	delList(Start);

	Start = Finish = NULL;
}

void setBetMarge() {
	user.setMarge();
}
void getIncome() {
	user.income();
}

void ValorantBet() {
	user.MakeAValBet();
}
void CSbet() {
	user.MakeACSBet();
}

void history() {
	user.History();
}

void FinishCSBet() {
	makeBetList("CSGObets.txt");
	betList* tmp = Start;
	int num, team;

	cout << "   +====================+====================+====================+====================+==========+\n";
	cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |   Банк   |\n";
	cout << "+==+====================+====================+====================+====================+==========+\n";

	while (tmp != NULL) {
		cout << "|" << tmp->count << " " << tmp->bet;
		tmp = tmp->next;
	}

	cout << "\nВведите номер: ";
	cin >> num;
	cout << "Выберите выйгравшую команду: ";
	cin >> team;
	if (team > 2) team = 2;
	else if (team < 1) team = 1;

	if (Finish->count < num || Start->count > num) {
		system("cls");
		cout << "Такого номера нет";
	}
	else {
		tmp = Start;
		while (tmp != NULL) {
			if (tmp->count == num) {
				ofstream file;
				file.open("Completed.txt", ios_base::app);

				file << tmp->bet.ratio1 << " ";
				file << tmp->bet.team1 << " ";
				file << tmp->bet.team2 << " ";
				file << tmp->bet.ratio2 << " ";
				file << tmp->bet.id << " ";
				file << tmp->bet.bank << " ";
				file << team << " ";

				file.close();

				betList* tmp2 = Start;
				while (tmp2 != NULL) {
					if (tmp2->count == num) {
						if (tmp2->prev == NULL && tmp2->next == NULL) {
							Start = Finish = NULL;
							delete tmp2;
							break;
						}
						else if (tmp2->prev == NULL) {
							tmp2->next->prev = NULL;
							Start = tmp2->next;
							delete tmp2;
							break;
						}
						else if (tmp2->next == NULL) {
							tmp2->prev->next = NULL;
							Finish = tmp2->prev;
							delete tmp2;
							break;
						}
						else {
							tmp2->prev->next = tmp2->next;
							tmp2->next->prev = tmp2->prev;
							delete tmp2;
							break;
						}
					}
					else {
						tmp2 = tmp2->next;
					}
				}
				break;
			}
			else {
				tmp = tmp->next;
			}
		}
	}

	ofstream file;
	file.open("CSGObets.txt", ios_base::trunc);
	tmp = Start;
	while (tmp != NULL) {
		file << tmp->bet.ratio1 << " ";
		file << tmp->bet.team1 << " ";
		file << tmp->bet.team2 << " ";
		file << tmp->bet.ratio2 << " ";
		file << tmp->bet.bank << " ";
		file << tmp->bet.id << " ";
		tmp = tmp->next;
	}

	file.close();

	tmp = Start;
	delList(Start);

	Start = Finish = NULL;
}
void FinishValorantBet() {
	makeBetList("Valorantbets.txt");
	betList* tmp = Start;
	int num, team;

	cout << "   +====================+====================+====================+====================+==========+\n";
	cout << "   |    Коэффициент     |      Команда 1     |      Команда 2     |     Коэффициент    |   Банк   |\n";
	cout << "+==+====================+====================+====================+====================+==========+\n";

	while (tmp != NULL) {
		cout << "|" << tmp->count << " " << tmp->bet;
		tmp = tmp->next;
	}

	cout << "\nВведите номер: ";
	cin >> num;
	cout << "Выберите выйгравшую команду: ";
	cin >> team;
	if (team > 2) team = 2;
	else if (team < 1) team = 1;

	if (Finish->count < num || Start->count > num) {
		system("cls");
		cout << "Такого номера нет";
	}
	else {
		tmp = Start;
		while (tmp != NULL) {
			if (tmp->count == num) {
				ofstream file;
				file.open("Completed.txt", ios_base::app);

				file << tmp->bet.ratio1 << " ";
				file << tmp->bet.team1 << " ";
				file << tmp->bet.team2 << " ";
				file << tmp->bet.ratio2 << " ";
				file << tmp->bet.id << " ";
				file << tmp->bet.bank << " ";
				file << team << " ";

				file.close();

				betList* tmp2 = Start;
				while (tmp2 != NULL) {
					if (tmp2->count == num) {
						if (tmp2->prev == NULL && tmp2->next == NULL) {
							Start = Finish = NULL;
							delete tmp2;
							break;
						}
						else if (tmp2->prev == NULL) {
							tmp2->next->prev = NULL;
							Start = tmp2->next;
							delete tmp2;
							break;
						}
						else if (tmp2->next == NULL) {
							tmp2->prev->next = NULL;
							Finish = tmp2->prev;
							delete tmp2;
							break;
						}
						else {
							tmp2->prev->next = tmp2->next;
							tmp2->next->prev = tmp2->prev;
							delete tmp2;
							break;
						}
					}
					else {
						tmp2 = tmp2->next;
					}
				}
				break;
			}
			else {
				tmp = tmp->next;
			}
		}
	}

	ofstream file;
	file.open("Valorantbets.txt", ios_base::trunc);
	tmp = Start;
	while (tmp != NULL) {
		file << tmp->bet.ratio1 << " ";
		file << tmp->bet.team1 << " ";
		file << tmp->bet.team2 << " ";
		file << tmp->bet.ratio2 << " ";
		file << tmp->bet.bank << " ";
		file << tmp->bet.id << " ";
		tmp = tmp->next;
	}

	file.close();

	tmp = Start;
	delList(Start);

	Start = Finish = NULL;
}