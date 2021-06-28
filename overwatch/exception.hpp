#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
namespace Exception {
	namespace Constants {
		constexpr const unsigned short RTN_EXIT_SUCCESS = 0;
		constexpr const unsigned short RTN_EXIT_FAILED = 1;
	}

	class baseReturn {
	private:
		unsigned short value = Constants::RTN_EXIT_FAILED;
	public:
		baseReturn() { this->value = Constants::RTN_EXIT_FAILED; }
		baseReturn(const unsigned short _val) { this->value = _val; }
		baseReturn(const baseReturn& _ref) {
			this->value = _ref.value;
		}
		baseReturn& operator=(const baseReturn& _ref) {
			this->value = _ref.value;
			return *this;
		}
		const unsigned short getValue() const noexcept { return this->value; }
	};
}
#endif
