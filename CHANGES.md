# woad - Changes <!-- omit in toc -->


## 0.0.1-alpha2 - 7th October 2026

* Added **.gitattributes**, **.vimrc**, and **.vscode/settings.json**;
* Added **cmake/BuildType.cmake** and **cmake/TargetMacros.cmake**;
* Aligned **prepare_cmake.sh**, **build_cmake.sh**, **clean_cmake.sh**, **ctest_cmake.sh**, and **remove_cmake_artefacts.sh** with the current **cstring** helpers;
* Added **run_all_automated_tests.sh**, **run_all_component_tests.sh**, **run_all_examples.sh**, **run_all_performance_tests.sh**, **run_all_scratch_tests.sh**, **run_all_unit_tests.sh**, and native **.cmd** counterparts;
* Switched CI to modular **ci.yml** and **ci-cell.yml**;
* Moved examples to **examples/c/colour** and **examples/c/version** (`example.c.colour`, `example.c.version`);
* Moved unit tests to **test/unit/codes** and **test/unit/version**, and named the scratch reporter `test.scratch.versions`;
* Added **INSTALL.md**, **FAQ.md**, **HOW_YOU_CAN_HELP.md**, and **KNOWN_ISSUES.md**;


## 0.0.1-alpha1 - 16th August 2026

* SGR colour and reset codes (`WOAD_RESET`, `WOAD_FG_*`, `WOAD_BG_*`, including bright variants);


## 0.0.0 - 15th August 2026

* initial project scaffolding;


<!-- ########################### end of file ########################### -->
