// chcksgn.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main(int argc, const char* argv[])
{
	setlocale(LC_ALL, "RUS");
	try {
		ComParamInput PL;
		PL.Init(argc, argv);
		CommandInterface CI;
		ThrowLastErrorIf(!CI.Init(PL), noinit, "Не удалось произвести инициализацию команды");
		ThrowLastErrorIf(!CI.Exec(), noexec, "Не удалось выполнить команды");
	}
	catch (SgnException & sgne)
	{
		std::cout << sgne.get_state() << " " << sgne.what();
	}
	catch (std::system_error& err) {
		std::cout << err.what();
	}
	catch (std::exception & se)
	{
		std::cout << se.what();
	}
	getchar();
	return 0;
}


