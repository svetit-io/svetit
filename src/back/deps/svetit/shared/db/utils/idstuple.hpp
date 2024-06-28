#pragma once

#include <tuple>

namespace svetit::db::utils {

template<class T, class R = void>
struct enable_if_type { typedef R type; };

template<typename T, class Enable = void>
struct IdsTuple {
	using type = std::index_sequence<0>;
	static constexpr std::tuple<std::size_t> Get() { return {0}; }
};

template<typename T>
struct IdsTuple<T, typename enable_if_type<typename T::Ids>::type> {
	using type = T::Ids;
	static constexpr auto Get() { return T::Ids::Get(); }
};

template<typename T, class Enable = void>
struct FilterIdsTuple {
	using type = std::index_sequence<>;
	static constexpr std::tuple<> Get() { return {}; }
};

template<typename T>
struct FilterIdsTuple<T, typename enable_if_type<typename T::FilterIds>::type> {
	using type = T::FilterIds;
	static constexpr auto Get() { return T::FilterIds::Get(); }
};

} // namespace svetit::db::utils
