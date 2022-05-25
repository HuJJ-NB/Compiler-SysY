# Compiler-SysY

## 运行

* `make run`或者`./build/app`

## 目录

``` shell
  .
  ├── [build]         //生成的文件
  ├── include         //头文件
  ├── Makefile        //主要的makefile
  ├── README.md
  ├── script
  │   └── build.mk    //用于构建的makefile
  └── src             //源码
```

## 文法

### 原推导式

SysY 语言的文法采用扩展的 Backus 范式表示，其中：

* 符号`[...]`表示方括号内包含的为可选项

* 符号`{...}`表示花括号内包含的为可重复 $0$ 次或多次的项

* `终结符`是由单引号括起的串，或者是`Ident`、`IntConst`这样的记号

SysY 语言的文法表示如下，其中 **`CompUnit`为开始符号**：

1. 编译单元 **CompUnit** → [ CompUnit ] ( Decl | FuncDef )
1. 声明 **Decl** → ConstDecl | VarDecl
1. 常量声明 **ConstDecl** → 'const' BType ConstDef { ',' ConstDef } ';'
1. 基本类型 **BType** → 'int'
1. 常数定义 **ConstDef** → Ident { '[' ConstExp ']' } '=' ConstInitVal
1. 常量初值 **ConstInitVal** → ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
1. 变量声明 **VarDecl** → BType VarDef { ',' VarDef } ';'
1. 变量定义 **VarDef** → Ident { '[' ConstExp ']' } [ '=' InitVal ]
1. 变量初值 **InitVal** → Exp | '{' [ InitVal { ',' InitVal } ] '}'
1. 函数定义 **FuncDef** → FuncType Ident '(' [ FuncFParams ] ')' Block
1. 函数类型 **FuncType** → 'void' | 'int'
1. 函数形参表 **FuncFParams** → FuncFParam { ',' FuncFParam }
1. 函数形参 **FuncFParam** → BType Ident [ '[' ']' { '[' ConstExp ']' } ]
1. 语句块 **Block** → '{' { BlockItem } '}'
1. 语句块项 **BlockItem** → Decl | Stmt
1. 语句
**Stmt** → LVal '=' Exp ';'
| [Exp] ';'
| Block
| 'if' '( Cond ')' Stmt [ 'else' Stmt ]
| 'while' '(' Cond ')' Stmt
| 'break' ';'
| 'continue' ';'
| 'return' [Exp] ';'
1. 左值表达式 **LVal** → Ident { '[' Exp ']' }
1. 表达式 **Exp** → LOrExp
1. 逻辑或表达式 **LOrExp** → LAndExp | LOrExp '||' LAndExp
1. 逻辑与表达式 **LAndExp** → EqExp | LAndExp '&&' EqExp
1. 相等性表达式 **EqExp** → RelExp | EqExp ('==' | '!=') RelExp
1. 关系表达式 **RelExp** → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
1. 加减表达式 **AddExp** → MulExp | AddExp ('+' | '−') MulExp
1. 乘除模表达式 **MulExp** → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
1. 一元表达式 **UnaryExp** → PrimaryExp | Ident '(' [ FuncRParams ] ')' | UnaryOp UnaryExp
1. 基本表达式 **PrimaryExp** → '(' Exp ')' | LVal | Number
1. 数值 **Number** → IntConst
1. 函数实参表 **FuncRParams** → Exp { ',' Exp }
1. 单目运算符 **UnaryOp** → '+' | '−' | '!'
1. 常量表达式 **ConstExp** → Exp 注：语义分析是需要确定为常量表达式

### 处理后

见 [syntax.md](syntax.md)
