DEVICE ?=

debug: build_debug/CMakeCache.txt
	cmake --build build_debug && ./build_debug/suzuki_dashboard ${DEVICE}
release: build_release/CMakeCache.txt
	cmake --build build_release && ./build_release/suzuki_dashboard ${DEVICE}
pi: build_pi/CMakeCache.txt
	cmake --build build_pi

cmake-debug:
	cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug
cmake-release:
	cmake -B build_release -DCMAKE_BUILD_TYPE=Release
cmake-pi:
	cmake -B build_pi -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=pi-toolchain.cmake -DPLATFORM=DRM

build_debug/CMakeCache.txt:
	cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug
build_release/CMakeCache.txt:
	cmake -B build_release -DCMAKE_BUILD_TYPE=Release
build_pi/CMakeCache.txt:
	cmake -B build_pi -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=pi-toolchain.cmake -DPLATFORM=DRM
