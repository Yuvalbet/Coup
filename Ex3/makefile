# קומפיילר ודגלים
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = 

# שמות קובצי ההרצה
EXEC_MAIN = main_exec
EXEC_TEST = test_exec

# קבצי CPP של ההרצה הראשית
SOURCES = \
    main.cpp \
    Game.cpp \
    Player.cpp \
    Roles/Governor.cpp \
    Roles/Spy.cpp \
    Roles/Baron.cpp \
    Roles/General.cpp \
    Roles/Judge.cpp \
    Roles/Merchant.cpp

# קבצי CPP של בדיקות
TEST_SOURCES = \
    Test.cpp \
    Game.cpp \
    Player.cpp \
    Roles/Governor.cpp \
    Roles/Spy.cpp \
    Roles/Baron.cpp \
    Roles/General.cpp \
    Roles/Judge.cpp \
    Roles/Merchant.cpp

# ברירת מחדל
all: Main

# בניית קובץ ההרצה הראשי
$(EXEC_MAIN): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXEC_MAIN) $(LDFLAGS)

# מטרה "Main" לפי דרישות המטלה: מריצה את הקובץ המהודר
Main: $(EXEC_MAIN)
	./$(EXEC_MAIN)

# בניית והרצת הבדיקות
$(EXEC_TEST): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(EXEC_TEST) $(LDFLAGS)

test: $(EXEC_TEST)
	./$(EXEC_TEST)

# בדיקת זליגות זיכרון
valgrind: $(EXEC_MAIN)
	valgrind --leak-check=full ./$(EXEC_MAIN)

# ניקוי
clean:
	rm -f $(EXEC_MAIN) $(EXEC_TEST)
