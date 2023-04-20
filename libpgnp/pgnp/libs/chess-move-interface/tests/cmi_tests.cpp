#include "CMI.hpp"
#include <catch_amalgamated.hpp>

using namespace CMI;

#define NEW_MOVE(VAR, SAN)                                                     \
  HalfMove *(VAR) = new HalfMove();                                            \
  (VAR)->SetSAN((SAN));

HalfMove *BuildTree() {
  // Move 1
  NEW_MOVE(m1, "e4");
  NEW_MOVE(m2, "e5");
  m1->SetMainline(m2);
  // Move 2
  NEW_MOVE(m3, "Nf3");
  NEW_MOVE(m4, "Nc6");
  m2->SetMainline(m3);
  m3->SetMainline(m4);
  // Move 3 Ponziani :D
  NEW_MOVE(m5, "c3");
  NEW_MOVE(m6, "Nf6");
  m4->SetMainline(m5);
  m5->SetMainline(m6);
  // Move 4 Ponziani :D
  NEW_MOVE(m7, "d4");
  NEW_MOVE(m8, "exd4");
  m5->SetMainline(m7);
  m7->SetMainline(m8);
  // Move 4 Variation
  NEW_MOVE(m8bis, "Nxe4");
  m8->AddVariation(m8bis);
  return m1;
}

TEST_CASE("CMI Tests Numbers", "[numbers]") {
  HalfMove *m = BuildTree();
  CHECK(m->IsConsistent());

  CHECK(m->GetNumber() == 1);
  CHECK(m->GetMainline()->GetNumber() == 1);
  CHECK(m->GetMainline()->GetMainline()->GetNumber() == 2);
}

TEST_CASE("CMI Tests Consistancy", "[IsConsistent/bindorder]") {
  // In mainline
  NEW_MOVE(m1, "e4");
  NEW_MOVE(m2, "e5");
  m1->SetMainline(m2);
  NEW_MOVE(m3, "Nf3");
  NEW_MOVE(m4, "Nc6");
  m3->SetMainline(m4); // Should normally be done after next line
  m2->SetMainline(m3);
  CHECK(!m1->IsConsistent());

  // In variations
  NEW_MOVE(n1, "e4");
  NEW_MOVE(n2, "e5");
  n1->SetMainline(n2);
  NEW_MOVE(n3, "c6");
  n2->AddVariation(n3);
  n3->SetNumber(4);
  CHECK(!n1->IsConsistent());
}

TEST_CASE("CMI Tests Consistancy", "[IsConsistent/isBlack]") {
  // In mainline
  NEW_MOVE(m1, "e4");
  NEW_MOVE(m2, "e5");
  m1->SetMainline(m2);
  m2->SetIsBlack(false);
  CHECK(!m1->IsConsistent());

  // In variation
  NEW_MOVE(n1, "e4");
  NEW_MOVE(n2, "e5");
  n1->SetMainline(n2);
  NEW_MOVE(n3, "c6");
  n2->AddVariation(n3);
  n3->SetIsBlack(false);
  CHECK(!n1->IsConsistent());
}