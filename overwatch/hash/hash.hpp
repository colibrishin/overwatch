#ifndef HASH_H
#define HASH_H

#include <string>
#include "sha256.h"

namespace Hash {
	constexpr auto SHA256_HASH_LENGTH = 65;

	struct SHA256 {
		char data[SHA256_HASH_LENGTH];

	private:
		bool compareHash(const char* s1, const char* s2) const noexcept;
		void calcHash(const char* data, char* digest);
	public:
		SHA256() {
			clear();
		}

		void clear() {
			std::memset(this->data, '\0', sizeof(this->data));
		}

		SHA256(const char* buf) {
			clear();
			calcHash(buf, this->data);
		}

		SHA256& operator=(const SHA256& _ref) {
			std::memcpy(this->data, _ref.data, sizeof(this->data));
			return *this;
		}

		bool operator==(const SHA256& second) {
			return compareHash(this->data, second.data);
		}

		const std::string getString() const noexcept{
			return this->data;
		}
	};

	
}

#endif