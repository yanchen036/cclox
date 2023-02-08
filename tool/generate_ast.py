import sys

EXPR_VISITOR_RETURN_TYPE = "ExprValue"
STMT_VISITOR_RETURN_TYPE = "void"


def define_ast(output_dir, base_name, types):
    path_h = output_dir + "/" + base_name.lower() + ".h"
    path_cc = output_dir + "/" + base_name.lower() + ".cc"

    type_names = list(map(lambda x: x.split(":=")[0].strip(), types))
    fields = list(map(lambda x: x.split(":=")[1].strip(), types))

    with open(path_h, "w") as file_h:
        file_h.write("#ifndef %s_H_\n" % base_name.upper())
        file_h.write("#define %s_H_\n\n" % base_name.upper())
        file_h.write("// Auto generated code, don't modify manually.\n")
        if base_name == "Expr":
            file_h.write('#include <string>\n#include "token.h"\n\n')
        else:
            file_h.write('#include <vector>\n#include "expr.h"\n\n')

        if base_name == "Expr":
            define_auxiliary(file_h, type_names)
        file_h.write("class %sVisitor;\n\n" % base_name)

        define_base_class(file_h, base_name)

        # The AST classes
        for type_name, field in zip(type_names, fields):
            define_type(file_h, base_name, type_name, field)

        define_visitor(file_h, base_name, type_names)

        file_h.write("#endif // %s_H_" % base_name.upper())

    with open(path_cc, "w") as file_cc:
        file_cc.write('#include <iostream>\n\n#include "%s.h"\n\n' % base_name.lower())

        for type_name in type_names:
            impl_type(file_cc, base_name, type_name)


aux_class = """
enum ValueType { VALSTRING, VALNUMBER, VALBOOL, VALNIL };

struct ExprValue {
  double number;
  bool boolean;
  std::string string;
  nullptr_t nil;
  ValueType type;
  ExprValue()
      : number(0), boolean(false), string(""), nil(nullptr), type(VALNIL) {}
  bool operator==(const ExprValue &obj) {
    if (this->type == VALNIL && obj.type == VALNIL)
      return true;
    if (this->type == VALBOOL && obj.type == VALBOOL)
      return this->boolean == obj.boolean;
    if (this->type == VALNUMBER && obj.type == VALNUMBER)
      return this->number == obj.number;
    if (this->type == VALSTRING && obj.type == VALSTRING)
      return this->string == obj.string;
    return false;
  }
};

"""


def define_auxiliary(f, type_names):
    f.write("enum ExprType {\n")
    for tn in type_names:
        f.write("  %s,\n" % str.upper(tn))
    f.write("};\n")

    f.write(aux_class)


def define_base_class(f, base_name):
    f.write("class %s {\npublic:\n" % base_name)
    f.write("  virtual ~%s() {};\n" % base_name)
    if base_name == "Expr":
        f.write("  virtual ExprType get_type() = 0;\n")
    f.write(
        "  virtual %s accept(%sVisitor* visitor) = 0;\n};\n\n"
        % (visitor_return_type(base_name), base_name)
    )


def define_visitor(f, base_name, type_names):
    f.write("class %sVisitor {\npublic:\n" % base_name)
    for type_name in type_names:
        f.write(
            "  virtual %s visit_%s%s(%s* %s) = 0;\n"
            % (
                visitor_return_type(base_name),
                type_name,
                base_name,
                type_name,
                type_name.lower(),
            )
        )
    f.write("};\n\n")


def define_type(f, base_name, type_name, field_list):
    fields = field_list.split(", ")
    field_types = list(map(lambda fd: fd.split(" ")[0], fields))
    field_names = list(map(lambda fd: fd.split(" ")[1], fields))

    f.write("class %s : public %s {\n" % (type_name, base_name))
    f.write("public:\n")

    # constuctor
    f.write("  %s(" % type_name)
    for i, field in enumerate(fields):
        if i == 0:
            f.write(field)
        else:
            f.write(", %s" % field)
    f.write(") : ")
    for i, fn in enumerate(field_names):
        if i == 0:
            f.write("%s(%s)" % (fn, fn))
        else:
            f.write(", %s(%s)" % (fn, fn))
    f.write(" {};\n\n")

    # deconstructor
    if type_name in ["Binary", "Grouping", "Unary"]:
        f.write("  virtual ~%s() {\n" % type_name)
        for ft, fn in zip(field_types, field_names):
            if ft == "Token":
                continue
            f.write("    delete %s;\n" % fn)
        f.write("  };\n")

    # define methods
    if base_name == "Expr":
        f.write("  virtual ExprType get_type();\n")
    f.write(
        "  virtual %s accept(%sVisitor* visitor);\n\n"
        % (visitor_return_type(base_name), base_name)
    )
    for field in fields:
        f.write("  %s;\n" % field)
    f.write("};\n\n")


def impl_type(f, base_name, type_name):

    f.write(
        "%s %s::accept(%sVisitor* visitor) {\n"
        % (visitor_return_type(base_name), type_name, base_name)
    )
    f.write("  return visitor->visit_%s%s(this);\n" % (type_name, base_name))
    f.write("};\n\n")

    if base_name == "Expr":
        f.write("ExprType %s::get_type() {\n" % type_name)
        f.write("  return %s;\n" % str.upper(type_name))
        f.write("};\n\n")


def visitor_return_type(base_name):
    if base_name == "Expr":
        return EXPR_VISITOR_RETURN_TYPE
    elif base_name == "Stmt":
        return STMT_VISITOR_RETURN_TYPE
    else:
        return "void"


def main():
    if len(sys.argv) != 2:
        print("Usage: python generate_ast.py <output directory>")
        sys.exit(64)
    output_dir = sys.argv[1]

    define_ast(
        output_dir,
        "Expr",
        [
            "Assign := Token name, Expr* value",
            "Binary := Expr* left, Token op, Expr* right",
            "Grouping := Expr* expression",
            "Unary := Token op, Expr* right",
            "PrimitiveString := std::string value",
            "PrimitiveNumber := double value",
            "PrimitiveBool := bool value",
            "PrimitiveNil := std::nullptr_t value",
            "Variable := Token name",
        ],
    )

    define_ast(
        output_dir,
        "Stmt",
        [
            "Block := std::vector<Stmt*> statements",
            "Expression := Expr* expression",
            "Print := Expr* expression",
            "Var := Token name, Expr* initializer",
        ],
    )


if __name__ == "__main__":
    main()
