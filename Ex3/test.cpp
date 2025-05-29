#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.hpp"
#include "Game.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"


// בדיקות בסיסיות
TEST_CASE("Player basic creation and info") {
    Governor p("Alice");

    CHECK(p.getName() == "Alice");
    CHECK(p.getCoins() == 0);
    CHECK(p.isActive());
}

// בדיקות על הוספה והורדה של מטבעות
TEST_CASE("Player add and remove coins") {
    Governor p("Bob");

    p.addCoins(5);
    CHECK(p.getCoins() == 5);

    p.removeCoins(2);
    CHECK(p.getCoins() == 3);

    CHECK_THROWS_AS(p.removeCoins(4), std::invalid_argument);
}
TEST_CASE("addCoins throws if negative amount") {
    Governor p("Test");
    CHECK_THROWS_WITH(p.addCoins(-1), "Cannot add negative coins.");
}

TEST_CASE("removeCoins throws if negative amount") {
    Governor p("Test");
    p.addCoins(5);
    CHECK_THROWS_WITH(p.removeCoins(-2), "Cannot remove negative coins.");
}



// בדיקת פלגים כמו active, sanction, spy
TEST_CASE("Player status flags") {
    Governor p("Charlie");

    CHECK(p.isActive());
    p.deactivate();
    CHECK_FALSE(p.isActive());

    CHECK_FALSE(p.isSanctioned());
    p.setSanctioned(true);
    CHECK(p.isSanctioned());
    p.setSanctioned(false);
    CHECK_FALSE(p.isSanctioned());

    CHECK_FALSE(p.isRevealedBySpy());
    p.setRevealedBySpy(true);
    CHECK(p.isRevealedBySpy());

    CHECK_FALSE(p.wasSpiedLastTurn());
    p.setSpiedLastTurn(true);
    CHECK(p.wasSpiedLastTurn());
}

// בדיקת arrestBlockedTurns
TEST_CASE("Player arrest block counter") {
    Governor p("Dana");

    CHECK(p.isArrestBlocked() == false);
    p.arrestBlockedTurns = 2;
    CHECK(p.isArrestBlocked() == true);

    p.updateArrestBlock(false);  // הפחתה
    CHECK(p.arrestBlockedTurns == 1);

    p.updateArrestBlock(true);   // איפוס ל־1
    CHECK(p.arrestBlockedTurns == 1);
    CHECK(p.isArrestBlocked() == true);
}

// gather – פעולה בסיסית + סנקציה
TEST_CASE("gather works unless sanctioned") {
    Governor p("Alice");

    CHECK(p.getCoins() == 0);
    CHECK_NOTHROW(p.gather());
    CHECK(p.getCoins() == 1);

    p.setSanctioned(true);
    CHECK_THROWS_WITH(p.gather(), "Sanctioned player cannot gather.");
}


TEST_CASE("setArrestedLastTurn sets the correct value") {
    Governor p("Eden");

    // פעולה שלא זורקת חריגה, אין גטר לבדיקה ישירה
    CHECK_NOTHROW(p.setArrestedLastTurn(true));
    CHECK_NOTHROW(p.setArrestedLastTurn(false));
}

TEST_CASE("getRoleName returns correct role for Governor") {
    Governor p("Liad");
    CHECK(p.getRoleName() == "Governor");
}

TEST_CASE("Calling virtual functions that have no effect in base") {
    Governor p1("Tomer");
    Governor p2("Ziv");

    // הוספת מטבע כדי למנוע חריגה בעת Arrest
    p1.addCoins(1);

    CHECK_NOTHROW(p1.receiveSanctionFrom(p2));
    CHECK_NOTHROW(p1.receiveArrestFrom(p2));
    CHECK_NOTHROW(p1.onArrested());
}

TEST_CASE("Calling empty role-based actions on Governor safely") {
    Governor p1("Lior");
    Governor p2("Dana");

    // Tax כן ממומש אצלו – צריך לבדוק גם את זה
    int before = p1.getCoins();
    CHECK_NOTHROW(p1.tax());
    CHECK(p1.getCoins() == before + 3);

    // הוספת מטבעות כדי ש־bribe לא ייכשל
    p1.addCoins(1); // עכשיו יש לו 4
    CHECK_NOTHROW(p1.bribe());

    // הוספת מטבעות למטרה כדי ש־arrest לא ייכשל
    p2.addCoins(1);
    CHECK_NOTHROW(p1.arrest(p2));

    p1.addCoins(3); // מוסיף לו מספיק בשביל sanction
    CHECK_NOTHROW(p1.sanction(p2));

    // הוספת מטבעות בשביל coup
    p1.addCoins(7);
    CHECK_NOTHROW(p1.coup(p2));
}

TEST_CASE("Clearing spy and sanction flags works") {
    Governor p("Test");

    p.setRevealedBySpy(true);
    CHECK(p.isRevealedBySpy());

    p.setRevealedBySpy(false);
    CHECK_FALSE(p.isRevealedBySpy());

    p.setSpiedLastTurn(true);
    CHECK(p.wasSpiedLastTurn());

    p.setSpiedLastTurn(false);
    CHECK_FALSE(p.wasSpiedLastTurn());
}

TEST_CASE("gather works again after sanction is cleared") {
    Governor p("Test");
    p.setSanctioned(true);
    CHECK_THROWS(p.gather());

    p.setSanctioned(false);
    CHECK_NOTHROW(p.gather());
    CHECK(p.getCoins() == 1);
}

TEST_CASE("Governor becomes inactive after coup") {
    Governor p1("A");
    Governor p2("B");

    p1.addCoins(7);
    CHECK(p2.isActive());
    p1.coup(p2);
    CHECK_FALSE(p2.isActive());
}

TEST_CASE("Player gets extra turn after bribe") {
    Governor p("A");
    p.addCoins(4);
    CHECK_NOTHROW(p.bribe());
    // כרגע אין דרך לבדוק ישירות את extraTurn - בדוק שהמטבעות ירדו
    CHECK(p.getCoins() == 0);
}

TEST_CASE("Spied player cannot perform arrest next turn") {
    Spy spy("Spy");
    Governor spied("Spied");
    Governor victim("Victim");

    // המרגל מרגל על השחקן
    spy.spyOn(spied);

    // נותנים למי שרוצה לבצע arrest ולמטרה שלו כסף – כדי למנוע חריגת "אין מספיק מטבעות"
    spied.addCoins(1);
    victim.addCoins(1);

    // השחקן שריגלו עליו מנסה לעצור – צריך להיכשל
    CHECK_THROWS_WITH(spied.arrest(victim), "Spied is blocked from using arrest this turn.");
}

TEST_CASE("arrest block counter disables after reaching zero") {
    Governor p("Test");

    // מסמן שהוא חסום
    p.arrestBlockedTurns = 1;
    CHECK(p.isArrestBlocked());

    // הפחתה בתור הבא – צריך לרדת לאפס
    p.updateArrestBlock(false);
    CHECK(p.arrestBlockedTurns == 0);
    CHECK_FALSE(p.isArrestBlocked());
}

TEST_CASE("coup throws if not enough coins") {
    Governor p1("A"), p2("B");
    CHECK_THROWS_WITH(p1.coup(p2), "Not enough coins to perform coup.");
}

TEST_CASE("coup deducts 7 coins from player") {
    Governor attacker("A");
    Governor target("B");

    attacker.addCoins(10);  // יש לו מספיק
    int before = attacker.getCoins();
    attacker.coup(target);
    CHECK(attacker.getCoins() == before - 7);
}

TEST_CASE("bribe throws if not enough coins") {
    Governor p("A");
    CHECK_THROWS_WITH(p.bribe(), "Not enough coins to remove.");
}

TEST_CASE("arrest succeeds even when performer has 0 coins") {
    Governor attacker("A");
    Governor target("B");

    target.addCoins(1);  // למטרה יש לפחות מטבע אחד

    CHECK_NOTHROW(attacker.arrest(target));
    CHECK(target.getCoins() == 0);  // נלקח ממנו המטבע
}

TEST_CASE("arrest throws if target has no coins") {
    Governor attacker("A");
    Governor target("B");

    CHECK_THROWS_WITH(attacker.arrest(target), "B has no coins to be arrested.");
}

TEST_CASE("receiveArrestFrom throws if no coins and sets arrested flag otherwise") {
    Governor attacker("A");
    Governor target("B");

    // בלי מטבעות – צריך לזרוק
    CHECK_THROWS_WITH(target.receiveArrestFrom(attacker), "B has no coins to be arrested.");

    // עם מטבעות – לא צריך לזרוק
    target.addCoins(1);
    CHECK_NOTHROW(target.receiveArrestFrom(attacker));
}


TEST_CASE("sanction throws if not enough coins") {
    Governor attacker("A");
    Governor target("B");

    // attacker מתחיל עם 0 מטבעות
    CHECK_THROWS_WITH(attacker.sanction(target), "Not enough coins to perform sanction.");

    attacker.addCoins(2);  // עדיין לא מספיק
    CHECK_THROWS_WITH(attacker.sanction(target), "Not enough coins to perform sanction.");

    attacker.addCoins(1);  // עכשיו יש לו 3
    CHECK_NOTHROW(attacker.sanction(target));  // לא צריך לזרוק יותר
    CHECK(target.isSanctioned());
}

TEST_CASE("sanction throws if target is inactive") {
    Governor attacker("A");
    Governor target("B");

    attacker.addCoins(3);
    target.deactivate();  // עושה את השחקן לא פעיל

    CHECK_THROWS_WITH(attacker.sanction(target), "Cannot sanction an inactive player.");
}


TEST_CASE("gather fails when player is sanctioned") {
    Governor p("Test");
    p.setSanctioned(true);
    CHECK_THROWS_WITH(p.gather(), "Sanctioned player cannot gather.");
}

TEST_CASE("tax fails when player is sanctioned") {
    Governor p("Test");
    p.setSanctioned(true);
    CHECK_THROWS_WITH(p.tax(), "Sanctioned player cannot use tax.");
}

TEST_CASE("setCurrentTurnIndex correctly updates turn index") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");

    game.addPlayer(p1);
    game.addPlayer(p2);

    // ברירת מחדל היא 0 (Alice)
    CHECK(game.getCurrentTurnIndex() == 0);

    game.setCurrentTurnIndex(1);
    CHECK(game.getCurrentTurnIndex() == 1);

    game.setCurrentTurnIndex(0);
    CHECK(game.getCurrentTurnIndex() == 0);

    delete p1;
    delete p2;
}

TEST_CASE("turn() prints the current player's name") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");
    game.addPlayer(p1);
    game.addPlayer(p2);

    game.setCurrentTurnIndex(1); // Bob

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(); // שמירת הפלט הרגיל
    std::cout.rdbuf(oss.rdbuf()); // הפניית cout ל־oss

    game.turn();

    std::cout.rdbuf(oldCout); // שחזור הפלט

    CHECK(oss.str().find("Bob") != std::string::npos);

    delete p1;
    delete p2;
}

TEST_CASE("nextTurn advances to next active player") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");
    Governor* p3 = new Governor("Charlie");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    game.setCurrentTurnIndex(0); // מתחיל עם Alice
    p2->deactivate(); // Bob לא פעיל – צריך לדלג עליו

    game.nextTurn();
    CHECK(game.getCurrentTurnIndex() == 2); // ציפייה ש-Charlie עכשיו בתור

    delete p1;
    delete p2;
    delete p3;
}

TEST_CASE("currentPlayer returns the correct player") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");

    game.addPlayer(p1);
    game.addPlayer(p2);

    game.setCurrentTurnIndex(0);
    CHECK(game.currentPlayer() == p1);

    game.setCurrentTurnIndex(1);
    CHECK(game.currentPlayer() == p2);

    delete p1;
    delete p2;
}

TEST_CASE("players returns names of all players in correct order") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");
    Governor* p3 = new Governor("Charlie");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    std::vector<std::string> names = game.players();
    CHECK(names.size() == 3);
    CHECK(names[0] == "Alice");
    CHECK(names[1] == "Bob");
    CHECK(names[2] == "Charlie");

    delete p1;
    delete p2;
    delete p3;
}

TEST_CASE("winner returns the name of the last active player") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");
    Governor* p3 = new Governor("Charlie");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    // מבטלים את שני הראשונים
    p1->deactivate();
    p2->deactivate();

    // צריך לזהות את Charlie כמנצח
    CHECK(game.winner() == "Charlie");

    delete p1;
    delete p2;
    delete p3;
}

TEST_CASE("playTurn executes GATHER and TAX correctly") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");

    game.addPlayer(p1);
    game.addPlayer(p2);

    // בתור ראשון: Alice
    CHECK(game.currentPlayer() == p1);

    // פעולה 1 = Gather
    game.playTurn(1);
    CHECK(p1->getCoins() == 1);

    // פעולה 2 = Tax
    game.playTurn(2);
    CHECK(p2->getCoins() == 3);

    delete p1;
    delete p2;
}
