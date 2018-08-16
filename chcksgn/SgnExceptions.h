struct SgnException : public std::exception
{
public:
	SgnException(const char* msg, int dataState) : std::exception(msg), dataState(dataState)
	{};
	const int get_state()
	{
		return this->dataState;
	}
private:
	int dataState;
};

void ThrowLastErrorIf(bool expression, int expr_type, const char* msg);
