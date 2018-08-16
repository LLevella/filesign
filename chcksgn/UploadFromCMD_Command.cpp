#include "stdafx.h"

bool UploadFromCMD_Command::Init(const ComParamInput& param)
{
	int nParams = param.GetNParams();
	ThrowLastErrorIf((nParams < 2), fewparams, "Слишком мало параметров, используйте -h для справки");

	int iparam = 0;
	std::string spar = param.GetParam(0);

	double blockinMb = 1;

	if (std::regex_match(spar, std::regex("[+-]?\\d+(\\.\\d+)?")))
	{
		blockinMb = stod(spar);
		if (blockinMb == 0.0) blockinMb = 1.0;
		iparam++;
	}
	this->BlockSize = blockinMb * Mbtob;
	
	this->sfin = param.GetParam(iparam);
	ThrowLastErrorIf((!boost::filesystem::exists(this->sfin))||boost::filesystem::is_empty(this->sfin), 
		nofile, "Отсутствует или пустой входной файл");
	this->FileSize = boost::filesystem::file_size(this->sfin);
	iparam++;

	this->sfout = param.GetParam(iparam);
	boost::filesystem::path p(this->sfout);
	ThrowLastErrorIf((boost::filesystem::exists(p) && boost::filesystem::is_directory(p)), 
		fileexistsanditdirectory, "Файл для записи существует и он дирректория");

	return true;
}
bool UploadFromCMD_Command::Exec()
{
	Counter Count(this->sfin, this->sfout, this->BlockSize, this->FileSize);
	Count.CalcFileSignHT();
	return true;
}