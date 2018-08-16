#include "stdafx.h"

Counter::Counter(std::string& sin, std::string& sout, uintmax_t blocksize, uintmax_t filesize)
{
	this->AllDataRead = false;
	this->AllDataHash = false;
	this->BlockSize = 1 * Mbtob;
	
	this->FileSize = 0;
	ThrowLastErrorIf((filesize == 0), nullfilesize, "Нулевой размер входного файла");

	this->fin = std::shared_ptr<std::ifstream>(new std::ifstream(sin, std::ios::binary));
	this->fout = std::shared_ptr<std::ofstream>(new std::ofstream(sout, std::ios::binary));

	ThrowLastErrorIf((this->fin.get() == NULL), nostream, "Ошибка открытия входного потока");
	ThrowLastErrorIf((this->fout.get() == NULL), nostream, "Ошибка открытия результирующего потока");
	
	if (blocksize != 0) this->BlockSize = blocksize;
	if (blocksize > this->getTotalSystemMemory()) this->BlockSize = this->getTotalSystemMemory();
	
	this->FileSize = filesize;
}

bool Counter::NullBuffer(char * buffer, uintmax_t buffer_size)
{
	ThrowLastErrorIf((buffer == NULL), nullpointer, "Пустой указатель на буфер");

	for (int i = 0; i < buffer_size; i++)
		*(buffer + i) = 0;
	return true;
}

std::string Counter::HashCRC32(const char* buffer, uintmax_t bufsize) {
	boost::crc_32_type result;
	result.process_bytes(buffer, bufsize);
	return std::to_string(result.checksum());
}

bool Counter::CalcFileSign() 
{
	uintmax_t bs = this->BlockSize;
	uintmax_t fs = this->FileSize;
	uintmax_t ibs = 0;

	ThrowLastErrorIf((!this->fin.get()->is_open()), closedstream, "Входной поток закрыт");
	ThrowLastErrorIf((!this->fout.get()->is_open()), closedstream, "Выходной поток закрыт");
	std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[bs]);
	std::string hash_result;
	for(;;)
	{
		ThrowLastErrorIf((!this->NullBuffer(buffer.get(), bs)), notnullbuffer, "Непустой буфер");
		if ((fs - ibs) < bs)
		{
			this->fin.get()->read(buffer.get(), (fs - ibs));
			hash_result = this->HashCRC32(buffer.get(), (fs - ibs));
			this->fout.get()->write(hash_result.c_str(), hash_result.length());
			break;
		}
		this->fin.get()->read(buffer.get(), bs);
		hash_result = this->HashCRC32(buffer.get(), bs);
		this->fout.get()->write(hash_result.c_str(), hash_result.length());
		ibs += bs;
	}
	return true;
}

uintmax_t Counter::getTotalSystemMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}

size_t Counter::getTotalSystemThreads()
{
	size_t num_threads = std::thread::hardware_concurrency();
	if (num_threads < 3) num_threads = 3;
	return num_threads;
}

bool Counter::CalcFileSignHT()
{
	uintmax_t bs = this->BlockSize;
	uintmax_t fs = this->FileSize;
	
	size_t num_threads = this->getTotalSystemThreads();
	std::cout << "Потоков доступно: " << num_threads << std::endl;

	size_t num_writer_threads = 1; // IO - по одному потоку 
	size_t num_reader_threads = 1;
	size_t num_hasher_threads = num_threads - num_writer_threads - num_reader_threads; // остальное на хэш
	
	std::cout << "Открыто потоков\n - на чтение: " << num_reader_threads << std::endl;
	std::cout << "- на расчет хэш: " << num_hasher_threads << std::endl;
	std::cout << "- на запись: " << num_writer_threads << std::endl;
	
	size_t max_num_queue_items = this->getTotalSystemMemory() / bs; 
	size_t max_queue_htow = max_num_queue_items / 2;
	size_t max_queue_rtoh = max_num_queue_items - max_queue_htow;

	std::cout << "Доступно ячеек очереди: " << max_num_queue_items << std::endl;
	std::cout << "- длина очереди на хэш - запись " << max_queue_htow << std::endl;
	std::cout << "- длина очереди на чтение - хэш: " << max_queue_rtoh << std::endl;

	ThrowLastErrorIf((!this->fin.get()->is_open()), closedstream, "Входной поток закрыт");
	ThrowLastErrorIf((!this->fout.get()->is_open()), closedstream, "Выходной поток закрыт");

	this->FromReadToHash.InitQ(max_queue_rtoh);
	this->FromHashToWrite.InitQ(max_queue_htow);

	std::thread ReadThread(&Counter::FileReader, this);
	if (ReadThread.joinable())
		ReadThread.detach();

	std::vector <std::thread> HashThreads;
	for (int i = 0; i < num_hasher_threads; i++)
	{
		HashThreads.push_back(std::thread(&Counter::FileHasher, this));
		if (HashThreads[i].joinable())
			HashThreads[i].detach();
	}
	
	std::thread WriteThread(&Counter::FileWriter, this);
	if (WriteThread.joinable())
		WriteThread.join();

	return true;
}

bool Counter::FileReader() // процесс чтения
{
	uintmax_t bs = this->BlockSize;
	uintmax_t fs = this->FileSize;
	uintmax_t ibs = 0;

	std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[bs]);
	try {
		int i = 0;
		for (;;)
		{
			ThrowLastErrorIf((!this->NullBuffer(buffer.get(), bs)), notnullbuffer, "Непустой буфер");
			
			if ((fs - ibs) < bs)
			{
				this->fin.get()->read(buffer.get(), (fs - ibs));
				this->FromReadToHash.push(std::string(buffer.get()));
				this->AllDataRead.store(true);
				break;
			}
			this->fin.get()->read(buffer.get(), bs);
			this->FromReadToHash.push(std::string(buffer.get()));
			ibs += bs;
			i++;
		}
	}
	catch (SgnException & sgne)
	{
		std::cout << sgne.get_state() << " " << sgne.what();
	}
	catch (std::ios::ios_base::failure &siif) {
		std::cout << siif.what();
	}
	catch (std::exception &se) {
		std::cout << se.what();
	}
	catch (...)
	{
		std::cout << "Неизвестная ошибка";
	}
	return true;
}

bool Counter::FileHasher() // процесс подсчета хэша
{
	try {
		uintmax_t bs = this->BlockSize;
		std::string hash_result;
		std::string read_result;
		int i = 0;
		for (;;)
		{
			this->FromReadToHash.pop(read_result);
			hash_result = this->HashCRC32(read_result.c_str(), read_result.length());
			this->FromHashToWrite.push(hash_result);
			i++;
			if ((this->FromReadToHash.empty()) && (this->AllDataRead.load()))
			{
				this->AllDataHash.store(true);
				break;
			}

		}
	}
	catch (SgnException & sgne)
	{
		std::cout << sgne.get_state() << " " << sgne.what();
	}
	catch (std::ios::ios_base::failure &siif) {
		std::cout << siif.what();
	}
	catch (std::exception &se) {
		std::cout << se.what();
	}
	catch (...)
	{
		std::cout << "Неизвестная ошибка";
	}
	return true;
}

bool Counter::FileWriter() // процесс записи
{
	try {
		std::string str_write;
		int i = 0;
		for (;;)
		{
			this->FromHashToWrite.pop(str_write);
			this->fout.get()->write(str_write.c_str(), str_write.length());
			i++;
			if ((this->FromHashToWrite.empty()) && (this->AllDataHash.load())&& (this->AllDataRead.load())) break;
			
		}
	}
	catch (SgnException & sgne)
	{
		std::cout << sgne.get_state() << " " << sgne.what();
	}
	catch (std::ios::ios_base::failure &siif) {
		std::cout << siif.what();
	}
	catch (std::exception &se) {
		std::cout << se.what();
	}
	catch (...)
	{
		std::cout << "Неизвестная ошибка";
	}
	return true;
}