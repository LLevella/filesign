// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#include <windows.h>

#include <vector>  
#include <iostream>  
#include <string>  
#include <iomanip>  
#include <stdexcept>  
#include <regex> 
#include <atomic>
#include <locale.h>  
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>


#include <boost/filesystem.hpp> // чтобы пока не лезть в std::exprimental
#include <boost/system/error_code.hpp>
#include <boost/crc.hpp> // есть отличная функция подсчета crc32

#include "keys.h"
#include "ErrCodes.h"

#include "SgnExceptions.h"

#include "SgnQueue.h"
#include "Counter.h"
#include "ComParamInput.h"
#include "Command.h"
#include "CommandInterface.h"







// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
