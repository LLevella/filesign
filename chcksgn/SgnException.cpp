#include "stdafx.h"

// ������� ������ ����������
void ThrowLastErrorIf(bool expression, int expr_type, const char* msg)
{
	if (expression) {
		throw SgnException(msg, expr_type);
	}
}