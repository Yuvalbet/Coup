# [TODO: Hebrew comment replaced – write English version]
# The C++ compiler to use for building the project
CXX = g++
# Compiler flags:
# -std=c++17: use the C++17 standard
# -Wall: enable all warnings
# -Wextra: enable extra warnings
# -g: include debug symbols for debugging
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# [TODO: Hebrew comment replaced – write English version]
EXEC_MAIN = main_exec
EXEC_TEST = test_exec

# [TODO: Hebrew comment replaced – write English version]
SOURCES = \
    main.cpp \
    Game.cpp \
    Player.cpp \
    Roles/Governor.cpp \
    Roles/Spy.cpp \
    Roles/Baron.cpp \
    Roles/General.cpp \
    Roles/Judge.cpp \
    Roles/Merchant.cpp \
    GameGUI.cpp

# [TODO: Hebrew comment replaced – write English version]
TEST_SOURCES = \
    test.cpp \
    Game.cpp \
    Player.cpp \
    Roles/Governor.cpp \
    Roles/Spy.cpp \
    Roles/Baron.cpp \
    Roles/General.cpp \
    Roles/Judge.cpp \
    Roles/Merchant.cpp

all: Main

$(EXEC_MAIN): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXEC_MAIN) $(LDFLAGS)

Main: $(EXEC_MAIN)
	./$(EXEC_MAIN)

$(EXEC_TEST): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(EXEC_TEST) $(LDFLAGS)

# Rule to execute the compiled unit tests
test: $(EXEC_TEST)
	./$(EXEC_TEST)

# Rule to run Valgrind on the main executable to detect memory leaks
valgrind: $(EXEC_MAIN)
	valgrind --leak-check=full ./$(EXEC_MAIN)

# Cleans up all generated files to reset the build environment
clean:
	rm -f $(EXEC_MAIN) $(EXEC_TEST)