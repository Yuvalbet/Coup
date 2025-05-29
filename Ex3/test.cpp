#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.hpp"
#include "Game.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Baron.hpp"
#include "Roles/General.hpp"
#include "Roles/Judge.hpp"      
#include "Roles/Merchant.hpp"  





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

TEST_CASE("Spy action throws when spying on inactive player") {
    Spy spy("Spy");
    Governor target("Target");
    target.deactivate();

    CHECK_THROWS_WITH(spy.spyOn(target), "Cannot spy on an inactive player.");
}

TEST_CASE("Spy effect wears off after one turn if not renewed") {
    Spy spy("Spy");
    Governor target("Target");

    // Spy on the target
    spy.spyOn(target);
    CHECK(target.wasSpiedLastTurn());

    // Simulate game clearing the effect
    target.setSpiedLastTurn(false);  // נאפס ידנית

    CHECK_FALSE(target.wasSpiedLastTurn());
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

TEST_CASE("Game constructor initializes members correctly") {
    Game game;

    // נוודא שהתור הראשון הוא 0
    CHECK(game.getCurrentTurnIndex() == 0);

    // כיוון שאין גטרים ל־sanctionedLastRound ול־lastArrestedPlayer,
    // נבדוק פשוט שהפעולות שמסתמכות עליהם לא זורקות חריגות
    CHECK_NOTHROW(game.getLastActionMessage()); // לא צריך לזרוק

    // אפשר לבדוק שגם players ריק בתחילה
    CHECK(game.getPlayers().empty());
}

TEST_CASE("getPlayers returns the correct player pointers") {
    Game game;

    Governor* p1 = new Governor("Alice");
    Governor* p2 = new Governor("Bob");

    game.addPlayer(p1);
    game.addPlayer(p2);

    const std::vector<Player*>& players = game.getPlayers();

    CHECK(players.size() == 2);
    CHECK(players[0] == p1);
    CHECK(players[1] == p2);

    delete p1;
    delete p2;
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

TEST_CASE("tryBlockBribe calls Judge to block bribe") {
    Game game;
    Judge* judge = new Judge("Judge");
    Governor* briber = new Governor("Briber");
    
    briber->addCoins(4);
    game.addPlayer(judge);
    game.addPlayer(briber);

    game.setCurrentTurnIndex(1);  // Briber's turn
    game.playTurn(5);  // Bribe

    // Judge should now have chance to block
    CHECK_NOTHROW(game.tryBlockBribe(judge));

    delete judge;
    delete briber;
}

TEST_CASE("tryBlockTax calls Governor to block tax") {
    Game game;
    Governor* blocker = new Governor("Blocker");
    Governor* taxer = new Governor("Taxer");

    game.addPlayer(blocker);
    game.addPlayer(taxer);

    taxer->addCoins(1);
    game.setCurrentTurnIndex(1);
    game.playTurn(2);  // Tax

    CHECK_NOTHROW(game.tryBlockTax(blocker));

    delete blocker;
    delete taxer;
}

TEST_CASE("tryBlockCoup returns false if no block is performed") {
    Game game;
    Governor* p1 = new Governor("Attacker");
    Governor* p2 = new Governor("Victim");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK(game.tryBlockCoup(p1, p2) == false);

    delete p1;
    delete p2;
}

TEST_CASE("tryBlockCoup returns true if General is active and has enough coins") {
    Game game;

    Governor* attacker = new Governor("Attacker");
    Governor* victim = new Governor("Victim");
    General* general = new General("General");

    attacker->addCoins(7); // כדי לאפשר ניסיון coup
    general->addCoins(5);  // לגנרל יש מספיק כדי לחסום

    game.addPlayer(attacker);
    game.addPlayer(victim);
    game.addPlayer(general);

    CHECK(game.tryBlockCoup(attacker, victim) == true);

    delete attacker;
    delete victim;
    delete general;
}

TEST_CASE("getValidTargets returns only other active players") {
    Game game;

    Governor* p1 = new Governor("P1");
    Governor* p2 = new Governor("P2");
    Governor* p3 = new Governor("P3");
    p2->deactivate();  // לא פעיל

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    std::vector<Player*> targets = game.getValidTargets(p1);
    CHECK(targets.size() == 1);
    CHECK(targets[0]->getName() == "P3");

    delete p1;
    delete p2;
    delete p3;
}

TEST_CASE("assignRandomRoles assigns players with correct names") {
    Game game;
    std::vector<std::string> names = {"A", "B", "C"};
    game.assignRandomRoles(names);

    std::vector<std::string> players = game.players();
    CHECK(players.size() == 3);
    CHECK(players[0] == "A");
    CHECK(players[1] == "B");
    CHECK(players[2] == "C");
}

TEST_CASE("getLastActionMessage returns the correct message") {
    Game game;
    game.setLastActionMessage("Hello world");
    CHECK(game.getLastActionMessage() == "Hello world");
}

TEST_CASE("addExtraTurns allows extra playTurn without advancing") {
    Game game;

    Governor* p1 = new Governor("A");
    Governor* p2 = new Governor("B");

    game.addPlayer(p1);
    game.addPlayer(p2);

    p1->addCoins(4);
    game.setCurrentTurnIndex(0);

    game.playTurn(5);  // Bribe
    game.addExtraTurns(1);

    // עדיין תור של p1
    CHECK(game.currentPlayer() == p1);

    delete p1;
    delete p2;
}

TEST_CASE("Governor blocks tax and cancels all coins gained from it") {
    Game game;
    Governor* gov = new Governor("Governor");
    Spy* spy = new Spy("Spy");  // סתם תפקיד רגיל

    game.addPlayer(gov);
    game.addPlayer(spy);

    spy->tax();
    CHECK(spy->getCoins() == 2);

    gov->blockTax(*spy);
    CHECK(spy->getCoins() == 0);

    delete gov;
    delete spy;
}

TEST_CASE("Judge role name is correct") {
    Judge j("Judgey");
    CHECK(j.getRoleName() == "Judge");
}

TEST_CASE("Judge blockBribe returns true and prints message") {
    Judge j("Judgey");
    Governor g("Briber");

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());  // הפניית cout

    bool result = j.blockBribe(g);

    std::cout.rdbuf(oldCout);  // החזרת הפלט

    CHECK(result == true);
    CHECK(oss.str().find("Briber's bribe was blocked by the Judge!") != std::string::npos);
}

TEST_CASE("Judge receives sanction and attacker loses 1 coin") {
    Judge j("Judgey");
    Governor attacker("Gov");

    attacker.addCoins(2);
    j.receiveSanctionFrom(attacker);

    CHECK(attacker.getCoins() == 1);
}

TEST_CASE("Baron invests and gains coins") {
    Baron b("Investor");
    b.addCoins(3);  // יש לו בדיוק 3

    CHECK_NOTHROW(b.invest());  // הפעולה לא זורקת חריגה
    CHECK(b.getCoins() == 6);   // ירד 3, עלה 6 → נטו 3
}

TEST_CASE("Baron invest throws when not enough coins") {
    Baron b("PoorBaron");
    b.addCoins(2);  // פחות משלוש

    CHECK_THROWS_WITH(b.invest(), "Not enough coins to invest.");
}

TEST_CASE("Baron receives coin when sanctioned") {
    Baron b("Barony");
    Governor attacker("Attacker");

    int before = b.getCoins();
    b.receiveSanctionFrom(attacker);
    CHECK(b.getCoins() == before + 1);
}


TEST_CASE("General blocks coup when has enough coins") {
    General g("Gabi");
    Governor victim("Vicky");
    Governor attacker("Attacker");

    g.addCoins(5);
    CHECK_NOTHROW(g.blockCoup(victim, attacker));
    CHECK(g.getCoins() == 0);  // הוריד 5 מטבעות
}

TEST_CASE("General blockCoup throws if not enough coins") {
    General g("Gabi");
    Governor victim("Vicky");
    Governor attacker("Attacker");

    g.addCoins(4);  // לא מספיק
    CHECK_THROWS_WITH(g.blockCoup(victim, attacker), "General doesn't have enough coins to block.");
}

TEST_CASE("General receiveArrestFrom sets arrested flag and prints") {
    General g("Gabi");
    Governor attacker("Attacker");

    CHECK_NOTHROW(g.receiveArrestFrom(attacker));
    // אין getter ל־arrested, אבל אין חריגה ואפשר להסתמך על הדפסת הודעה
}

TEST_CASE("Merchant gets bonus coin at start of turn if has 3 or more") {
    Merchant m("Maya");
    m.addCoins(3);  // תנאי להפעלת הבונוס

    m.onStartTurn();  // בונוס אמור להתקבל
    CHECK(m.getCoins() == 4);  // קיבל בונוס 1
}

TEST_CASE("Merchant does not get bonus coin if has less than 3") {
    Merchant m("Maya");
    m.addCoins(2);  // פחות מ־3

    m.onStartTurn();  // אין בונוס
    CHECK(m.getCoins() == 2);  // לא השתנה
}

TEST_CASE("Merchant arrested with enough coins pays 2 and gets arrested") {
    Merchant m("Maya");
    Governor attacker("Gov");

    m.addCoins(3);
    CHECK_NOTHROW(m.receiveArrestFrom(attacker));
    CHECK(m.getCoins() == 1);  // ירדו 2
}

TEST_CASE("Merchant arrest throws if less than 2 coins") {
    Merchant m("Maya");
    Governor attacker("Gov");

    m.addCoins(1);
    CHECK_THROWS_WITH(m.receiveArrestFrom(attacker), "Maya does not have 2 coins to be arrested.");
}