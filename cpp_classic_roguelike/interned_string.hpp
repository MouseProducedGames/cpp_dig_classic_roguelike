// Reference:
//   <https://capscollective.com/blog/siege-string-tokens/>
//   <https://en.cppreference.com/w/cpp/utility/hash.html>
//   <https://en.cppreference.com/w/cpp/container/unordered_set.html>

// std imports
import <unordered_set>;
import <utility>;

class InternedString;

template<> struct std::hash<InternedString>;

class InternedString
{
public:
	InternedString() noexcept;
	explicit InternedString(const char* s);

	operator bool() const noexcept;

	static InternedString find(const char* s);

	// may return null.
	static const char* find_id(const char* s);

	static InternedString find_or_register(const char* s);

private:
	static std::unordered_set<const char*>& get_interned_string_set() noexcept;
	const char* _char_string;

	friend struct std::hash<InternedString>;
};

template<> struct std::hash<InternedString>
{
	std::size_t operator()(const InternedString& is) const noexcept;

};

#define DECLARE_INTERNED_STRING(type, name) \
	extern InternedString type##_##name

#define DEFINE_INTERNED_STRING(type, name) \
	InternedString type##_##name(#name)