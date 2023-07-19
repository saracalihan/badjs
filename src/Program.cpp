/*
 * Copyright (c) 2023, Alihan SARAÇ <saracalihan@gmail.com>
 *
 * This file is under the "MIT" license. Please read "LICENSE" file for more
 * detail.
 */

#include "../ASstdlib/Logger.hpp"
#include "../includes/AST.hpp"

using namespace falsy;
;

int main(int argc, char **argv) {
  Program main = Program();
  VariableDeclaration *varDec = new VariableDeclaration(DECLARATION_KINDS::Let);
  Identifier id = Identifier("users");
  Literal init = Literal(12);

  varDec->append(VariableDeclarator(id, init));
  varDec->append(VariableDeclarator(Identifier("age"), Literal(18)));
  main.append(varDec);
  main.append(varDec);

  varDec->dump(1);
  // main.dump(1);
  return 0;
}
