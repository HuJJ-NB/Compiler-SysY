#ifndef __SYNTAX__
#define __SYNTAX__

#include <lexer.h>

enum SyntaxType {
SYN_NOTYPE = 0,
SYN_CompUnit,
SYN_Decl,
SYN_ConstDecl,
SYN_BType,
SYN_ConstDef,
SYN_ConstInitArray,
SYN_ConstInitVal,
SYN_VarDecl,
SYN_VarDef,
SYN_InitArray,
SYN_InitVal,
SYN_FuncDef,
SYN_FuncType,
SYN_FuncFParams,
SYN_FuncFParamIndex,
SYN_FuncFParam,
SYN_Block,
SYN_BlockItem,
SYN_Stmt,
SYN_IFMatchedStmt,
SYN_IFUnmatchedStmt,
SYN_LVal,
SYN_Exp,
SYN_LOrExp,
SYN_LAndExp,
SYN_EqExp,
SYN_RelExp,
SYN_AddExp,
SYN_MulExp,
SYN_UnaryExp,
SYN_PrimaryExp,
SYN_Number,
SYN_FuncRParams,
SYN_UnaryOp,
SYN_ConstExp,
SYN_TOKEN,
};
typedef struct SyntaxUnit{
    int type;
    int line_no;
    struct SyntaxUnit *first_sub_node;
    struct SyntaxUnit *next;
    Token *token;
} SyntaxUnit;

int syntax(char *src);

#endif
