#pragma once

// Reference:
//   <https://capscollective.com/blog/siege-string-tokens/>
//   <https://en.cppreference.com/w/cpp/utility/hash.html>
//   <https://en.cppreference.com/w/cpp/container/unordered_set.html>

// std imports
import <unordered_set>;
import <utility>;

template<typename T> class InternedString;

template<typename T> struct std::hash<InternedString<T>>;

template<typename T> class InternedString
{
public:
	InternedString() noexcept : _char_string(nullptr) {}
	explicit InternedString(const char* s)
		: _char_string(find_id(s)) {
	}

	operator bool() const noexcept
	{
		return _char_string != nullptr;
	}
	
	bool operator==(const InternedString& rhs) const noexcept
	{
		return _char_string == rhs._char_string;
	}
	bool operator!=(const InternedString& rhs) const noexcept
	{
		return !(*this == rhs);
	}
	bool operator<(const InternedString& rhs) const noexcept
	{
		return this->_char_string < rhs._char_string;
	}

	//const char* operator*() const noexcept
	//{
	//	return _char_string;
	//}

	static InternedString find(const char* s)
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
	static const char* find_id(const char* s)
	{
		return find(s)._char_string;
	}

	static InternedString find_or_register(const char* s)
	{
		if (s == nullptr || !strlen(s)) return {};

		auto& interned_string_set = get_interned_string_set();
		auto it = interned_string_set.insert(s);
		if (!it.second) return {};

		return InternedString(*(it.first));
	}

private:
	static std::unordered_set<const char*>& get_interned_string_set() noexcept
	{
		static std::unordered_set<const char*> interned_string_set;
		return interned_string_set;
	}
	const char* _char_string;

	friend struct std::hash<InternedString>;
};

template<typename T> struct std::hash<InternedString<T>>
{
	std::size_t operator()(const InternedString<T>& is) const noexcept
	{
		//return std::hash<const char*>()(is._char_string);
		return (size_t)(void*)(is._char_string);
	}

};

#define DECLARE_INTERNED_STRING(type, name) \
	extern InternedString<type> type##_##name

#define DEFINE_INTERNED_STRING(type, name) \
	InternedString<type> type##_##name = InternedString<type>::find_or_register(#name)