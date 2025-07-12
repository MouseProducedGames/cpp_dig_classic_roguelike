export module match;

// std imports
import <utility>;
import <variant>;


template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

export template <typename Val, typename... Ts>
auto match(Val&& val, Ts... ts) {
	return std::visit(overloaded{ std::forward<Ts>(ts)... }, std::forward<Val>(val));
}