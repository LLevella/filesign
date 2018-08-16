#pragma once

// ����� �������� �������� �����
class Counter
{
	//������� � �������� ������
	std::shared_ptr<std::ifstream> fin;
	std::shared_ptr<std::ofstream> fout;

	//������� ������-���, ���-������
	SgnQueue<std::string> FromReadToHash;
	SgnQueue<std::string> FromHashToWrite;

	//������ �����, ������ �����
	uintmax_t BlockSize;
	uintmax_t FileSize;

	//����� ������ ���������, ���� ����������
	std::atomic<bool> AllDataRead;
	std::atomic<bool> AllDataHash;

	//���������� �������� CRC ��� ��������� ����� ������.
	std::string HashCRC32(const char* buffer, uintmax_t bufsize); 

	//����� ��������� ����������� ������
	uintmax_t getTotalSystemMemory();

	//����������� ��������� ������
	size_t getTotalSystemThreads();

	//��������� ������
	bool NullBuffer(char * buffer, uintmax_t buffer_size);

public:
	Counter()
	{
	}
	Counter(std::string& sin, std::string& sout, uintmax_t blocksize, uintmax_t filesize);

	//���������������� ������� ������ - ���������� �������� - ������
	bool CalcFileSign();

	//������������� ������� ������ - ���������� �������� - ������
	bool CalcFileSignHT();
	bool FileReader();
	bool FileWriter();
	bool FileHasher();
}; 

