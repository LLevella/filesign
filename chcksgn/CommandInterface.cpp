#include "stdafx.h"


//  Инициализация команды 
// (в случае ввода ошибочных параметров приглашение на вызов справки)

bool CommandInterface:: Init(const ComParamInput& param)
{
	if (param.HasKey(hkey)) //UCommand = new ShowHelp_Command;
		UCommand = std::unique_ptr<ShowHelp_Command>(new ShowHelp_Command);
	if (param.HasKey(skey)) //UCommand = new UploadFromCMD_Command;
		UCommand = std::unique_ptr<UploadFromCMD_Command>(new UploadFromCMD_Command);
	ThrowLastErrorIf((UCommand == NULL), nullcommand, "Используйте -h для вызова справки");
	return UCommand->Init(param);
	
}

//  Выполнение 

bool CommandInterface:: Exec()
{
	return UCommand -> Exec();
}