#include <gtest/gtest.h>
#include "Cell.h"
#include "Spreadsheet.h"

TEST(FunctionTest_OK, SUM) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string elvart = "6.000";
    teszt->add(2, true);
    teszt->add(1, false);
    teszt->edit(1,1, "1");
    teszt->edit(2,1, "2");
    teszt->edit(3,1, "3");
    teszt->edit(1,2, "=SUM(A1:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][2]->getDisplayedValue();
    EXPECT_EQ(elvart, eredmeny);
}

TEST(FunctionTest_OK, AVG) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string elvart = "2.000";
    teszt->add(2, true);
    teszt->add(1, false);
    teszt->edit(1,1, "1");
    teszt->edit(2,1, "2");
    teszt->edit(3,1, "3");
    teszt->edit(1,2, "=AVG(A1:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][2]->getDisplayedValue();
    EXPECT_EQ(elvart, eredmeny);
}

TEST(FunctionTest_OK, MIN) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string elvart = "1.000";
    teszt->add(2, true);
    teszt->add(1, false);
    teszt->edit(1,1, "1");
    teszt->edit(2,1, "2");
    teszt->edit(3,1, "3");
    teszt->edit(1,2, "=MIN(A1:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][2]->getDisplayedValue();
    EXPECT_EQ(elvart, eredmeny);
}

TEST(FunctionTest_OK, MAX) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string elvart = "3.000";
    teszt->add(2, true);
    teszt->add(1, false);
    teszt->edit(1,1, "1");
    teszt->edit(2,1, "2");
    teszt->edit(3,1, "3");
    teszt->edit(1,2, "=MAX(A1:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][2]->getDisplayedValue();
    EXPECT_EQ(elvart, eredmeny);
}

/*--------------#NAME? HIBA TESZTELÉSE--------------*/
//a sorok korlátját lépjük túl
TEST(FunctionTest_ERROR, NAME_CASE_1) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#NAME?";
    teszt->add(2, true);
    teszt->add(2, false);
    teszt->edit(1,1, "=SUM(A2:A5)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][1]->getDisplayedValue();
    EXPECT_EQ(hiba, eredmeny);
}

//az oszlopok korlátját lépjük túl
TEST(FunctionTest_ERROR, NAME_CASE_2) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#NAME?";
    teszt->add(2, true);
    teszt->add(2, false);
    teszt->edit(1,1, "=SUM(C1:D1)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][1]->getDisplayedValue();
    EXPECT_EQ(hiba, eredmeny);
}

//mindkét irányba túllépjük a korlátot
TEST(FunctionTest_ERROR, NAME_CASE_3) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#NAME?";
    teszt->add(2, true);
    teszt->add(2, false);
    teszt->edit(1,1, "=SUM(A2:E10)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][1]->getDisplayedValue();
    EXPECT_EQ(hiba, eredmeny);
}

//kezdetben #NAME? error, bővítés után ok
TEST(FunctionTest_ERROR, NAME_CASE_4) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#NAME?";
    teszt->edit(1,1, "=SUM(A2:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny = teszt->getVector()[1][1]->getDisplayedValue();
    EXPECT_EQ(hiba, eredmeny);

    teszt->add(2, true);
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    eredmeny = teszt->getVector()[1][1]->getDisplayedValue();
    EXPECT_EQ("0.000", eredmeny);
}

/*--------------#CYCLE? HIBA TESZTELÉSE--------------*/
//egy cella saját magára hivatkozik
TEST(FunctionTest_ERROR, CYCLE_CASE_1) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#CYCLE?";
    teszt->add(2, true);
    teszt->edit(1,1, "=SUM(A1:A2)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny_1 = teszt->getVector()[1][1]->getDisplayedValue();
    std::string eredmeny_2 = teszt->getVector()[2][1]->getDisplayedValue();
    
    EXPECT_EQ(hiba, eredmeny_1);    //A1 cella #CYCLE? lesz
    EXPECT_EQ(" ", eredmeny_2);      //A2 cella üres marad
}

//két cella hivatkozik egymásra
TEST(FunctionTest_ERROR, CYCLE_CASE_2) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#CYCLE?";
    teszt->add(2, true);
    teszt->edit(1,1, "=SUM(A2:A3)");
    teszt->edit(2,1, "5");
    teszt->edit(3,1, "=SUM(A1:A2)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();
    std::string eredmeny_1 = teszt->getVector()[1][1]->getDisplayedValue();
    std::string eredmeny_2 = teszt->getVector()[3][1]->getDisplayedValue();
    
    EXPECT_EQ(hiba, eredmeny_1);    //Az A1 cella #CYCLE? lesz
    EXPECT_EQ(hiba, eredmeny_2);    //Az A3 cella #CYCLE? lesz
    EXPECT_EQ("5",  teszt->getVector()[2][1]->getValue());  //Az A2 cella marad 5
}

//swap előtt még nem, de utána saját magára hivatkozik egy cella
TEST(FunctionTest_ERROR, CYCLE_CASE_3) {
    Spreadsheet* teszt = new Spreadsheet(1,1);
    std::string hiba = "#CYCLE?";
    teszt->add(2, true);
    teszt->add(1, false);
    teszt->edit(1,1, "1");
    teszt->edit(2,1, "2");
    teszt->edit(3,1, "3");
    teszt->edit(1,2, "=SUM(A1:A3)");
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();

    EXPECT_EQ("6.000", teszt->getVector()[1][2]->getDisplayedValue());

    teszt->swap(1,1,1,2);
    teszt->evaluateFunctions();
    teszt->setDisplayedValues();

    EXPECT_EQ(hiba, teszt->getVector()[1][1]->getDisplayedValue());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}