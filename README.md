# Compiler-SysY

## 依赖

- 构建需要安装`pcre`库，Ubuntu系统可以使用`apt-get install libpcre3-dev`安装，其他系统可以用各自的包管理器安装。

## 运行

- `make run`或者`./build/app { INPUT_FILE [ -o OUTPUT_FILE ] }`

## 目录

``` shell
  .
  ├── Makefile
  ├── README.md
  ├── build
  │   ├── app
  │   ├── obj-app
  │   └── output
  ├── include
  │   ├── compiler.h
  │   ├── debug.h
  │   ├── lexer.h
  │   └── syntax.h
  ├── resource
  ├── script
  │   └── build.mk
  └── src
      ├── compiler.cc
      ├── lexer.cc
      ├── main.cc
      ├── output.cc
      └── syntax.cc
```

## TODO

- [x] 更改输出格式。
- [ ] 报错。
- [ ] 更加工程化。
- [ ] 语义分析。
