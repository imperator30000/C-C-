#ifndef LN_H
#define LN_H

#include "return_codes.h"
#include <string_view>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class LN
{
  private:
	bool isNaN;

  public:
	void setIsNaN(bool isNaN);

	std::vector< uint64_t > data;

	void left_shift_one();

	void right_shift_one();

	bool isNegative;

	int compare(LN &other);

	void kill_zeroes();

	LN(long long num = 1ull);

	LN(std::vector< uint64_t > data, bool isNegative);

	LN(const char *str);

	LN(std::string_view str);

	LN(const LN &other);

	LN(LN &&other);

	LN &operator=(const LN &other);

	LN &operator=(LN &&other);

	bool operator==(LN &other);

	bool operator!=(LN &other);

	bool operator>(LN &other);

	bool operator<(LN &other);

	bool operator<=(LN &other);

	bool operator>=(LN &other);

	LN &operator+(LN &other);

	LN operator+=(LN &other);

	LN operator-=(LN &other);

	LN operator*=(LN &other);

	LN operator/=(LN &other);

	LN operator%=(LN &other);

	LN &operator-(LN &other);
	//    LN operator-=(LN &other);

	LN &operator*(LN &other);

	LN operator/(LN &other);

	LN operator%(LN &other);

	LN operator-();

	LN operator~();

	bool get(int ind) const;

	void set(long long ind, int val);

	std::string get_hex() const;

	friend std::ostream &operator<<(std::ostream &os, const LN &obj);

	void setIsNegative(bool isNegative);

	explicit operator bool() const;

	explicit operator long long() const;
};
#endif	  // LN_H