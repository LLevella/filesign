#include "stdafx.h"

bool ShowHelp_Command::Init(const ComParamInput& param)
{
	return true;
}

bool ShowHelp_Command::Exec()
{
	std::cout << "����������� �����:" << std::endl;
	std::cout << "-s [������ ������] ��������_���� ��������_����" << std::endl;
	std::cout << "-h " << std::endl;
	return true;	
}