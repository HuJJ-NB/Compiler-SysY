#include <syntax.h>
#include <debug.h>

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

static inline SyntaxUnit *syntax_unit_new() {
    SyntaxUnit *unit = (SyntaxUnit *)malloc(sizeof(SyntaxUnit));
    unit->type = SYN_NOTYPE;
    unit->line_no = 0;
    unit->first_sub_node = NULL;
    unit->next = NULL;
    unit->token = NULL;
    return unit;
}

static inline void syntax_unit_brother_delete(SyntaxUnit *unit);

static inline void syntax_unit_delete(SyntaxUnit *unit) {
    if (unit->first_sub_node) {
        syntax_unit_brother_delete(unit->first_sub_node);
    }
    free(unit);
}

static inline void syntax_unit_brother_delete(SyntaxUnit *unit) {
    while (unit) {
        SyntaxUnit *del = unit;
        unit = unit->next;
        syntax_unit_delete(del);
    }
}

static inline void syntax_unit_visit(SyntaxUnit *unit, int depth = 0) {
    printf("%*s", 2 * depth, "");
    switch (unit->type) {
    case SYN_CompUnit:
        printf("CompUnit :%d\n", unit->line_no);
        break;
    case SYN_Decl:
        printf("Decl :%d\n", unit->line_no);
        break;
    case SYN_ConstDecl:
        printf("ConstDecl :%d\n", unit->line_no);
        break;
    case SYN_BType:
        printf("BType :%d\n", unit->line_no);
        break;
    case SYN_ConstDef:
        printf("ConstDef :%d\n", unit->line_no);
        break;
	case SYN_ConstInitArray:
        printf("ConstInitArray :%d\n", unit->line_no);
        break;
	case SYN_ConstInitVal:
        printf("ConstInitVal :%d\n", unit->line_no);
        break;
	case SYN_VarDecl:
        printf("VarDecl :%d\n", unit->line_no);
        break;
	case SYN_VarDef:
        printf("VarDef :%d\n", unit->line_no);
        break;
	case SYN_InitArray:
        printf("InitArray :%d\n", unit->line_no);
        break;
	case SYN_InitVal:
        printf("InitVal :%d\n", unit->line_no);
        break;
	case SYN_FuncDef:
        printf("FuncDef :%d\n", unit->line_no);
        break;
	case SYN_FuncType:
        printf("FuncType :%d\n", unit->line_no);
        break;
	case SYN_FuncFParams:
        printf("FuncFParams :%d\n", unit->line_no);
        break;
	case SYN_FuncFParamIndex:
        printf("FuncFParamIndex :%d\n", unit->line_no);
        break;
	case SYN_FuncFParam:
        printf("FuncFParam :%d\n", unit->line_no);
        break;
	case SYN_Block:
        printf("Block :%d\n", unit->line_no);
        break;
	case SYN_BlockItem:
        printf("BlockItem :%d\n", unit->line_no);
        break;
	case SYN_Stmt:
        printf("Stmt :%d\n", unit->line_no);
        break;
	case SYN_IFMatchedStmt:
        printf("IFMatchedStmt :%d\n", unit->line_no);
        break;
	case SYN_IFUnmatchedStmt:
        printf("IFUnmatchedStmt :%d\n", unit->line_no);
        break;
	case SYN_LVal:
        printf("LVal :%d\n", unit->line_no);
        break;
	case SYN_Exp:
        printf("Exp :%d\n", unit->line_no);
        break;
	case SYN_LOrExp:
        printf("LOrExp :%d\n", unit->line_no);
        break;
	case SYN_LAndExp:
        printf("LAndExp :%d\n", unit->line_no);
        break;
	case SYN_EqExp:
        printf("EqExp :%d\n", unit->line_no);
        break;
	case SYN_RelExp:
        printf("RelExp :%d\n", unit->line_no);
        break;
	case SYN_AddExp:
        printf("AddExp :%d\n", unit->line_no);
        break;
	case SYN_MulExp:
        printf("MulExp :%d\n", unit->line_no);
        break;
	case SYN_UnaryExp:
        printf("UnaryExp :%d\n", unit->line_no);
        break;
	case SYN_PrimaryExp:
        printf("PrimaryExp :%d\n", unit->line_no);
        break;
	case SYN_Number:
        printf("Number :%d\n", unit->line_no);
        break;
	case SYN_FuncRParams:
        printf("FuncRParams :%d\n", unit->line_no);
        break;
	case SYN_UnaryOp:
        printf("UnaryOp :%d\n", unit->line_no);
        break;
	case SYN_ConstExp:
        printf("ConstExp :%d\n", unit->line_no);
        break;
	case SYN_TOKEN:
        switch (unit->token->type) {
		case TK_INTCON:
            printf("INTCON %s\n", unit->token->str);
            break;
		case TK_ID:
            printf("ID %s\n", unit->token->str);
            break;
		case TK_CONST:
            printf("KEY CONST\n");
            break;
		case TK_INT:
            printf("KEY INT\n");
            break;
		case TK_VOID:
            printf("KEY VOID\n");
            break;
		case TK_IF:
            printf("KEY IF\n");
            break;
		case TK_ELSE:
            printf("KEY ELSE\n");
            break;
		case TK_WHILE:
            printf("KEY WHILE\n");
            break;
		case TK_DO:
            printf("KEY DO\n");
            break;
		case TK_BREAK:
            printf("KEY BREAK\n");
            break;
		case TK_CONTINUE:
            printf("KEY CONTINUE\n");
            break;
		case TK_RETURN:
            printf("KEY RETURN\n");
            break;
		case TK_ASSIGN:
            printf("ASSIGN =\n");
            break;
		case TK_ADD_ASSIGN:
            printf("ASSIGN +=\n");
            break;
		case TK_SUB_ASSIGN: 
            printf("ASSIGN -=\n");
            break;
		case TK_MUL_ASSIGN:
            printf("ASSIGN *=\n");
            break;
		case TK_DIV_ASSIGN:
            printf("ASIGN /=\n");
            break;
		case TK_MOD_ASSIGN:
            printf("ASIGN %%=\n");
            break;
		case TK_EQUAL:
            printf("OP ==\n");
            break;
		case TK_NEQUAL:
            printf("OP !=\n");
            break;
		case TK_LTE:
            printf("OP <=\n");
            break;
		case TK_LT:
            printf("OP <\n");
            break;
		case TK_GTE:
            printf("OP >=\n");
            break;
		case TK_GT:
            printf("OP >\n");
            break;
        case TK_LAND:
            printf("OP &&\n");
            break;
        case TK_LOR:
            printf("OP ||\n");
            break;
        case TK_LNOT:
            printf("OP !\n");
        case TK_ADD:
            printf("OP +\n");
            break;
        case TK_SUB:
            printf("OP -\n");
            break;
        case TK_MUL:
            printf("OP *\n");
            break;
        case TK_DIV:
            printf("OP /\n");
            break;
        case TK_MOD:
            printf("OP %%\n");
            break;
        case '(':
            printf("LPAREN (\n");
            break;
        case ')':
            printf("RPAREN )\n");
            break;
        case '[':
            printf("LBRACKET [\n");
            break;
        case ']':
            printf("RBRACKET ]\n");
            break;
        case '{':
            printf("LBRACE {\n");
            break;
        case '}':
            printf("RBRACE }\n");
            break;
        case ';':
            printf("SEMICN ;\n");
            break;
        case ',':
            printf("COMMA ,\n");
            break;
        }
        break;
    }
    if (unit->first_sub_node) {
        SyntaxUnit *cur = unit->first_sub_node;
        while (cur) {
            syntax_unit_visit(cur, depth + 1);
            cur = cur->next;
        }
    }
}

static inline void syntax_unit_set_sub_node(SyntaxUnit *unit, SyntaxUnit *sub_node) {
    if (unit->first_sub_node) {
        syntax_unit_delete(unit->first_sub_node);
    }
    if (sub_node) unit->line_no = sub_node->line_no;
    unit->first_sub_node = sub_node;
}

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

static inline bool CompUnit(SyntaxUnit *tree);
static inline bool Decl(SyntaxUnit *tree);
static inline bool ConstDecl(SyntaxUnit *tree);
static inline bool BType(SyntaxUnit *tree);
static inline bool ConstDef(SyntaxUnit *tree);
static inline bool ConstInitArray(SyntaxUnit *tree);
static inline bool ConstInitVal(SyntaxUnit *tree);
static inline bool VarDecl(SyntaxUnit *tree);
static inline bool VarDef(SyntaxUnit *tree);
static inline bool InitArray(SyntaxUnit *tree);
static inline bool InitVal(SyntaxUnit *tree);
static inline bool FuncDef(SyntaxUnit *tree);
static inline bool FuncType(SyntaxUnit *tree);
static inline bool FuncFParams(SyntaxUnit *tree);
static inline bool FuncFParamIndex(SyntaxUnit *tree);
static inline bool FuncFParam(SyntaxUnit *tree);
static inline bool Block(SyntaxUnit *tree);
static inline bool BlockItem(SyntaxUnit *tree);
static inline bool Stmt(SyntaxUnit *tree);
static inline bool IFMatchedStmt(SyntaxUnit *tree);
static inline bool IFUnmatchedStmt(SyntaxUnit *tree);
static inline bool LVal(SyntaxUnit *tree);
static inline bool Exp(SyntaxUnit *tree);
static inline bool LOrExp(SyntaxUnit *tree);
static inline bool LAndExp(SyntaxUnit *tree);
static inline bool EqExp(SyntaxUnit *tree);
static inline bool RelExp(SyntaxUnit *tree);
static inline bool AddExp(SyntaxUnit *tree);
static inline bool MulExp(SyntaxUnit *tree);
static inline bool UnaryExp(SyntaxUnit *tree);
static inline bool PrimaryExp(SyntaxUnit *tree);
static inline bool Number(SyntaxUnit *tree);
static inline bool FuncRParams(SyntaxUnit *tree);
static inline bool UnaryOp(SyntaxUnit *tree);
static inline bool ConstExp(SyntaxUnit *tree);

static inline bool is_eof() {
    STORE_P;
    Token *t;
    if (next_token(t)) {
        return true;
    }
    RESTORE_P;
    return false;
}

static inline bool token(int type, SyntaxUnit *unit) {
    Assert(unit, "SyntaxUnit is NULL");
    unit->type = SYN_TOKEN;
    syntax_unit_set_sub_node(unit, NULL);

    STORE_P;

    if (next_token(unit->token)) return false;

    if (unit->token->type == type) {
        unit->line_no = unit->token->line_no;
        return true;
    }
    RESTORE_P;
    
    return false;
}

//1. 编译单元 **CompUnit** → ( Decl | FuncDef ) { Decl | FuncDef }
static inline bool CompUnit(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_CompUnit;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;
    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Decl(node_p) || FuncDef(node_p)) {
            cur->next = node_p;
            cur = cur->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while (0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        STORE_P;
        if (Decl(node_p) || FuncDef(node_p)) {
            cur->next = node_p;
            cur = node_p;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while (1);

    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 声明 **Decl** → ConstDecl | VarDecl
static inline bool Decl(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_Decl;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (ConstDecl(node_p) || VarDecl(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 常量声明 **ConstDecl** → 'const' BType ConstDef { ',' ConstDef } ';'
static inline bool ConstDecl(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_ConstDecl;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;

    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();

        if (token(TK_CONST, node_p) && BType(node_p->next) && ConstDef(node_p->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && ConstDef(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(';', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 基本类型 **BType** → 'int'
static inline bool BType(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_BType;
    STORE_P;

    do{
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_INT, node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    }while(0);
}

//1. 常数定义 **ConstDef** → Ident { '[' ConstExp ']' } '=' ConstInitVal
static inline bool ConstDef(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_ConstDef;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_ID, node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        STORE_P;
        if (token('[', node_p) && ConstExp(node_p->next) && token(']', node_p->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token(TK_ASSIGN, node_p) && ConstInitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 数组初值 **ConstInitArray** → '{' ConstInitVal { ',' ConstInitVal } '}'
static inline bool ConstInitArray(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_ConstInitArray;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token('{', node_p) && ConstInitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && ConstInitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token('}', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while (0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 常量初值 **ConstInitVal** → ConstExp | '{' '}' | ConstInitArray
static inline bool ConstInitVal(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_ConstInitVal;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (ConstExp(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);
    
    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token('{', node_p) && token('}', node_p->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (ConstInitArray(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 变量声明 **VarDecl** → BType VarDef { ',' VarDef } ';'
static inline bool VarDecl(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_VarDecl;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (BType(node_p) && VarDef(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && VarDef(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(';', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;

}

//1. 变量定义 **VarDef** → Ident { '[' ConstExp ']' } [ '=' InitVal ]
static inline bool VarDef(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_VarDef;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_ID, node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        STORE_P;
        if (token('[', node_p) && ConstExp(node_p->next) && token(']', node_p->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(TK_ASSIGN, node_p) && InitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 数组初值 **InitArray** → '{' InitVal { ',' InitVal } '}'
static inline bool InitArray(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_InitArray;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token('{', node_p) && InitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && InitVal(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while (1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token('}', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while (0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 变量初值 **InitVal** → Exp | '{' '}' | InitArray
static inline bool InitVal(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_InitVal;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Exp(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token('{', node_p) && token('}', node_p->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (InitArray(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 函数定义 **FuncDef** → FuncType Ident '(' [ FuncFParams ] ')' Block
static inline bool FuncDef(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncDef;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        if (FuncType(node_p) && token(TK_ID, node_p->next) && token('(', node_p->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        STORE_P;
        if (FuncFParams(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token(')', node_p) && Block(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 函数类型 **FuncType** → 'int' | 'void'
static inline bool FuncType(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncType;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_INT, node_p) || token(TK_VOID, node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 函数形参表 **FuncFParams** → FuncFParam { ',' FuncFParam }
static inline bool FuncFParams(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncFParams;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (FuncFParam(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && FuncFParam(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while (1);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 函数形参 **FuncFParamIndex** → '[' ']' { '[' ConstExp ']' }
static inline bool FuncFParamIndex(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncFParamIndex;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (token('[', node_p) && token(']', node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        STORE_P;
        if (token('[', node_p) && ConstExp(node_p->next) && token(']', node_p->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while (1);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 函数形参 **FuncFParam** → BType Ident [ FuncFParamIndex ]
static inline bool FuncFParam(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncFParam;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (BType(node_p) && token(TK_ID, node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        STORE_P;
        if (FuncFParamIndex(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 语句块 **Block** → '{' { BlockItem } '}'
static inline bool Block(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_Block;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token('{', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        STORE_P;
        if (BlockItem(node_p)) {
            cur->next = node_p;
            cur = node_p;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while (1);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token('}', node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while (0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 语句块项 **BlockItem** → Decl | Stmt
static inline bool BlockItem(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_BlockItem;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Decl(node_p) || Stmt(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 语句 **Stmt** → IFMatchedStmt | IFUnmatchedStmt
static inline bool Stmt(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_Stmt;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (IFMatchedStmt(node_p) || IFUnmatchedStmt(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. if匹配语句 **IFMatchedStmt** → ( Block | ';' )
//| ( Exp | 'break' | 'continue' | 'return' ) ';'
//| 'return' Exp ';'
//| 'while' '(' Exp ')' Stmt
//| 'if' '( Exp ')' IFMatchedStmt 'else' IFMatchedStmt
static inline bool IFMatchedStmt(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_IFMatchedStmt;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Block(node_p) || token(';', node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if ((Exp(node_p) || token(TK_BREAK, node_p) || token(TK_CONTINUE, node_p) || token(TK_RETURN, node_p)) && token(';', node_p->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        if (token(TK_RETURN, node_p) && Exp(node_p->next) && token(';', node_p->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        node_p->next->next->next = syntax_unit_new();
        node_p->next->next->next->next = syntax_unit_new();
        if (token(TK_WHILE, node_p) && token('(', node_p->next) && Exp(node_p->next->next) && token(')', node_p->next->next->next) && Stmt(node_p->next->next->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next->next->next);
        syntax_unit_delete(node_p->next->next->next);
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        node_p->next->next->next = syntax_unit_new();
        node_p->next->next->next->next = syntax_unit_new();
        node_p->next->next->next->next->next = syntax_unit_new();
        node_p->next->next->next->next->next->next = syntax_unit_new();
        if (token(TK_IF, node_p) && token('(', node_p->next) && Exp(node_p->next->next) && token(')', node_p->next->next->next) && IFMatchedStmt(node_p->next->next->next->next) && token(TK_ELSE, node_p->next->next->next->next->next) && IFMatchedStmt(node_p->next->next->next->next->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next->next->next->next->next);
        syntax_unit_delete(node_p->next->next->next->next->next);
        syntax_unit_delete(node_p->next->next->next->next);
        syntax_unit_delete(node_p->next->next->next);
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. if不匹配语句 **IFUnmatchedStmt** → 'if' '( Exp ')' IFMatchedStmt [ 'else' IFUnmatchedStmt ]
static inline bool IFUnmatchedStmt(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_IFUnmatchedStmt;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        node_p->next->next->next = syntax_unit_new();
        node_p->next->next->next->next = syntax_unit_new();
        if (token(TK_IF, node_p) && token('(', node_p->next) && Exp(node_p->next->next) && token(')', node_p->next->next->next) && IFMatchedStmt(node_p->next->next->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next->next->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next->next->next);
        syntax_unit_delete(node_p->next->next->next);
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(TK_ELSE, node_p) && IFUnmatchedStmt(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 左值表达式 **LVal** → Ident { '[' Exp ']' }
static inline bool LVal(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_LVal;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_ID, node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        STORE_P;
        if (token('[', node_p) && Exp(node_p->next) && token(']', node_p->next->next)) {
            cur->next = node_p;
            cur = node_p->next->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while (1);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 表达式 **Exp** → LVal '=' Exp | LOrExp
static inline bool Exp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_Exp;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        if (LVal(node_p) && token(TK_ASSIGN, node_p->next) && Exp(node_p->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (LOrExp(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 逻辑或表达式 **LOrExp** → LAndExp [ '||' LOrExp ]
static inline bool LOrExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_LOrExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (LAndExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(TK_LOR, node_p) && LOrExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 逻辑与表达式 **LAndExp** → EqExp [ '&&' LAndExp ]
static inline bool LAndExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_LAndExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (EqExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(TK_LAND, node_p) && LAndExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 相等性表达式 **EqExp** → RelExp [ ('==' | '!=') EqExp ]
static inline bool EqExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_EqExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (RelExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if ((token(TK_EQUAL, node_p) || token(TK_NEQUAL, node_p)) && EqExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 关系表达式 **RelExp** → AddExp [ ('<' | '>' | '<=' | '>=') RelExp ]
static inline bool RelExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_RelExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (AddExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if ((token(TK_LT, node_p) || token(TK_GT, node_p) || token(TK_LTE, node_p) || token(TK_GTE, node_p)) && RelExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 加减表达式 **AddExp** → MulExp [ ('+' | '−') AddExp ]
static inline bool AddExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_AddExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (MulExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if ((token(TK_ADD, node_p) || token(TK_SUB, node_p)) && AddExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 乘除模表达式 **MulExp** → UnaryExp [ ('*' | '/' | '%') MulExp ]
static inline bool MulExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_MulExp;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (UnaryExp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if ((token(TK_MUL, node_p) || token(TK_DIV, node_p) || token(TK_MOD, node_p)) && MulExp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 一元表达式 **UnaryExp** → Ident '(' ')' | Ident '(' FuncRParams ')' | PrimaryExp | UnaryOp UnaryExp
static inline bool UnaryExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_UnaryExp;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        if (token(TK_ID, node_p) && token('(', node_p->next) && token(')', node_p->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        node_p->next->next->next = syntax_unit_new();
        if (token(TK_ID, node_p) && token('(', node_p->next) && FuncRParams(node_p->next->next) && token(')', node_p->next->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next->next);
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (PrimaryExp(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        if (UnaryOp(node_p) && UnaryExp(node_p->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 基本表达式 **PrimaryExp** → '(' Exp ')' | LVal | Number
static inline bool PrimaryExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_PrimaryExp;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        node_p->next->next = syntax_unit_new();
        if (token('(', node_p) && Exp(node_p->next) && token(')', node_p->next->next)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next->next);
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (LVal(node_p) || Number(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 数值 **Number** → IntConst
static inline bool Number(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_Number;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_INTCON, node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 函数实参表 **FuncRParams** → Exp { ',' Exp }
static inline bool FuncRParams(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_FuncRParams;
    SyntaxUnit *head = syntax_unit_new();
    SyntaxUnit *cur = head;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Exp(node_p)) {
            cur->next = node_p;
            cur = node_p;
            break;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        syntax_unit_brother_delete(head);
        return false;
    } while(0);

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        node_p->next = syntax_unit_new();
        STORE_P;
        if (token(',', node_p) && Exp(node_p->next)) {
            cur->next = node_p;
            cur = node_p->next;
            continue;
        }

        RESTORE_P;
        syntax_unit_delete(node_p->next);
        syntax_unit_delete(node_p);
        break;
    } while(1);
    syntax_unit_set_sub_node(tree, head->next);
    syntax_unit_delete(head);
    return true;
}

//1. 单目运算符 **UnaryOp** → '+' | '−' | '!'
static inline bool UnaryOp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_UnaryOp;
    
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (token(TK_ADD, node_p) || token(TK_SUB, node_p) || token(TK_LNOT, node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

//1. 常量表达式 **ConstExp** → Exp 注：语义分析是需要确定为常量表达式
static inline bool ConstExp(SyntaxUnit *tree) {
    Assert(tree, "SyntaxUnit is NULL");
    tree->type = SYN_ConstExp;
    STORE_P;

    do {
        SyntaxUnit *node_p = syntax_unit_new();
        if (Exp(node_p)) {
            syntax_unit_set_sub_node(tree, node_p);
            return true;
        }

        RESTORE_P;
        syntax_unit_delete(node_p);
        return false;
    } while(0);
}

int syntax(char *src){
    lexer_init(src);

    SyntaxUnit *tree = syntax_unit_new();
    bool ret = CompUnit(tree) && is_eof();
    syntax_unit_visit(tree);
    syntax_unit_delete(tree);

    lexer_free();
    return !ret;
}
