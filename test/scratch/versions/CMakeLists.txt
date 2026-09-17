add_executable(versions
	main.c
)

target_link_libraries(versions
	PRIVATE
		woad::woad
)

target_compile_options(versions
	PRIVATE
		${WOAD_COMPILE_OPTIONS}
)

set_target_properties(versions
	PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
)
