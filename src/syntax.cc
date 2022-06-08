#include <stdio.h>
#include <stdlib.h>
#include <syntax.h>
#include <lexer.h>

#define STORE_P \
    Token_stack_node *store; \
    do { \
        preget_next_token(); \
        store = tokens.tokens_p; \
    } while(0)

#define RESTORE_P \
    do { \
        tokens.tokens_p = store; \
    } while (0)

typedef struct Token_stack_node {
    Token *token;
    struct Token_stack_node *next;
}Token_stack_node;

typedef struct {
    Token_stack_node *tokens_head, *tokens_p, *tokens_tail;
}Token_stack;
static Token_stack tokens = {NULL, NULL, NULL};

static inline bool preget_next_token() {
    if (tokens.tokens_p == NULL) {
        Token_stack_node * new_node = (Token_stack_node *)malloc(sizeof(Token_stack_node));
        bool is_EOF = false;
        do {
            new_node->token = make_token(&is_EOF);
        } while (!is_EOF && (new_node->token == NULL || new_node->token->type == TK_SNT || new_node->token->type == TK_MNT));
        if (is_EOF) {
            free(new_node);
            return true;
        }
        new_node->next = NULL;
        if (tokens.tokens_head == NULL) {
            tokens.tokens_head = new_node;
        }
        else {
            tokens.tokens_tail->next = new_node;
        }
        tokens.tokens_tail = new_node;
        tokens.tokens_p = new_node;
    }
    return false;
}

static inline bool next_token(Token *&t) {
    if (preget_next_token()) return true;
    t = tokens.tokens_p->token;
    tokens.tokens_p = tokens.tokens_p->next;
    return false;
}

static inline bool CompUnit();
static inline bool Decl();
static inline bool ConstDecl();
static inline bool BType();
static inline bool ConstDef();
static inline bool ConstInitVal();
static inline bool VarDecl();
static inline bool VarDef();
static inline bool InitVal();
static inline bool FuncDef();
static inline bool FuncType();
static inline bool FuncFParams();
static inline bool FuncFParam();
static inline bool Block();
static inline bool BlockItem();
static inline bool Stmt();
static inline bool IFMatchedStmt();
static inline bool IFUnmatchedStmt();
static inline bool LVal();
static inline bool Exp();
static inline bool LOrExp();
static inline bool LAndExp();
static inline bool EqExp();
static inline bool RelExp();
static inline bool AddExp();
static inline bool MulExp();
static inline bool UnaryExp();
static inline bool PrimaryExp();
static inline bool Number();
static inline bool FuncRParams();
static inline bool UnaryOp();
static inline bool ConstExp();

static inline bool token(int type) {
    Token_stack_node *store = tokens.tokens_p;

    Token *t = NULL;

    if (next_token(t)) {
        return type == TK_EOF;
    }

    if (t->type == type) {
    printf("Token → '%s'\n", t->str);
    return true;
    }
    RESTORE_P;
    
    return false;
}

//1. 编译单元 **CompUnit** → ( Decl | FuncDef ) { Decl | FuncDef }
static inline bool CompUnit() {
    STORE_P;
    do {
        if (Decl() || FuncDef()) {
            break;
        }

        RESTORE_P;
        return false;
    } while (0);

    do {
        STORE_P;
        if (Decl() || FuncDef()) {
            continue;
        }

        RESTORE_P;
        printf("CompUnit → ( Decl | FuncDef ) { Decl | FuncDef }\n");
        return true;
    } while (1);
}

//1. 声明 **Decl** → ConstDecl | VarDecl
static inline bool Decl() {
    STORE_P;

    do {
        if (ConstDecl()) {
            printf("Decl → ConstDecl\n");
            return true;
        } 

        RESTORE_P;
        break;
    } while(0);

    do {
        if (VarDecl()) {
            printf("Decl → VarDecl\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 常量声明 **ConstDecl** → 'const' BType ConstDef { ',' ConstDef } ';'
static inline bool ConstDecl() {
    STORE_P;

    do {
        if (token(TK_CONST) && BType() && ConstDef()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && ConstDef()) continue;

        RESTORE_P;
        break;
    } while(1);

    do {
        if (token(';')) {
            printf("ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 基本类型 **BType** → 'int'
static inline bool BType() {
    STORE_P;

    do{
        if (token(TK_INT)) {
            printf("BType → 'int'\n");
            return true;
        }

        RESTORE_P;
        return false;
    }while(0);
}

//1. 常数定义 **ConstDef** → Ident { '[' ConstExp ']' } '=' ConstInitVal
static inline bool ConstDef() {
    STORE_P;

    do {
        if (token(TK_ID)) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('[') && ConstExp() && token(']')) continue;

        RESTORE_P;
        break;
    } while(1);

    do {
        if (token(TK_ASSIGN) && ConstInitVal()) {
            printf("ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 数组初值 **ConstInitArray_** → ConstInitVal { ',' ConstInitVal }
static inline bool ConstInitArray_() {
    STORE_P;

    do {
        if (ConstInitVal()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && ConstInitVal()) continue;

        RESTORE_P;
        printf("ConstInitArray_ → ConstInitVal { ',' ConstInitVal }\n");
        return true;
    } while(1);
}

//1. 常量数组 **ConstInitArray** → '{' [ ConstInitArray_ ] '}'
static inline bool ConstInitArray() {
    STORE_P;

    do {
        if (token('{')) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (ConstInitArray_()) break;

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token('}')) {
            printf("ConstInitArray → '{' [ ConstInitArray_ ] '}'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while (0);
}

//1. 常量初值 **ConstInitVal** → ConstExp | ConstInitArray
static inline bool ConstInitVal() {
    STORE_P;

    do {
        if (ConstExp()) {
            printf("ConstInitVal → ConstExp\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (ConstInitArray()) {
            printf("ConstInitVal → ConstInitArray\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 变量声明 **VarDecl** → BType VarDef { ',' VarDef } ';'
static inline bool VarDecl() {
    STORE_P;

    do {
        if (BType() && VarDef()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && VarDef()) continue;

        RESTORE_P;
        break;
    } while(1);

    do {
        if (token(';')) {
            printf("VarDecl → BType VarDef { ',' VarDef } ';'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 变量定义 **VarDef** → Ident { '[' ConstExp ']' } [ '=' InitVal ]
static inline bool VarDef() {
    STORE_P;

    do {
        if (token(TK_ID)) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('[') && ConstExp() && token(']')) continue;

        RESTORE_P;
        break;
    } while(1);

    do {
        STORE_P;
        if (token(TK_ASSIGN) && InitVal()) break;

        RESTORE_P;
        break;
    } while(0);
    printf("VarDef → Ident { '[' ConstExp ']' } [ '=' InitVal ]\n");
    return true;
}

//1. 数组初值 **InitArray_** → InitVal { ',' InitVal }
static inline bool InitArray_() {
    STORE_P;
    do {
        if (InitVal()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && InitVal()) continue;

        RESTORE_P;
        printf("InitArray_ → InitVal { ',' InitVal }\n");
        return true;
    } while (1);
}

//1. 变量数组 **InitArray** → '{' [ InitArray_ ] '}'
static inline bool InitArray() {
    STORE_P;

    do {
        if (token('{')) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (InitArray_()) break;

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token('}')) {
            printf("InitArray → '{' [ InitArray_ ] '}'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while (0);
}

//1. 变量初值 **InitVal** → Exp | InitArray
static inline bool InitVal() {
    STORE_P;

    do {
        if (Exp()) {
            printf("InitVal → Exp\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (InitArray()) {
            printf("InitVal → InitArray\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 函数定义 **FuncDef** → FuncType Ident '(' [ FuncFParams ] ')' Block
static inline bool FuncDef() {
    STORE_P;

    do {
        if (FuncType() && token(TK_ID) && token('(')) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (FuncFParams()) break;

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(')') && Block()) {
            printf("FuncDef → FuncType Ident '(' [ FuncFParams ] ')' Block\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 函数类型 **FuncType** → 'int' | 'void'
static inline bool FuncType() {
    STORE_P;

    do {
        if (token(TK_INT)) {
            printf("FuncType → 'int'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_VOID)) {
            printf("FuncType → 'void'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 函数形参表 **FuncFParams** → FuncFParam { ',' FuncFParam }
static inline bool FuncFParams() {
    STORE_P;

    do {
        if (FuncFParam()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && FuncFParam()) continue;

        RESTORE_P;
        printf("FuncFParams → FuncFParam { ',' FuncFParam }\n");
        return true;
    } while (1);
}

//1. 函数形参 **FuncFParamIndex** → '[' ']' { '[' ConstExp ']' }
static inline bool FuncFParamIndex() {
    STORE_P;

    do {
        if (token('[') && token(']')) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('[') && ConstExp() && token(']')) continue;

        RESTORE_P;
        printf("FuncFParamIndex → '[' ']' { '[' ConstExp ']' }\n");
        return true;
    } while (1);
}

//1. 函数形参 **FuncFParam** → BType Ident [ FuncFParamIndex ]
static inline bool FuncFParam() {
    STORE_P;

    do {
        if (BType() && token(TK_ID)) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (FuncFParamIndex()) break;

        RESTORE_P;
        break;
    } while(0);
    printf("FuncFParam → BType Ident [ FuncFParamIndex ]\n");
    return true;
}

//1. 语句块 **Block** → '{' { BlockItem } '}'
static inline bool Block() {
    STORE_P;

    do {
        if (token('{')) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (BlockItem()) continue;

        RESTORE_P;
        break;
    } while (1);

    do {
        if (token('}')) {
            printf("Block → '{' { BlockItem } '}'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while (0);
}

//1. 语句块项 **BlockItem** → Decl | Stmt
static inline bool BlockItem() {
    STORE_P;

    do {
        if (Decl()) {
            printf("BlockItem → Decl\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (Stmt()) {
            printf("BlockItem → Stmt\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 语句 **Stmt** → IFMatchedStmt | IFUnmatchedStmt
static inline bool Stmt() {
    STORE_P;

    do {
        if (IFMatchedStmt()) {
            printf("Stmt → IFMatchedStmt\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (IFUnmatchedStmt()) {
            printf("Stmt → IFUnmatchedStmt\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. if匹配语句 **IFMatchedStmt** → 'if' '( Exp ')' IFMatchedStmt 'else' IFMatchedStmt
//| Block
//| ';'
//| Exp ';'
//| 'break' ';'
//| 'continue' ';'
//| 'return' Exp ';'
//| 'return' ';'
//| 'while' '(' Exp ')' Stmt
static inline bool IFMatchedStmt() {
    STORE_P;

    do {
        if (token(TK_IF) && token('(') && Exp() && token(')') && IFMatchedStmt() && token(TK_ELSE) && IFMatchedStmt()) {
            printf("IFMatchedStmt → 'if' '( Exp ')' IFMatchedStmt 'else' IFMatchedStmt\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (Block()) {
            printf("IFMatchedStmt → Block\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(';')) {
            printf("IFMatchedStmt → ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (Exp() && token(';')) {
            printf("IFMatchedStmt → Exp ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_BREAK) && token(';')) {
            printf("IFMatchedStmt → 'break' ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_CONTINUE) && token(';')) {
            printf("IFMatchedStmt → 'continue' ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_RETURN) && Exp() && token(';')) {
            printf("IFMatchedStmt → 'return' Exp ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_RETURN) && token(';')) {
            printf("IFMatchedStmt → 'return' ';'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_WHILE) && token('(') && Exp() && token(')') && Stmt()) {
            printf("IFMatchedStmt → 'while' '(' Exp ')' Stmt\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);

}

//1. if不匹配语句 **IFUnmatchedStmt** → 'if' '( Exp ')' IFMatchedStmt [ 'else' IFUnmatchedStmt ]
static inline bool IFUnmatchedStmt() {
    STORE_P;

    do {
        if (token(TK_IF) && token('(') && Exp() && token(')') && IFMatchedStmt()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(TK_ELSE) && IFUnmatchedStmt()) break;

        RESTORE_P;
        break;
    } while(0);
    printf("IFUnmatchedStmt → 'if' '( Exp ')' IFMatchedStmt [ 'else' IFUnmatchedStmt ]\n");
    return true;
}

//1. 左值表达式 **LVal** → Ident { '[' Exp ']' }
static inline bool LVal() {
    STORE_P;

    do {
        if (token(TK_ID)) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('[') && Exp() && token(']')) continue;

        RESTORE_P;
        printf("LVal → Ident { '[' Exp ']' }\n");
        return true;
    } while (1);
}

//1. 表达式 **Exp** → LVal '=' Exp | LOrExp
static inline bool Exp() {
    STORE_P;

    do {
        if (LVal() && token(TK_ASSIGN) && Exp()) {
            printf("Exp → LVal '=' Exp\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (LOrExp()) {
            printf("Exp → LOrExp\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 逻辑或表达式 **LOrExp** → LAndExp [ '||' LOrExp ]
static inline bool LOrExp() {
    STORE_P;

    do {
        if (LAndExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('|') && LOrExp()) {
            printf("LOrExp → LAndExp '||' LAndExp\n");
            break;
        }

        RESTORE_P;
        printf("LOrExp → LAndExp\n");
        break;
    } while(0);
    return true;
}

//1. 逻辑与表达式 **LAndExp** → EqExp [ '&&' LAndExp ]
static inline bool LAndExp() {
    STORE_P;

    do {
        if (EqExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token('&') && LAndExp()) {
            printf("LAndExp → EqExp '&&' LAndExp\n");
            break;
        }

        RESTORE_P;
        printf("LAndExp → EqExp\n");
        break;
    } while(0);
    return true;
}

//1. 相等性表达式 **EqExp** → RelExp [ ('==' | '!=') EqExp ]
static inline bool EqExp() {
    STORE_P;

    do {
        if (RelExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if ((token(TK_EQUAL) || token(TK_NEQUAL)) && EqExp()) {
            printf("EqExp → RelExp ('==' | '!=') EqExp\n");
            break;
        }

        RESTORE_P;
        printf("EqExp → RelExp\n");
        break;
    } while(0);
    return true;
}

//1. 关系表达式 **RelExp** → AddExp [ ('<' | '>' | '<=' | '>=') RelExp ]
static inline bool RelExp() {
    STORE_P;

    do {
        if (AddExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if ((token(TK_LT) || token(TK_GT) || token(TK_LTE) || token(TK_GTE)) && RelExp()) {
            printf("RelExp → AddExp ('<' | '>' | '<=' | '>=') RelExp\n");
            break;
        }

        RESTORE_P;
        printf("RelExp → AddExp\n");
        break;
    } while(0);
    return true;
}

//1. 加减表达式 **AddExp** → MulExp [ ('+' | '−') AddExp ]
static inline bool AddExp() {
    STORE_P;

    do {
        if (MulExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if ((token('+') || token('-')) && AddExp()) {
            printf("AddExp → MulExp ('+' | '−') AddExp\n");
            break;
        }

        RESTORE_P;
        printf("AddExp → MulExp\n");
        break;
    } while(0);
    return true;
}

//1. 乘除模表达式 **MulExp** → UnaryExp [ ('*' | '/' | '%') MulExp ]
static inline bool MulExp() {
    STORE_P;

    do {
        if (UnaryExp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if ((token('*') || token('/') || token('%')) && MulExp()) {
            printf("MulExp → UnaryExp ('*' | '/' | '%%') MulExp\n");
            break;
        }

        RESTORE_P;
        printf("MulExp → UnaryExp\n");
        break;
    } while(0);
    return true;
}

//1. 一元表达式 **UnaryExp** → Ident '(' ')' | Ident '(' FuncRParams ')' | PrimaryExp | UnaryOp UnaryExp
static inline bool UnaryExp() {
    STORE_P;

    do {
        if (token(TK_ID) && token('(') && token(')')) {
            printf("UnaryExp → Ident '(' ')'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (token(TK_ID) && token('(') && FuncRParams() && token(')')) {
            printf("UnaryExp → Ident '(' FuncRParams ')'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (PrimaryExp()) {
            printf("UnaryExp → PrimaryExp\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (UnaryOp() && UnaryExp()) {
            printf("UnaryExp → UnaryOp UnaryExp\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 基本表达式 **PrimaryExp** → '(' Exp ')' | LVal | Number
static inline bool PrimaryExp() {
    STORE_P;

    do {
        if (token('(') && Exp() && token(')')) {
            printf("PrimaryExp → '(' Exp ')'\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (LVal()) {
            printf("PrimaryExp → LVal\n");
            return true;
        }

        RESTORE_P;
        break;
    } while(0);

    do {
        if (Number()) {
            printf("PrimaryExp → Number\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 数值 **Number** → IntConst
static inline bool Number() {
    STORE_P;

    do {
        if (token(TK_INTCON)) {
            printf("Number → IntConst\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 函数实参表 **FuncRParams** → Exp { ',' Exp }
static inline bool FuncRParams() {
    STORE_P;

    do {
        if (Exp()) break;

        RESTORE_P;
        return false;
    } while(0);

    do {
        STORE_P;
        if (token(',') && Exp()) continue;

        RESTORE_P;
        printf("FuncRParams → Exp { ',' Exp }\n");
        return true;
    } while(1);
}

//1. 单目运算符 **UnaryOp** → '+' | '−' | '!'
static inline bool UnaryOp() {
    STORE_P;

    do {
        if (token('+')) {
            printf("UnaryOp → '+'\n");
            return true;
        }
        if (token('-')) {
            printf("UnaryOp → '−'\n");
            return true;
        }
        if (token('!')) {
            printf("UnaryOp → '!'\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

//1. 常量表达式 **ConstExp** → Exp 注：语义分析是需要确定为常量表达式
static inline bool ConstExp() {
    STORE_P;

    do {
        if (Exp()) {
            printf("ConstExp → Exp\n");
            return true;
        }

        RESTORE_P;
        return false;
    } while(0);
}

void syntax_init(char *src){
    lexer_init(src);
}

void syntax_free(){
    lexer_free();
}

bool syntax(){
    return CompUnit() && token(TK_EOF);
}
