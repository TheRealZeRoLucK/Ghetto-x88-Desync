#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)

#define PAD(size) \
private: \
    std::byte PAD_NAME(__COUNTER__) [size]; \
public:

namespace utilities {
	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
	bool is_behind_smoke(vec3_t start_pos, vec3_t end_pos);
	int get_fps() noexcept;
}
