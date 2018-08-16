#pragma once

//  Класс хранения и доступа у параметрам командной строки 

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

	// количество параметров в командной строке
	int GetNParams() const
	{
		if (argc > 1) return argc - 1;
		return -1;
	}

	// возврат параметров следующих за ключом по идексу 
	/*
		в командной строке:
		0 - имя программы
		1 - ключ
		[2 - размер]
		3,4 - файлы входной, выходной
	*/
	//  Параметр следующий после ключа (по индексу от 0)
	std::string GetParam(int ParamIndex) const;

	// поиск ключа
	bool HasKey(std::string& CommandKey) const;

	// пустая строка
	inline std::string Empty() const
	{
		return "";
	}
};

