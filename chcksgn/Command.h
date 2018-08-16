 #pragma once

class Command
{
protected:
	std::string sfin;
	std::string sfout;
	uintmax_t BlockSize;
	uintmax_t FileSize;
public:
	virtual bool Init(const ComParamInput& param) = 0;
	virtual bool Exec() = 0;
};
 

//  Класс - справка 
class ShowHelp_Command : public Command 
{
public:
	bool Init(const ComParamInput& param);
	bool Exec();
};


//  Класс - выполнение работы по вычислению сигнатур
class UploadFromCMD_Command : public Command
{
public:
	bool Init(const ComParamInput& param);
	bool Exec();
};