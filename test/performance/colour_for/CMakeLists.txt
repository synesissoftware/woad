define_performance_test_program(test.performance.colour_for main.c)

# PlatformSTL supplies the host name in the report preamble. Absent here,
# the program still builds and reports a coarse name.

if(DEFINED STLSOFT)

	set(_woad_perf_stlsoft_inc "${STLSOFT}/include")
elseif(DEFINED ENV{STLSOFT})

	set(_woad_perf_stlsoft_inc "$ENV{STLSOFT}/include")
else()

	find_package(STLSoft QUIET)
endif()

if(DEFINED _woad_perf_stlsoft_inc)

	target_include_directories(test.performance.colour_for
		PRIVATE
			"${_woad_perf_stlsoft_inc}"
	)
	target_compile_definitions(test.performance.colour_for
		PRIVATE
			WOAD_PERF_HAS_PLATFORMSTL
	)
elseif(TARGET STLSoft::STLSoft)

	target_link_libraries(test.performance.colour_for
		PRIVATE
			STLSoft::STLSoft
	)
	target_compile_definitions(test.performance.colour_for
		PRIVATE
			WOAD_PERF_HAS_PLATFORMSTL
	)
endif()
