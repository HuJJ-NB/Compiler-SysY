# 处理后的文法

1. **CompUnit** → 'const' BType Ident DefIndexs '=' ConstInitVal ConstDef_Repeat ';' CompUnit
1. **CompUnit** → BType Ident DefIndexs VarInit_Exist VarDef_Repeat ';' CompUnit
1. **CompUnit** → BType Ident '(' FuncFParams ')' '{' BlockItems '}' CompUnit
1. **CompUnit** → 'void' Ident '(' FuncFParams ')' '{' BlockItems '}' CompUnit
1. **CompUnit** → $\varepsilon$
1. **BType** → 'int'
1. **DefIndexs** → '[' ConstExp ']' DefIndexs
1. **DefIndexs** → $\varepsilon$
1. **ConstInitVal** → ConstExp
1. **ConstInitVal** → '{' ConstInitVal_Repeat_Exist '}'
1. **ConstInitVal_Repeat_Exist** → ConstInitVal ConstInitVal_Repeat
1. **ConstInitVal_Repeat_Exist** → $\varepsilon$
1. **ConstInitVal_Repeat** → ',' ConstInitVal ConstInitVal_Repeat
1. **ConstInitVal_Repeat** → $\varepsilon$
1. **ConstDef_Repeat** → ',' Ident DefIndexs '=' ConstInitVal ConstDef_Repeat
1. **ConstDef_Repeat** → $\varepsilon$
1. **VarInit_Exist** → '=' VarInitVal
1. **VarInit_Exist** → $\varepsilon$
1. **VarInitVal** → Exp
1. **VarInitVal** → '{' VarInitVal_Repeat_Exist '}'
1. **VarInitVal_Repeat_Exist** → VarInitVal VarInitVal_Repeat
1. **VarInitVal_Repeat_Exist** → $\varepsilon$
1. **VarInitVal_Repeat** → ',' VarInitVal VarInitVal_Repeat
1. **VarInitVal_Repeat** → $\varepsilon$
1. **VarDef_Repeat** → ',' Ident DefIndexs VarInit_Exist VarDef_Repeat
1. **VarDef_Repeat** → $\varepsilon$
1. **FuncFParams** → BType Ident FuncFParam_Indexs FuncFParam_Repeat
1. **FuncFParams** → $\varepsilon$
1. **FuncFParam_Indexs** → '[' ']' DefIndexs
1. **FuncFParam_Indexs** → $\varepsilon$
1. **FuncFParam_Repeat** → ',' BType Ident FuncFParam_Indexs FuncFParam_Repeat
1. **FuncFParam_Repeat** → $\varepsilon$
1. **BlockItems** → Stmt BlockItems
1. **BlockItems** → 'const' BType Ident DefIndexs '=' ConstInitVal ConstDef_Repeat ';' BlockItems
1. **BlockItems** → BType Ident DefIndexs VarInit_Exist VarDef_Repeat ';' BlockItems
1. **BlockItems** → $\varepsilon$
1. **Stmt** → Exp_Exist ';'
1. **Stmt** → '{' BlockItems '}'
1. **Stmt** → 'if' '( Exp ')' Stmt Else_Exist
1. **Stmt** → 'while' '(' Exp ')' Stmt
1. **Stmt** → 'break' ';'
1. **Stmt** → 'continue' ';'
1. **Stmt** → 'return' Exp_Exist ';'
1. **Else_Exist** → 'else' Stmt
1. **Else_Exist** → $\varepsilon$
1. **Indexs** → '[' Exp ']' Indexs
1. **Indexs** → $\varepsilon$
1. **Exp_Exist** → Exp
1. **Exp_Exist** → $\varepsilon$
1. **Exp** → UnaryExp MulExp_T AddExp_T RelExp_T EqExp_T LAndExp_T LOrExp_T
1. **Exp** → Ident Exp_B_Ident
1. **Exp_B_Ident** → Indexs Exp_B_LVal
1. **Exp_B_Ident** → '(' FuncRParams ')' MulExp_T AddExp_T RelExp_T EqExp_T LAndExp_T LOrExp_T
1. **Exp_B_LVal** → '=' Exp
1. **Exp_B_LVal** → MulExp_T AddExp_T RelExp_T EqExp_T LAndExp_T LOrExp_T
1. **LOrExp_T** → '||' UnaryExp_Ident MulExp_T AddExp_T RelExp_T EqExp_T LAndExp_T LOrExp_T
1. **LOrExp_T** → $\varepsilon$
1. **LAndExp_T** → '&&' UnaryExp_Ident MulExp_T AddExp_T RelExp_T EqExp_T LAndExp_T
1. **LAndExp_T** → $\varepsilon$
1. **EqExp_T** → '==' UnaryExp_Ident MulExp_T AddExp_T RelExp_T EqExp_T
1. **EqExp_T** → '!=' UnaryExp_Ident MulExp_T AddExp_T RelExp_T EqExp_T
1. **EqExp_T** → $\varepsilon$
1. **RelExp_T** → '<' UnaryExp_Ident MulExp_T AddExp_T RelExp_T
1. **RelExp_T** → '>' UnaryExp_Ident MulExp_T AddExp_T RelExp_T
1. **RelExp_T** → '<=' UnaryExp_Ident MulExp_T AddExp_T RelExp_T
1. **RelExp_T** → '>=' UnaryExp_Ident MulExp_T AddExp_T RelExp_T
1. **RelExp_T** → $\varepsilon$
1. **AddExp_T** → '+' UnaryExp_Ident MulExp_T AddExp_T
1. **AddExp_T** → '-' UnaryExp_Ident MulExp_T AddExp_T
1. **AddExp_T** → $\varepsilon$
1. **MulExp_T** → '\*' UnaryExp_Ident MulExp_T
1. **MulExp_T** → '/' UnaryExp_Ident MulExp_T
1. **MulExp_T** → '%' UnaryExp_Ident MulExp_T
1. **MulExp_T** → $\varepsilon$
1. **UnaryExp** → UnaryOp UnaryExp_Ident
1. **UnaryExp** → '(' Exp ')'
1. **UnaryExp** → IntConst
1. **UnaryExp_Ident** → Ident Ident_B
1. **UnaryExp_Ident** → UnaryExp
1. **Ident_B** → '(' FuncRParams ')'
1. **Ident_B** → Indexs
1. **FuncRParams** → Exp FuncRParam_Repeat
1. **FuncRParams** → $\varepsilon$
1. **FuncRParam_Repeat** → ',' Exp FuncRParam_Repeat
1. **FuncRParam_Repeat** → $\varepsilon$
1. **UnaryOp** → '+'
1. **UnaryOp** → '-'
1. **UnaryOp** → '!'
1. **ConstExp** → ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T ConstEqExp_T ConstLAndExp_T ConstLOrExp_T
1. **ConstLOrExp_T** → '||' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T ConstEqExp_T ConstLAndExp_T ConstLOrExp_T
1. **ConstLOrExp_T** → $\varepsilon$
1. **ConstLAndExp_T** → '&&' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T ConstEqExp_T ConstLAndExp_T
1. **ConstLAndExp_T** → $\varepsilon$
1. **ConstEqExp_T** → '==' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T ConstEqExp_T
1. **ConstEqExp_T** → '!=' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T ConstEqExp_T
1. **ConstEqExp_T** → $\varepsilon$
1. **ConstRelExp_T** → '<' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T
1. **ConstRelExp_T** → '>' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T
1. **ConstRelExp_T** → '<=' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T
1. **ConstRelExp_T** → '>=' ConstUnaryExp ConstMulExp_T ConstAddExp_T ConstRelExp_T
1. **ConstRelExp_T** → $\varepsilon$
1. **ConstAddExp_T** → '+' ConstUnaryExp ConstMulExp_T ConstAddExp_T
1. **ConstAddExp_T** → '-' ConstUnaryExp ConstMulExp_T ConstAddExp_T
1. **ConstAddExp_T** → $\varepsilon$
1. **ConstMulExp_T** → '\*' ConstUnaryExp ConstMulExp_T
1. **ConstMulExp_T** → '/' ConstUnaryExp ConstMulExp_T
1. **ConstMulExp_T** → '%' ConstUnaryExp ConstMulExp_T
1. **ConstMulExp_T** → $\varepsilon$
1. **ConstUnaryExp** → UnaryOp ConstUnaryExp
1. **ConstUnaryExp** → '(' ConstExp ')'
1. **ConstUnaryExp** → IntConst
