#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <memory>

class Member
{
public:
	typedef std::shared_ptr<Member> Ptr;

public:
	Member(const std::string& specialization, int exp);

	static Ptr create(const std::string& specialization, int exp);

	int getExperience() const;
	void setExperience(int exp);
	int addExperience(int exp);

	std::string getSpecialization() const;

private:
	const std::string specialization;
	int experience;
};

#endif // MEMBER_H
