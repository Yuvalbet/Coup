//Email: yuvali532@gmail.com 
# The C++ compiler to use for building the project
CXX = g++
# Compiler flags:
# -std=c++17: use the C++17 standard
# -Wall: enable all warnings
# -Wextra: enable extra warnings
# -g: include debug symbols for debugging
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

EXEC_MAIN = main_exec
EXEC_TEST = test_exec

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

# Default target: builds the main program
all: Main

$(EXEC_MAIN): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXEC_MAIN) $(LDFLAGS)

Main: $(EXEC_MAIN)
	./$(EXEC_MAIN)

$(EXEC_TEST): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(EXEC_TEST) $(LDFLAGS)

# Rule to run unit tests
test: $(EXEC_TEST)
	./$(EXEC_TEST)

# Rule to run Valgrind on the main executable to detect memory leaks
valgrind: $(EXEC_MAIN)
	valgrind --leak-check=full ./$(EXEC_MAIN)

# Rule to remove compiled files and executables
clean:
	rm -f $(EXEC_MAIN) $(EXEC_TEST)