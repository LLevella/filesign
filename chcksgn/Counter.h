#pragma once

// класс подсчета сигнатур файла
class Counter
{
	//входной и выходной потоки
	std::shared_ptr<std::ifstream> fin;
	std::shared_ptr<std::ofstream> fout;

	//очереди чтение-хэш, хэш-запись
	SgnQueue<std::string> FromReadToHash;
	SgnQueue<std::string> FromHashToWrite;

	//размер блока, размер файла
	uintmax_t BlockSize;
	uintmax_t FileSize;

	//флаги данные прочитаны, хэши подсчитаны
	std::atomic<bool> AllDataRead;
	std::atomic<bool> AllDataHash;

	//вычисление значения CRC для заданного блока данных.
	std::string HashCRC32(const char* buffer, uintmax_t bufsize); 

	//объем доступной оперативной памяти
	uintmax_t getTotalSystemMemory();

	//колличество доступных тредов
	size_t getTotalSystemThreads();

	//обнуление буфера
	bool NullBuffer(char * buffer, uintmax_t buffer_size);

public:
	Counter()
	{
	}
	Counter(std::string& sin, std::string& sout, uintmax_t blocksize, uintmax_t filesize);

	//последовательная функция чтение - вычисление сигнатур - запись
	bool CalcFileSign();

	//многопоточная функция чтение - вычисление сигнатур - запись
	bool CalcFileSignHT();
	bool FileReader();
	bool FileWriter();
	bool FileHasher();
}; 

