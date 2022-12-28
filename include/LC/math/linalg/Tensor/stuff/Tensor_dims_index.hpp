
#pragma once

#include <cstddef>

#ifdef __cpp_lib_char8_t
#include <ostream>
#include <cassert>
#endif

namespace lc
{
	struct variable_size_t {}; static constexpr inline variable_size_t variable_size;
	struct undef_index_t {}; static constexpr inline undef_index_t undef_index;

	//using TensorDim = std::variant<fixed_size, variable_size>;

	struct TensorDim {

		enum class Type {
			Number,
			Variable
		};

		constexpr TensorDim() = default;
		constexpr TensorDim(TensorDim&&) = default;
		constexpr TensorDim(const TensorDim&) = default;

		constexpr TensorDim(size_t size) : m_size(size), m_type(Type::Number) {}
		constexpr TensorDim(int size) : TensorDim(size_t(size)) {}
		constexpr TensorDim(variable_size_t size) : m_type(Type::Variable) {} // TODO explicit?
		constexpr TensorDim(nullptr_t) : TensorDim(variable_size) {} // TODO explicit?
		// TODO nullopt

		constexpr TensorDim& operator=(TensorDim&&) = default;
		constexpr TensorDim& operator=(const TensorDim&) = default;

		constexpr TensorDim& operator=(size_t size) { m_size = size; m_type = Type::Number; return *this; }
		constexpr TensorDim& operator=(variable_size_t size) { m_type = Type::Variable; return *this; }
		constexpr TensorDim& operator=(nullptr_t) { return *this = variable_size; }

		constexpr bool is(Type type) const {
			return m_type == type;
		}

		constexpr bool is_variable() const {
			return this->is(Type::Variable);
		}

		constexpr bool is_fixed() const {
			return this->is(Type::Number);
		}

		constexpr size_t value() const {
			// TODO checks
			return m_size;
		}

		constexpr bool operator==(const TensorDim& other) const = default;
		constexpr bool operator!=(const TensorDim& other) const = default;

		//private: // ! <- private would not allow for using this classs as template parameter
		size_t m_size = 0;
		Type m_type = Type::Variable;
	};

	struct TensorPartialIndexValue {

		enum class Type {
			Number,
			Undefined
		};

		constexpr TensorPartialIndexValue() = default;
		constexpr TensorPartialIndexValue(TensorPartialIndexValue&&) = default;
		constexpr TensorPartialIndexValue(const TensorPartialIndexValue&) = default;

		constexpr TensorPartialIndexValue(size_t idx) : m_idx(idx), m_type(Type::Number) {}
		constexpr TensorPartialIndexValue(int idx) : TensorPartialIndexValue(size_t(idx)) {}
		constexpr TensorPartialIndexValue(undef_index_t idx) : m_type(Type::Undefined) {} // TODO explicit?
		constexpr TensorPartialIndexValue(nullptr_t) : TensorPartialIndexValue(undef_index) {} // TODO explicit?
		// TODO nullopt

		constexpr TensorPartialIndexValue& operator=(TensorPartialIndexValue&&) = default;
		constexpr TensorPartialIndexValue& operator=(const TensorPartialIndexValue&) = default;

		constexpr TensorPartialIndexValue& operator=(size_t idx) { m_idx = idx; m_type = Type::Number; return *this; }
		constexpr TensorPartialIndexValue& operator=(variable_size_t idx) { m_type = Type::Undefined; return *this; }
		constexpr TensorPartialIndexValue& operator=(nullptr_t) { return *this = undef_index; }

		constexpr bool is(Type type) const {
			return m_type == type;
		}

		constexpr bool is_undefined() const {
			return this->is(Type::Undefined);
		}

		constexpr bool is_set() const {
			return this->is(Type::Number);
		}

		constexpr size_t value() const {
			// TODO checks
			return m_idx;
		}

		constexpr bool operator==(const TensorPartialIndexValue& other) const = default;
		constexpr bool operator!=(const TensorPartialIndexValue& other) const = default;

		//private: // ! <- private would not allow for using this classs as template parameter
		size_t m_idx = 0;
		Type m_type = Type::Number;
	};
	
#ifdef __cpp_lib_char8_t
	// TODO maybe in a cpp??
	inline std::ostream& operator<<(std::ostream& os, const lc::TensorDim& dim) {
		if (dim.is_fixed())
		{
			os << dim.value();
			return os;
		}

		if (dim.is_variable())
		{
			//os << "{variable}";
			os << "~";
			return os;
		}

		assert(0);
		return os;
	}
	inline std::ostream& operator<<(std::ostream& os, const lc::TensorPartialIndexValue& dim) {
		if (dim.is_set())
		{
			os << dim.value();
			return os;
		}

		if (dim.is_undefined())
		{
			os << ".";
			return os;
		}
		return os;
	}
#endif
}

