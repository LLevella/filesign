#pragma once

//  ����� ���������� �������������� � �������� ��������� ������� 

class CommandInterface
{
	std::unique_ptr <Command> UCommand;

public:

	bool Init(const ComParamInput& param);
	bool Exec();
};
