#pragma once

// local imports
#include "interned_string.hpp"

// std imports
import <cstdint>;
import <unordered_map>;

struct SKILL {};
typedef InternedString<SKILL> SkillName;

#define DECLARE_SKILL(name) DECLARE_INTERNED_STRING(SKILL, name)

class Skills {
public:
	uint16_t& operator[](SkillName index);

	std::uint16_t get_raw_skill(SkillName skill_name);
	double get_skill_value(SkillName skill_name);

	//std::uint16_t set_raw_skill(SkillName skill_name, std::uint16_t);

private:
	std::unordered_map<SkillName, std::uint16_t> _values;
};



DECLARE_SKILL(FIGHTING);
DECLARE_SKILL(MINING);