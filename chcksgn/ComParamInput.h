#pragma once

//  ����� �������� � ������� � ���������� ��������� ������ 

class ComParamInput
{
	int argc;
	std::vector<std::string> argv;
public:
	
	ComParamInput() {
		argc = 0; 
	};
	
	~ComParamInput() {
		argc = 0;
	}

	bool Init(int argc, const char* argv[]);

	// ���������� ���������� � ��������� ������
	int GetNParams() const
	{
		if (argc > 1) return argc - 1;
		return -1;
	}

	// ������� ���������� ��������� �� ������ �� ������ 
	/*
		� ��������� ������:
		0 - ��� ���������
		1 - ����
		[2 - ������]
		3,4 - ����� �������, ��������
	*/
	//  �������� ��������� ����� ����� (�� ������� �� 0)
	std::string GetParam(int ParamIndex) const;

	// ����� �����
	bool HasKey(std::string& CommandKey) const;

	// ������ ������
	inline std::string Empty() const
	{
		return "";
	}
};

