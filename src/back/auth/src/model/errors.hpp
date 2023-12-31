#pragma once

#include <stdexcept>

namespace svetit::errors {

struct SecurityRisk final : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

} // namespace svetit::errors