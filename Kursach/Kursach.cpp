#include "Libriaries.h"
#include "Menu.h"
#include "Func.h"

int main() {
	srand(time(NULL));

	Menu UserMenu, AdminMenu, nonregMenu;
	mainMenu = &nonregMenu;

	nonregMenu.CreateMenu(2, "Войти", "Зарегистрироваться");
	{
		nonregMenu.function[0] = auth;
		nonregMenu.function[1] = reg;
	}

	UserMenu.CreateMenu(3, "Просмотреть ставки", "Сделать ставку", "История ставок");
	{
		UserMenu.sub[0].CreateMenu(2, "CS:GO", "Valorant");
		{
			UserMenu.sub[0].function[0] = GetCSGOBets;
			UserMenu.sub[0].function[1] = GetValorantBets;
		}
		UserMenu.sub[1].CreateMenu(2, "CS:GO", "Valorant");
		{
			UserMenu.sub[1].function[0] = CSbet;
			UserMenu.sub[1].function[1] = ValorantBet;
		}
		UserMenu.function[2] = history;
	}

	AdminMenu.CreateMenu(6, "Добавить ставку", "Просмотреть ставки", "Удалить ставку", "Завершить ставку", "Установить маржу", "Выручка");
	{
		AdminMenu.sub[0].CreateMenu(2, "CS:GO", "Valorant");
		{
			AdminMenu.sub[0].function[0] = addCSGO;
			AdminMenu.sub[0].function[1] = addValorant;
		}
		AdminMenu.sub[1].CreateMenu(2, "CS:GO", "Valorant");
		{
			AdminMenu.sub[1].function[0] = GetCSGOBets;
			AdminMenu.sub[1].function[1] = GetValorantBets;

		}
		AdminMenu.sub[2].CreateMenu(2, "CS:GO", "Valorant");
		{
			AdminMenu.sub[2].function[0] = delCSGOBets;
			AdminMenu.sub[2].function[1] = delValorantBets;
		}
		AdminMenu.sub[3].CreateMenu(2, "CS:GO", "Valorant");
		{
			AdminMenu.sub[3].function[0] = FinishCSBet;
			AdminMenu.sub[3].function[1] = FinishValorantBet;
		}
		AdminMenu.function[4] = setBetMarge;
		AdminMenu.function[5] = getIncome;
	}

	bool running = true;
	while (running) {
		if (user.AdminOrUser() == 2) mainMenu = &UserMenu;
		else if (user.AdminOrUser() == 1) mainMenu = &AdminMenu;
		mainMenu->ShowMenu();
		mainMenu->Navigation(&running);
	}
}