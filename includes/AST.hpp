/*
 * Copyright (c) 2023, Alihan SARAÇ <saracalihan@gmail.com>
 *
 * This file is under the "MIT" license. Please read "LICENSE" file for more
 * detail.
 */

#include "../ASstdlib/Logger.hpp"
#include "../ASstdlib/StringView.hpp"
#include "../ASstdlib/Variant.hpp"
#include "../ASstdlib/Vector.hpp"
#include <iostream>
#include <stdexcept>

using namespace ASstdlib;

namespace falsy {

enum AST_TYPES {
  Program_t = 1,
  VariableDeclaration_t,
  VariableDeclarator_t,
  Identifier_t,
  Expression_t,
  Literal_t,

};

enum DECLARATION_KINDS { Let, Const, Var };

inline StringView AST_TYPE_to_SW(AST_TYPES t) {
  std::string s = "";
  switch (t) {
  case AST_TYPES::Program_t:
    s = "Program";
    break;
  case AST_TYPES::VariableDeclaration_t:
    s = "VariableDeclaration";
    break;
  case AST_TYPES::VariableDeclarator_t:
    s = "VariableDeclaraton";
    break;
  case AST_TYPES::Identifier_t:
    s = "Identifier";
    break;
  case AST_TYPES::Expression_t:
    s = "Expression";
    break;
  case AST_TYPES::Literal_t:
    s = "Literal";
    break;
  default:
    throw std::runtime_error("Invalid AST_TYPE");
  }
  return StringView(s);
}

inline StringView DECLERATION_KINKS_to_SW(DECLARATION_KINDS t) {
  std::string s = "";
  switch (t) {
  case DECLARATION_KINDS::Const:
    s = "const";
    break;
  case DECLARATION_KINDS::Let:
    s = "let";
    break;
  case DECLARATION_KINDS::Var:
    s = "var";
    break;
  default:
    throw std::runtime_error("Invalid AST_TYPE");
  }
  return StringView(s);
}

inline StringView repeat_indent(int time) {
  StringView indent = " ";
  return indent * (time * 2);
}

class ASTNode {
protected:
  AST_TYPES m_type;
  int start;
  int end;

public:
  ASTNode(AST_TYPES t) : m_type(t) {}

  virtual StringView class_name() { return StringView("ASTNode"); }
  virtual int dump(int index) {
    StringView indent = repeat_indent(index);
    LOG(indent + "[" + class_name().to_string() + "]:");
    indent += "  ";
    LOG(indent + "type: " + AST_TYPE_to_SW(m_type).to_string());
    return index;
  }
  void dump() { dump(0); }
};

class Identifier : public ASTNode {
private:
  AST_TYPES m_type = AST_TYPES::Identifier_t;
  StringView m_name;

public:
  Identifier() : ASTNode(AST_TYPES::Identifier_t) {}
  Identifier(StringView name)
      : m_name(name), ASTNode(AST_TYPES::Identifier_t) {}
  StringView class_name() override { return StringView("Identifier"); }
  int dump(int index) {
    ASTNode::dump(index);
    LOG((repeat_indent(++index) + "name: " + m_name).to_string());
    return index;
  }
};

class Expression : public ASTNode {
public:
  Expression() : ASTNode(AST_TYPES::Expression_t) {}
};

class Literal : ASTNode {
private:
  StringView m_row;
  Variant m_value;

public:
  Literal() : ASTNode(AST_TYPES::Literal_t) {
    m_value = Variant();
    m_value.set_undefined();
  }
  Literal(Variant v) : m_value(v), ASTNode(Literal_t) {}
  StringView class_name() { return StringView("Literal"); }
  int dump(int index) override {
    ASTNode::dump(index);
    LOG(repeat_indent(++index) + "value: 18");
    return index;
  }
};

class VariableDeclarator : public ASTNode {
private:
  AST_TYPES m_type = AST_TYPES::VariableDeclarator_t;
  Identifier m_id;
  Literal m_init;

public:
  VariableDeclarator() : ASTNode(AST_TYPES::VariableDeclarator_t){};
  VariableDeclarator(Identifier id)
      : m_id(id), ASTNode(AST_TYPES::VariableDeclarator_t){};
  VariableDeclarator(Identifier id, Literal init)
      : m_id(id), m_init(init), ASTNode(AST_TYPES::VariableDeclarator_t){};
  StringView class_name() override { return StringView("VariableDeclarator"); }
  int dump(int index) override {
    ASTNode::dump(index);
    StringView indent = repeat_indent(++index);
    LOG((indent + "id: "));
    m_id.dump(++index);
    LOG((indent + "init: "));
    m_init.dump(++index);
    return index;
  }
};

class VariableDeclaration : public ASTNode {
private:
  DECLARATION_KINDS m_kind;
  Vector<VariableDeclarator> m_declarations;

public:
  VariableDeclaration(DECLARATION_KINDS k)
      : m_kind(k), ASTNode(AST_TYPES::VariableDeclaration_t){};
  StringView class_name() override { return StringView("VariableDecloration"); }
  void append(VariableDeclarator var) { m_declarations.push(var); }
  int dump(int index) {
    ASTNode::dump(index);
    StringView indent = repeat_indent(index);
    LOG((indent + "kind: " + DECLERATION_KINKS_to_SW(m_kind).to_string()));
    LOG((indent + "declerations: "));

    index++;
    m_declarations.for_each([&](VariableDeclarator d) { d.dump(index); });
    return index;
    ;
  }
};

class Program : ASTNode {
private:
  Vector<ASTNode *> m_body;

public:
  Program() : ASTNode(AST_TYPES::Program_t) {}
  StringView class_name() override { return StringView("Program"); }
  void append(ASTNode *node) { m_body.push(node); }
  int dump(int index) {
    ASTNode::dump(index);
    StringView indent = repeat_indent(++index);
    LOG(indent + "body:");
    index++;

    for (int i = 0; i < m_body.size(); i++) {
      m_body[i]->dump(index);
    }
    return index;
  }
};

} // namespace falsy