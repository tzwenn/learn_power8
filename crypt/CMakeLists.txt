add_executable(sha256 sha256.cpp powersha.h)

target_include_directories(sha256 PUBLIC
		"../vmx"
	)
