#include "stdafx.h"

bool ShowHelp_Command::Init(const ComParamInput& param)
{
	return true;
}

bool ShowHelp_Command::Exec()
{
	std::cout << "Используйте ключи:" << std::endl;
	std::cout << "-s [размер блоков] исходный_файл конечный_файл" << std::endl;
	std::cout << "-h " << std::endl;
	return true;	
}