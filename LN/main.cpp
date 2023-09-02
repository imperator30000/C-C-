#include "LN.h"
#include "return_codes.h"
#include <string_view>

#include <iomanip>
#include <vector>

LN operator""_ln(const char *str, std::size_t size)
{
	return LN(std::string_view(str, size));
}

std::ostream &operator<<(std::ostream &os, const LN &obj)
{
	os << obj.get_hex();
	return os;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Error: cnt args != 2 \n");
		return ERROR_PARAMETER_INVALID;
	}
	std::ifstream input(argv[1]);

	if (!input.is_open())
	{
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		return ERROR_CANNOT_OPEN_FILE;
	}
	std::vector< LN > stack = std::vector< LN >();
	std::string line;
	while (std::getline(input, line))
	{
		if (line == "+")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			stack.push_back(right + left);
		}
		else if (line == "-")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			stack.push_back(right - left);
		}
		else if (line == "*")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			stack.push_back(right * left);
		}
		else if (line == "/")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			stack.push_back(right / left);
		}
		else if (line == "%")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			stack.push_back(right % left);
		}
		else if (line == "~")
		{
			LN right = stack.back();
			stack.pop_back();
			stack.push_back(~right);
		}
		else if (line == "_")
		{
			LN right = stack.back();
			stack.pop_back();
			stack.push_back(-right);
		}
		else if (line == "<")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right < left));
			stack.push_back(ret);
		}
		else if (line == "<=")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right <= left));
			stack.push_back(ret);
		}
		else if (line == ">")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right > left));
			stack.push_back(ret);
		}
		else if (line == ">=")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right >= left));
			stack.push_back(ret);
		}
		else if (line == "==")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right == left));
			stack.push_back(ret);
		}
		else if (line == "!=")
		{
			LN right = stack.back();
			stack.pop_back();
			LN left = stack.back();
			stack.pop_back();
			LN ret = LN(static_cast< long long >(right != left));
			stack.push_back(ret);
		}
		else
		{
			LN new_ln = LN(line);
			stack.push_back(new_ln);
		}
	}
	input.close();

	std::ofstream output(argv[2]);
	if (!output.is_open())
	{
		fprintf(stderr, "cannot open file %s\n", argv[2]);
		return ERROR_CANNOT_OPEN_FILE;
	}
	while (!stack.empty())
	{
		output << stack.back() << std::endl;
		stack.pop_back();
	}
	output.close();

	LN l = "4"_ln;
	LN l2 = "7"_ln;
	l2 %= l;
	return 0;
}
