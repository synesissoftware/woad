macro(define_target_compile_options target_name)

	target_compile_options(${target_name}
		PRIVATE
			$<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
				-Werror -Wall -Wextra -Wpedantic
			>
			$<$<C_COMPILER_ID:MSVC>:
				/WX /W4
			>
	)

	set_target_properties(${target_name}
		PROPERTIES
			C_STANDARD 11
			C_STANDARD_REQUIRED ON
			C_EXTENSIONS OFF
	)
endmacro(define_target_compile_options)


function(define_automated_test_program program_name entry_point_source_name)

	add_executable(${program_name}
		${entry_point_source_name}
	)

	target_link_libraries(${program_name}
		PRIVATE
			woad::woad
	)

	define_target_compile_options(${program_name})

	add_test(
		NAME ${program_name}
		COMMAND ${program_name}
	)

	set_tests_properties(${program_name}
		PROPERTIES
			TIMEOUT 30
	)
endfunction(define_automated_test_program)


function(define_example_program program_name entry_point_source_name)

	add_executable(${program_name}
		${entry_point_source_name}
	)

	target_link_libraries(${program_name}
		PRIVATE
			woad::woad
	)

	define_target_compile_options(${program_name})
endfunction(define_example_program)


# Performance programs are built for human attention (see test-standards).
# They are discovered by run_all_performance_tests.sh and must NOT be
# registered with CTest (unlike define_automated_test_program).

function(define_performance_test_program program_name entry_point_source_name)

	define_example_program(${program_name} ${entry_point_source_name})
endfunction(define_performance_test_program)


# ############################## end of file ############################# #
