#pragma once

// local imports
#include "interned_string.hpp"

// std imports
import <cstdint>;
import <unordered_map>;

#define DECLARE_SKILL(name) DECLARE_INTERNED_STRING(SKILL, name)

DECLARE_SKILL(MINING);

class Skills {
public:
	uint16_t& operator[](InternedString index);

	std::uint16_t get_raw_skill(InternedString skill_name);
	double get_skill_value(InternedString skill_name);

	//std::uint16_t set_raw_skill(InternedString skill_name, std::uint16_t);

private:
	std::unordered_map<InternedString, std::uint16_t> _values;
};