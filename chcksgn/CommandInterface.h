#pragma once

//  Класс организует взамимодествие с классами обработки команды 

class CommandInterface
{
	std::unique_ptr <Command> UCommand;

public:

	bool Init(const ComParamInput& param);
	bool Exec();
};
