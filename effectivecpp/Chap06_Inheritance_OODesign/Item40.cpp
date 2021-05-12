#include <iostream>
#include <stdio.h>
#include <memory>
//#include <string>

#include <string.h>
#include <cstring>
using namespace std;



/*
条款40：明智而审慎地使用多重继承
virtual 继承的成本会较高：

使用 virtual 继承的那些 classes 所产生的对象往往比使用 non-virtual 继承的体积要大；
访问 virtual base classes 成员变量时，也比访问 non-virtual base classes 的成员变量速度慢；
virtual base classes 初始化由继承体系中的最低层（most derived）class 负责；
*/
class DatabaseID
{
};


// This class specifies the interface to be implemented.
class IPerson
{
public:
	virtual ~IPerson();

	virtual std::string name() const = 0;
	virtual std::string birthDate() const = 0;
};


class PersonInfo
{
public:
	explicit PersonInfo(DatabaseID pid);
	virtual ~PersonInfo();

	virtual const char* theName() const;
	virtual const char* theBirthDate() const;

private:
	virtual const char* valueDelimOpen() const;
	virtual const char* valueDelimClose() const;
};


static const int Max_Formatted_Field_Value_Length = 80;

const char* PersonInfo::valueDelimOpen() const
{
	// Default opening delimiter.
	return "[";
}

const char* PersonInfo::valueDelimClose() const
{
	// Default closing delimiter.
	return "]";
}


const char* PersonInfo::theName() const
{
	// Reserve buffer for return value;
	// Because this is static it's automatically initialized to all zeros
	static char value[Max_Formatted_Field_Value_Length];

	// Write opening delimiter.
	//局限于Windows平台
	//strcpy_s(value, Max_Formatted_Field_Value_Length, valueDelimOpen());
	strcpy(value,  valueDelimOpen());

	// Append to the string in value this object's
	// name field (being careful to avoid buffer overrun)

	// Write closing delimiter.
	//局限于Windows平台
	//strcat_s(value, Max_Formatted_Field_Value_Length, valueDelimClose());
	strcpy(value,  valueDelimOpen());


	return value;
}


// Note the use of multiple inheritance.
class CPerson : public IPerson, private PersonInfo
{
public:
	explicit CPerson(DatabaseID id) : PersonInfo(id)
	{
	}

	// Implementation of the required IPerson member functions.
	virtual std::string name() const
	{
		return PersonInfo::theName();
	}

	virtual std::string birthDate()
	{
		return PersonInfo::theBirthDate();
	}

private:
	// Redefinitions of inherited virtual delimiter functions.
	const char* valueDelimOpen() const { return ""; }
	const char* valueDelimClose() const { return ""; }
};

int main()
{
	return 0;
}