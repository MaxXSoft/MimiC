#ifndef MIMIC_MID_ANALYZER_H_
#define MIMIC_MID_ANALYZER_H_

#include <string>
#include <string_view>
#include <unordered_set>
#include <cstddef>

#include "mid/eval.h"
#include "define/ast.h"
#include "define/type.h"

#include "xstl/nested.h"
#include "xstl/guard.h"

namespace mimic::mid {

// perform semantic analysis
class Analyzer {
 public:
  Analyzer(Evaluator &eval) : eval_(eval) { Reset(); }

  void Reset() {
    auto new_env = [] {
      return xstl::MakeNestedMap<std::string, define::TypePtr>();
    };
    symbols_ = new_env();
    aliases_ = new_env();
    structs_ = new_env();
    enums_ = new_env();
    in_func_ = false;
    in_loop_ = 0;
  }

  define::TypePtr AnalyzeOn(define::VarDeclAST &ast);
  define::TypePtr AnalyzeOn(define::VarDefAST &ast);
  define::TypePtr AnalyzeOn(define::InitListAST &ast);
  define::TypePtr AnalyzeOn(define::FuncDeclAST &ast);
  define::TypePtr AnalyzeOn(define::FuncDefAST &ast);
  define::TypePtr AnalyzeOn(define::FuncParamAST &ast);
  define::TypePtr AnalyzeOn(define::StructDefAST &ast);
  define::TypePtr AnalyzeOn(define::EnumDefAST &ast);
  define::TypePtr AnalyzeOn(define::TypeAliasAST &ast);
  define::TypePtr AnalyzeOn(define::StructElemAST &ast);
  define::TypePtr AnalyzeOn(define::StructElemDefAST &ast);
  define::TypePtr AnalyzeOn(define::EnumElemAST &ast);
  define::TypePtr AnalyzeOn(define::BlockAST &ast);
  define::TypePtr AnalyzeOn(define::IfElseAST &ast);
  define::TypePtr AnalyzeOn(define::WhileAST &ast);
  define::TypePtr AnalyzeOn(define::ControlAST &ast);
  define::TypePtr AnalyzeOn(define::BinaryAST &ast);
  define::TypePtr AnalyzeOn(define::CastAST &ast);
  define::TypePtr AnalyzeOn(define::UnaryAST &ast);
  define::TypePtr AnalyzeOn(define::IndexAST &ast);
  define::TypePtr AnalyzeOn(define::FuncCallAST &ast);
  define::TypePtr AnalyzeOn(define::AccessAST &ast);
  define::TypePtr AnalyzeOn(define::IntAST &ast);
  define::TypePtr AnalyzeOn(define::CharAST &ast);
  define::TypePtr AnalyzeOn(define::StringAST &ast);
  define::TypePtr AnalyzeOn(define::IdAST &ast);
  define::TypePtr AnalyzeOn(define::PrimTypeAST &ast);
  define::TypePtr AnalyzeOn(define::StructTypeAST &ast);
  define::TypePtr AnalyzeOn(define::EnumTypeAST &ast);
  define::TypePtr AnalyzeOn(define::ConstTypeAST &ast);
  define::TypePtr AnalyzeOn(define::PointerTypeAST &ast);
  define::TypePtr AnalyzeOn(define::UserTypeAST &ast);

 private:
  // pointer of symbol table (environment)
  using EnvPtr = xstl::NestedMapPtr<std::string, define::TypePtr>;

  // switch to new environment
  xstl::Guard NewEnv();
  // handle array type
  TypePtr HandleArray(TypePtr base, const ASTPtrList &arr_lens,
                      std::string_view id, bool is_param);

  // base type of all enumerators
  static define::TypePtr enum_base_;

  // evaluator
  Evaluator &eval_;
  // symbol table, aliases, structs, enums
  EnvPtr symbols_, aliases_, structs_, enums_;
  // used when analyzing var/const declarations
  define::TypePtr var_type_;
  // used when analyzing function related stuffs
  bool in_func_;
  define::TypePtr cur_ret_;
  // used when analyzing structs
  std::string_view last_struct_name_;
  define::TypePairList struct_elems_;
  std::unordered_set<std::string_view> struct_elem_names_;
  define::TypePtr struct_elem_base_;
  // used when analyzing while loops
  std::size_t in_loop_;
};

}  // namespace mimic::mid

#endif  // MIMIC_MID_ANALYZER_H_
