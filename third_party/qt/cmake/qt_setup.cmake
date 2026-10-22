
find_package(Qt6 6.11 REQUIRED COMPONENTS Core Quick QuickControls2 Multimedia)

cmake_diagnostic(PUSH)
cmake_diagnostic(SET CMD_UNINITIALIZED IGNORE)
find_package(QtDocs)
cmake_diagnostic(POP)

qt_standard_project_setup(REQUIRES 6.11)