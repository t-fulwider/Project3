/*#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <algorithm>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;

TEST_CASE("insert", "INCORRECT CMD") {

  CampusCompass c;

  bool res = c.AddStudent("1234567", "Tanner", "1", {});
  REQUIRE(!res);

  res = c.AddStudent("12345678", "Tanner12", "1", {});
  REQUIRE(!res);

  res = c.AddStudent("1234567KL", "Tanner", "1", {});
  REQUIRE(!res);

  res = c.AddStudent("12345678", "Tanner", "1", {"cop3502"});
  REQUIRE(!res);
}

TEST_CASE("drop class", "INCORRECT CMD") {

  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "1", {});

  bool res = c.DropClass("1234567", "COP3503");
  REQUIRE(!res);
}

TEST_CASE("replace class", "INCORRECT CMD") {

  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "1", {"COP3530"});

  bool res = c.ReplaceClass("1234567A", "COP3530", "COP3503");
  REQUIRE(!res);
}

TEST_CASE("insert edge cases", "Edge") {

  CampusCompass c;

  bool res = c.AddStudent("12345678", "Tanner", "1", {});
  REQUIRE(res);

  res = c.AddStudent("12345678", "Tanner F", "1", {});
  REQUIRE(!res);

}

TEST_CASE("remove edge cases", "Edge") {

  CampusCompass c;

  bool res = c.AddStudent("12345678", "Tanner", "1", {});
  REQUIRE(res);

  res = c.RemoveStudent("12345677");
  REQUIRE(!res);

}

TEST_CASE("dropClass edge cases", "Edge") {

  CampusCompass c;

  bool res = c.AddStudent("12345678", "Tanner", "1", {});
  REQUIRE(res);

  res = c.DropClass("12345678", "COP3502");
  REQUIRE(!res);

}

TEST_CASE("Test add/remove student", "FUNCTION") {

  CampusCompass c;
  c.AddStudent("12345678", "Tanner", "1", {});

  REQUIRE(c.PrintStudents()[0] == "Tanner");
  REQUIRE(c.PrintStudents().size() == 1);

  c.RemoveStudent("12345678");

  REQUIRE(c.PrintStudents().empty());
}

TEST_CASE("Test drop class", "FUNCTION") {

  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "1", vector<string>{"COP3530", "ENC3246"});

  REQUIRE(c.GetClasses("12345678").size() == 2);
  bool res = c.DropClass("12345678", "COP3530");
  REQUIRE(res);
  REQUIRE(c.GetClasses("12345678")[0] == "ENC3246");
}

TEST_CASE("Test replace class", "FUNCTION") {

  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "1", vector<string>{"COP3530", "ENC3246"});

  bool res = c.ReplaceClass("12345678", "COP3530", "ENC3246");
  REQUIRE(!res);

  res = c.ReplaceClass("12345678", "COP3530", "COP3502");
  REQUIRE(res);
  vector<string> classes = c.GetClasses("12345678");
  REQUIRE(std::find(classes.begin(), classes.end() - 1, "COP3502") != classes.end());

  REQUIRE(c.DropClass("12345678", "COP3502"));
}

TEST_CASE("Test remove class", "FUNCTION") {

  CampusCompass c;

  c.AddStudent("12345677", "Tanner", "1", vector<string>{"COP3530", "ENC3246"});
  c.AddStudent("12345678", "Tanner", "1", vector<string>{"COP3530", "ENC3246"});
  c.AddStudent("12345679", "Tanner", "1", vector<string>{"COP3530", "ENC3246"});

  REQUIRE(c.RemoveClass("COP3530") == 3);
  REQUIRE(c.GetClasses("12345678").size() == 1);
  REQUIRE(c.RemoveClass("COP3502") == 0);
}

TEST_CASE("Test djikstras", "FUNCTION"){

  CampusCompass c;

  c.AddStudent("12345678", "tanner", "21", {"COP3502", "COP3503"});

  c.ShortestEdges("12345678");
  REQUIRE(true);
}

TEST_CASE("testing shortestEdges", "FUNCTION") {

  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "21", {"PHY2048"});

  REQUIRE(c.ShortestEdges("12345678")[0].second == 22);
  cout << endl;

  pair<int, int> test = {7, 49};
  vector<pair<int, int>> testV = {test};
  c.ToggleEdges(testV);

  REQUIRE(c.ShortestEdges("12345678")[0].second == -1);
  cout << endl;
}

TEST_CASE("testing student zone", "ignore") {
// !! personal test case , not part of submission !!
  CampusCompass c;

  c.AddStudent("12345678", "Tanner", "21", {"PHY2048", "COP3502"});

  c.StudentZone("12345678");
  cout << endl;
  REQUIRE(true);
}*/