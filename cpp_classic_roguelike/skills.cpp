#include "skills.hpp"

#define DEFINE_SKILL(name) DEFINE_INTERNED_STRING(SKILL, name)

DEFINE_SKILL(MINING);



uint16_t& Skills::operator[](InternedString index)
{
	return _values[index];
}


std::uint16_t Skills::get_raw_skill(InternedString skill_name)
{
	auto it = _values.find(skill_name);
	if (it == _values.end()) return 3;
	return it->second;
}
double Skills::get_skill_value(InternedString skill_name)
{
	return static_cast<double>(get_raw_skill(skill_name));
}

//std::uint16_t Skills::set_raw_skill(
//	InternedString skill_name,
//	std::uint16_t raw_value
//)
//{
//	_values[skill_name] = raw_value;
//}