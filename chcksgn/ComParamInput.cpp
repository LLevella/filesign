#include "stdafx.h"

// хранение параметров ком.строки

bool ComParamInput::Init(int argc, const char* argv[])
{
	this->argc = argc;
	if (argc > 1)
	{
		this->argc = argc - 1;
		for (int i = 1; i < argc; i++)
		{
			this->argv.push_back(argv[i]);
		}
	}

	return true;
}

// поиск ключа

bool ComParamInput::HasKey(std::string& CommandKey) const
{
	if (this->argv.empty()) return false;

	for (int i = 0; i < this->argc; i++)
	{
		if (this->argv[i] == CommandKey) return true;
	}
	return false;
}

std::string ComParamInput::GetParam(int ParamIndex) const
{
	if (this->argv.empty()) return this->Empty();
	if (ParamIndex < 0) return this->Empty();
	if (ParamIndex > this->argc - 2) return this->Empty();
	return this->argv[ParamIndex + 1];
}