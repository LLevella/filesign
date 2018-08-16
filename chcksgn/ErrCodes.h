#pragma once


enum ErrCodes : int {
	success = 0,
	nullcommand,
	fewparams,
	noexec,
	noinit,
	nofile,
	fileexistsanditdirectory,
	nostream,
	closedstream,
	notnullbuffer,
	nullpointer,
	nullfilesize
};