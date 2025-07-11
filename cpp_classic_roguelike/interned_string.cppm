#include "interned_string.hpp"

// Reference:
//   <https://capscollective.com/blog/siege-string-tokens/>
//   <https://en.cppreference.com/w/cpp/utility/hash.html>
//   <https://en.cppreference.com/w/cpp/container/unordered_set.html>

// std imports
import <unordered_set>;
import <utility>;

class InternedString;

template<> struct std::hash<InternedString>;

InternedString::InternedString() noexcept : _char_string(nullptr) {}
InternedString::InternedString(const char* s)
	: _char_string(find_id(s)) {}

InternedString::operator bool() const noexcept
{
	return _char_string != nullptr;
}

bool InternedString::operator==(const InternedString& rhs) const noexcept
{
	return _char_string == rhs._char_string;
}
bool InternedString::operator!=(const InternedString& rhs) const noexcept
{
	return !(*this == rhs);
}
bool InternedString::operator<(const InternedString& rhs) const noexcept
{
	return this->_char_string < rhs._char_string;
}

//const char* InternedString::operator*() const noexcept
//{
//	return _char_string;
//}

InternedString InternedString::find(const char* s)
{
	if (s == nullptr || !strlen(s)) return {};
		
	auto& interned_string_set = get_interned_string_set();
	auto it = interned_string_set.find(s);
	if (it == interned_string_set.end()) return {};

	InternedString output;
	output._char_string = *it; // If it's valid, it can be assigned.
	return output;
}

// may return null.
const char* InternedString::find_id(const char* s)
{
	return find(s)._char_string;
}

InternedString InternedString::find_or_register(const char* s)
{
	if (s == nullptr || !strlen(s)) return {};
		
	auto& interned_string_set = get_interned_string_set();
	auto it = interned_string_set.insert(s);
	if (!it.second) return {};
		
	return InternedString(*(it.first));
}

std::unordered_set<const char*>& InternedString::get_interned_string_set() noexcept
{
	static std::unordered_set<const char*> interned_string_set;
	return interned_string_set;
}

std::size_t std::hash<InternedString>::operator()(
	const InternedString& is
) const noexcept
{
	//return std::hash<const char*>()(is._char_string);
	return (size_t)(void*)(is._char_string);
}