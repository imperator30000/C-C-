#include "LN.h"

#include <climits>
#include <cstring>
#include <iostream>
#include <vector>
void LN::left_shift_one()
{
	bool add_bit = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		bool new_add_bit = get(i * 64 + 63);
		data[i] <<= 1ull;
		if (add_bit)
		{
			set(i * 64, 1);
		}
		add_bit = new_add_bit;
	}
	if (add_bit)
	{
		data.emplace(data.begin(), 1ull);
	}
}

void LN::right_shift_one()
{
	bool add_bit = 0;
	for (long long i = data.size(); i >= 0; i--)
	{
		if (add_bit)
			set(i * 64 + 63, 1);
		add_bit = get(i * 64);
		data[i] >>= 1ull;
	}
	kill_zeroes();
}

std::string LN::get_hex() const
{
	if (data.size() == 0)
		return "0";
	if (data[0] == 0ull)
		return "0";

	std::ostringstream hex_;

	if (isNegative)
	{
		hex_ << "-";
	}

	for (size_t i = 0; i < data.size(); i++)
	{
		std::ostringstream hexDigits;
		hexDigits << std::uppercase << std::hex << data.at(i);
		std::string hexString = hexDigits.str();
		if (i > 0 && hexString.length() < 16)
		{
			hex_ << std::string(16 - hexString.length(), '0') + hexString;
		}
		else
		{
			hex_ << std::hex << std::uppercase << data.at(i);
		}
	}

	return hex_.str();
};
int LN::compare(LN &other)
{
	if (data[0] == 0 && other.data[0] == 0)
	{
		return 0;
	}
	if (isNegative && !other.isNegative)
	{
		return -1;
	}
	if (!isNegative && other.isNegative)
	{
		return 1;
	}
	if (data.size() < other.data.size())
	{
		if (isNegative)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	if (data.size() > other.data.size())
	{
		if (!isNegative)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	int ret;
	if (isNegative)
	{
		ret = -1;
	}
	else
	{
		ret = 1;
	}
	for (int i = data.size() - 1; i >= 0; i--)
	{
		if (data[i] > other.data[i])
			return ret;
		if (data[i] < other.data[i])
			return -ret;
	}
	return 0;
}

void LN::kill_zeroes()
{
	if (data.size() == 1)
		return;
	size_t i = 0;
	bool was_zero = false;
	for (; i < data.size(); i++)
	{
		//        std::cout<<std::dec<<  data[i] << std::endl;
		//        std::cout<<std::dec<<  i << std::endl;
		//        std::cout<<"     " << std::endl;
		if (data[i] == 0ull)
			was_zero = true;
		if (data[i] != 0ull)
			break;
	}
	if (was_zero)
		data.erase(data.begin(), data.begin() + i);
}

LN::LN(long long num)
{
	if (num < 0ll)
	{
		isNegative = true;
		num = -num;
	}
	else
	{
		isNegative = false;
	}
	isNaN = false;
	data.push_back(num);
	//    std::cout << std::dec <<  data.back() << std::endl;
}

LN::LN(const char *str)
{
	if (std::strlen(str) == 0)
	{
		*this = LN(0ll);
		return;
	}
	if (str[0] == '-')
	{
		isNegative = true;
		str++;
	}
	else
	{
		isNegative = false;
	}
	size_t len = std::strlen(str);
	size_t remains = len % 16;

	unsigned long long new_data_i;
	if (remains > 0)
	{
		char subStr[remains + 1];
		std::strncpy(subStr, str, remains);
		subStr[remains] = '\0';
		new_data_i = std::stoull(subStr, nullptr, 16);
		data.push_back(new_data_i);
	}
	char subStr[17];
	for (size_t i = 0; i < len / 16; i++)
	{
		std::strncpy(subStr, str + i * 16 + remains, 16);
		subStr[16] = '\0';
		new_data_i = std::stoull(subStr, nullptr, 16);
		data.push_back(new_data_i);
	}
}

LN::LN(std::vector< uint64_t > data, bool isNegative)
{
	this->data = std::move(data);
	this->isNegative = isNegative;
}

LN::LN(std::string_view str)
{
	LN new_ln = LN(str.data());
	*this = new_ln;
}

LN::LN(const LN &other) : data(other.data), isNegative(other.isNegative), isNaN(other.isNaN)
{
	data = std::vector< uint64_t >(other.data);
}

LN::LN(LN &&other) : data(std::move(other.data)), isNegative(other.isNegative), isNaN(other.isNaN)
{
	other.data.clear();
}

bool LN::get(int ind) const
{
	int cnt = ind / 64;
	if (cnt >= data.size())
		return 0;
	ind = ind % 64;
	return (data[data.size() - 1 - cnt] >> ind) & 1;
}

void LN::set(long long ind, int val)
{
	int cnt = ind / 64;
	ind = ind % 64;
	if (val == 0)
	{
		data[data.size() - 1 - cnt] &= ~(1ull << ind);
	}
	else
	{
		data[data.size() - 1 - cnt] |= (1ull << ind);
	}
}

LN &LN::operator=(const LN &other)
{
	if (this != &other)
	{
		data = other.data;
		isNegative = other.isNegative;
		isNaN = other.isNaN;
	}
	return *this;
}

LN &LN::operator=(LN &&other)
{
	if (this != &other)
	{
		data = std::move(other.data);
		other.data.clear();
		isNegative = other.isNegative;
		isNaN = other.isNaN;
	}
	return *this;
}

bool LN::operator==(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) == 0;
}

bool LN::operator!=(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) != 0;
}

bool LN::operator<(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) < 0;
}

bool LN::operator<=(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) <= 0;
}

bool LN::operator>(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) > 0;
}

bool LN::operator>=(LN &other)
{
	if (isNaN || other.isNaN)
	{
		return 0;
	}
	return compare(other) >= 0;
}

LN &LN::operator+(LN &other)
{
	unsigned long long new_size = data.size() + 1;
	unsigned long long current_size = other.data.size();

	int current_operand = 0;
	bool first_second = true;
	if (data.size() < other.data.size())
	{
		first_second = false;
		new_size = other.data.size() + 1;
		current_size = data.size();
	}

	std::vector< uint64_t > new_data(new_size, 0);
	LN *new_ln;
	if (isNegative == other.isNegative)
	{
		new_ln = new LN(new_data, isNegative);
	}
	else
	{
		if (isNegative)
		{
			isNegative = false;
			LN &new_ln = other - *this;
			isNegative = true;
			return new_ln;
		}
		else
		{
			other.setIsNegative(false);
			LN &new_ln = *this - other;
			other.setIsNegative(true);
			return new_ln;
		}
	}
	int i = 0;
	for (; i < current_size * 64; i++)
	{
		int other_get = other.get(i);
		int this_get = get(i);
		int input = other_get + this_get + current_operand;
		//        std::cout << input << " " <<  current_operand << " " << other_get << " " << this_get << " " << i << "
		//        " << (other_get ^ this_get ^ current_operand) << std::endl;
		if (input > 1)
		{
			input = other_get ^ this_get ^ current_operand;
			current_operand = 1;
		}
		else
		{
			current_operand = 0;
		}
		new_ln->set(i, input);
		//        std::cout << new_ln->data[2]  << std::endl;
	}
	LN *first;
	if (first_second)
	{
		first = this;
	}
	else
	{
		first = &other;
	}
	for (; i < first->data.size() * 64; i++)
	{
		int first_get = first->get(i);
		int input = first_get + current_operand;
		if (input == 2)
		{
			current_operand = 1;
			input = 0;
		}
		else
		{
			current_operand = 0;
		}
		new_ln->set(i, input);
	}
	new_ln->set(i, current_operand);
	new_ln->kill_zeroes();

	return *new_ln;
}

LN LN::operator+=(LN &other)
{
	LN new_ln = *this;
	*this = *this + other;
	return *this;
}

LN LN::operator-=(LN &other)
{
	LN new_ln = *this;
	*this = *this - other;
	return *this;
}

LN LN::operator*=(LN &other)
{
	LN new_ln = *this;
	*this = *this * other;
	return *this;
}

LN LN::operator/=(LN &other)
{
	LN new_ln = *this;
	*this = *this * other;
	return *this;
}

LN LN::operator%=(LN &other)
{
	LN new_ln = *this;
	*this = *this % other;
	return *this;
}

LN &LN::operator-(LN &other)
{
	unsigned long long new_size;
	unsigned long long current_size;
	unsigned long long sub_current_size;
	LN *first;
	LN *second;
	int current_operand = 0;
	bool sign;
	if (*this < other)
	{
		sign = true;
		new_size = other.data.size();
		current_size = data.size();
		sub_current_size = other.data.size();
		first = &other;
		second = this;
	}
	else
	{
		sign = false;
		new_size = data.size();
		current_size = other.data.size();
		sub_current_size = data.size();
		first = this;
		second = &other;
	}

	int i = 0;
	//    std::cout << first->isNegative  << std::endl;
	//    std::cout << second->isNegative  << std::endl;
	if (second->isNegative && !first->isNegative)
	{
		second->setIsNegative(0);
		LN &new_ln = *first + *second;
		second->setIsNegative(1);
		new_ln.setIsNegative(sign);
		return new_ln;
	}

	std::vector< uint64_t > new_data(new_size, 0);
	LN *new_ln;
	new_ln = new LN(new_data, sign);
	for (; i < current_size * 64; i++)
	{
		int first_get = first->get(i);
		int second_get = second->get(i);
		int input = first_get - second_get + current_operand;
		if (input == -1)
		{
			current_operand = -1;
			input = 1;
		}
		else if (input == -2)
		{
			current_operand = -1;
			input = 0;
		}
		else
		{
			current_operand = 0;
		}
		new_ln->set(i, input);

		//                std::cout << first_get  << std::endl;
	}
	for (; i < sub_current_size * 64; i++)
	{
		int first_get = first->get(i);
		int input = first_get + current_operand;
		if (input == -1)
		{
			current_operand = -1;
			input = 1;
		}
		else
		{
			current_operand = 0;
		}
		new_ln->set(i, input);
	}
	new_ln->set(i, current_operand);
	new_ln->kill_zeroes();
	return *new_ln;
}

LN &LN::operator*(LN &other)
{
	bool new_sign = isNegative != other.isNegative;
	LN *new_ln = new LN(0ll);
	bool start_not_zero = false;
	for (long long i = data.size() * 64 - 1; i >= 0; i--)
	{
		if (start_not_zero)
			new_ln->left_shift_one();
		if (get(i))
		{
			*new_ln = *new_ln + other;
			start_not_zero = true;
		}
	}
	new_ln->setIsNegative(new_sign);
	return *new_ln;
}

LN LN::operator/(LN &other)
{
	if (other.data[0] == 0ull)
	{
		throw std::runtime_error("Division by zero");
	}
	if (data[0] == 0ull)
	{
		return LN(0ll);
	}
	LN low = LN(0ll);
	LN one = LN(1ll);
	LN high = *this;
	LN result;
	while (low <= high)
	{
		LN mid = low + high;
		mid.right_shift_one();
		if ((mid * other) == *this)
		{
			return mid;
		}
		else if ((mid * other) > *this)
		{
			high = mid - one;
		}
		else
		{
			low = mid + one;
			result = mid;
		}
	}

	return result;
}

LN LN::operator%(LN &other)
{
	return *this - ((*this / other) * other);
}

LN LN::operator-()
{
	LN new_ln = *this;
	new_ln.setIsNegative(!new_ln.isNegative);
	return new_ln;
}

LN LN::operator~()
{
	if (isNegative && data[0] != 0ull)
	{
		LN new_ln;
		new_ln.setIsNaN(1);
		return new_ln;
	}
	LN low = LN(0ll);
	LN one = LN(1ll);
	LN high = *this;
	if (data[0] == 0ull || *this == one)
	{
		return *this;
	}
	while (high - low > one)
	{
		LN mid = low + high;
		mid.right_shift_one();
		LN square = mid * mid;
		if (square == *this)
		{
			return mid;
		}
		if (square < *this)
		{
			low = mid;
		}
		else
		{
			high = mid;
		}
	}

	return low;
}

void LN::setIsNegative(bool isNegative)
{
	LN::isNegative = isNegative;
}

void LN::setIsNaN(bool isNaN)
{
	LN::isNaN = isNaN;
}

LN::operator bool() const
{
	return data[0] != 0ull && !isNaN;
}

LN::operator long long() const
{
	if (data.size() > 1 ||
		(data.size() == 1 && (isNegative && data[0] > static_cast< uint64_t >(LLONG_MAX) + 1ull ||
							  !isNegative && data[0] > static_cast< uint64_t >(LLONG_MAX))))
	{
		throw std::overflow_error("Value does not fit into long long");
	}
	return static_cast< long long >(data[0]);
}
