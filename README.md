# Compiler-SysY

## 运行

* `make run`或者`./build/app`

## 词法分析

* 目录

  ``` shell
  .
  ├── build           //make生成的文件在这
  ├── include         //头文件
  │   ├── debug.h
  │   ├── lexer.h     //词法分析器中会被别的模块调用的内容
  │   └── utils.h
  ├── Makefile        //主要的makefile 包括一些运行的规则
  ├── README.md
  ├── script
  │   └── build.mk    //用于构建的makefile
  └── src
      ├── lexer.cc    //词法分析器
      └── main.cc     //不用说了
  ```

* 分工

  * 陈俊鑫
    * [ ] 词法分析部分的正则语法

  * 胡家骏
    * [x] 主要框架

  * 胡清华
    * [ ] 从文件输入

  * 潘相会
    * [ ] 支持用makefile给出待分析的文件
