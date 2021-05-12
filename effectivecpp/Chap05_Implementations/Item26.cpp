#include <iostream>
#include <string>
#include <stdexcept>



/*
 *条款26：尽可能延后变量定义式的出现时间
 *Postpone variable definitions as long as possible.
 *尽可能延后定义式的真正意义在于延后这份定义直到能够给它初值实参为止，这样不仅能够避免构造和析构非必要对象，还可以避免无
 *意义的 default 构造行为。
 *
 *
 * */

void encrypt(std::string& s)
{
	s = "pa$$w0rd";
}

std::string encryptPassword(const std::string& password)
{
	if (password.length() < 8)
	{
		throw std::logic_error("Password too short");
	}

	std::string encrypted(password);
	encrypt(encrypted);

	return encrypted;
}

int main()
{
	std::string encrypt = encryptPassword("password");
	std::cout<<encrypt<<std::endl;
	return 0;
}
