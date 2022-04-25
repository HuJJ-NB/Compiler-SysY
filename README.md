# Compiler-SysY

## 运行

* `make run`或者`./build/app`

## 词法分析

* 目录

  ``` shell
  .
  ├── build           //make生成的文件在这
  ├── include         //头文件
  │   ├── common.h    //公共头文件
  │   ├── debug.h
  │   ├── lexer.h     //词法分析器中会被别的模块调用的内容
  │   └── utils.h
  ├── Makefile        //主要的makefile 包括一些运行的规则
  ├── README.md
  ├── script
  │   └── build.mk    //用于构建的makefile
  └── src
      ├── init.cc     //初始化函数
      ├── lexer.cc    //词法分析器源文件
      └── main.cc     //不用说了
  ```

* 任务

  * 理解整个框架，包括makefile，实现利用`make run ARGS=--src=={filename}`来对指定文件进行词法分析。

    * [ ] 实现词法分析器的正则语法。

    * [ ] 用命令`make run ARGS=string`的方式来给程序传递字符串参数，并输出。

    * [ ] 根据字符串给出的文件名读取文件，并进行词法分析。
