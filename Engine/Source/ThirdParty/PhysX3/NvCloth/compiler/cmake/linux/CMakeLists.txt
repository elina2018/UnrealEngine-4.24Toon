cmake_minimum_required(VERSION 3.3)
MESSAGE(STATUS "[NvCloth]cmake/linux/CMakeList.txt")
include(../common/CMakeLists.txt)


IF(NOT DEFINED TARGET_BUILD_PLATFORM) # Not defined, default to linux
	SET(TARGET_BUILD_PLATFORM "linux")
ENDIF()

SET(PLATFORM_LIST linux)

IF (NOT ${TARGET_BUILD_PLATFORM} IN_LIST PLATFORM_LIST)
	MESSAGE(FATAL_ERROR "Invalid platform:" ${TARGET_BUILD_PLATFORM})
ENDIF()

IF (DEFINED PX_32BIT)
SET(CMAKE_CXX_FLAGS "-arch i386 -msse2 -std=c++11 -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Werror -ferror-limit=0 -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables -Wno-cast-align -Wno-conversion -Wno-missing-noreturn -Wno-missing-variable-declarations -Wno-shift-sign-overflow -Wno-covered-switch-default -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-unused-member-function -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-deprecated -Wno-non-virtual-dtor -Wno-invalid-noreturn -Wno-return-type-c-linkage -Wno-reserved-id-macro -Wno-c++98-compat-pedantic -Wno-unused-local-typedef -Wno-old-style-cast -Wno-newline-eof -Wno-unused-private-field -Wno-undefined-reinterpret-cast -Wno-invalid-offsetof -gdwarf-2 -Wno-unused-value -Wno-format-nonliteral -Wno-undef -Wno-c++2a-compat -Wno-unused-private-field -Wno-tautological-type-limit-compare")
ELSEIF()
SET(CMAKE_CXX_FLAGS "-arch x86_64 -msse2 -std=c++11 -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Werror -ferror-limit=0 -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables -Wno-cast-align -Wno-conversion -Wno-missing-noreturn -Wno-missing-variable-declarations -Wno-shift-sign-overflow -Wno-covered-switch-default -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-unused-member-function -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-deprecated -Wno-non-virtual-dtor -Wno-invalid-noreturn -Wno-return-type-c-linkage -Wno-reserved-id-macro -Wno-c++98-compat-pedantic -Wno-unused-local-typedef -Wno-old-style-cast -Wno-newline-eof -Wno-unused-private-field -Wno-undefined-reinterpret-cast -Wno-invalid-offsetof -gdwarf-2 -Wno-unused-value -Wno-format-nonliteral  -Wno-undef -Wno-c++2a-compat -Wno-unused-private-field -Wno-tautological-type-limit-compare")
ENDIF()

SET(CMAKE_SHARED_LINKER_FLAGS "")

# Build debug info for all configurations
SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -O0 -g3 -gdwarf-3")
SET(CMAKE_CXX_FLAGS_CHECKED "-std=c++11 -g3 -gdwarf-3 -O3")
SET(CMAKE_CXX_FLAGS_PROFILE "-std=c++11 -O3 -g3 -gdwarf-3")
SET(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -g3 -gdwarf-3")


#set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")

# Controls PX_NVTX for all projects on linux
SET(PHYSX_LINUX_ENABLE_NVTX 0)

# Disable cuda and dx for all projects on linux
SET(PHYSX_LINUX_COMPILE_DEFS DISABLE_COMPUTE_PHYSX)
SET(PHYSX_LINUX_DEBUG_COMPILE_DEFS _DEBUG;PX_DEBUG=1;PX_CHECKED=1;PX_NVTX=${PHYSX_MAC_ENABLE_NVTX};PX_SUPPORT_PVD=0)
SET(PHYSX_LINUX_CHECKED_COMPILE_DEFS NDEBUG;PX_CHECKED=1;PX_NVTX=${PHYSX_MAC_ENABLE_NVTX};PX_SUPPORT_PVD=0)
SET(PHYSX_LINUX_PROFILE_COMPILE_DEFS NDEBUG;PX_PROFILE=1;PX_NVTX=${PHYSX_MAC_ENABLE_NVTX};PX_SUPPORT_PVD=0)
SET(PHYSX_LINUX_RELEASE_COMPILE_DEFS NDEBUG;PX_SUPPORT_PVD=0)

SET(PX_SELECT_COMPONENTS PxFoundation)
FIND_PACKAGE(PxShared REQUIRED)

# Include the PxShared-exports.cmake file that PxShared created. There's other ways to do this, but there's a bunch of issues with it in a CI environment
#  such as - we don't want to be installing any built artifacts on this server, etc. So for now we hack it.

# Add PxShared as a dependency so that we can use project references
ADD_SUBDIRECTORY(${PXSHARED_ROOT_DIR}/src/compiler/cmake/linux "${CMAKE_CURRENT_BINARY_DIR}/pxshared_bin")

# Include all of the projects
INCLUDE(NvCloth.cmake)


