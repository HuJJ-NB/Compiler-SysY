# Compiler-SysY

## 依赖

- 构建需要安装`pcre`库，Ubuntu系统可以使用`apt-get install libpcre3-dev`安装，其他系统可以用各自的包管理器安装。

## 运行

- `make run`或者`./build/app SOURCE_FILE -l LOG_FILE`

## 目录

``` shell
  .
  ├── build
  ├── include
  │   ├── compiler.h
  │   ├── debug.h
  │   ├── lexer.h
  │   └── syntax.h
  ├── Makefile
  ├── README.md
  ├── resource
  │   └── test.c
  ├── script
  │   └── build.mk
  └── src
      ├── compiler.cc
      ├── lexer.cc
      ├── log.cc
      ├── main.cc
      └── syntax.cc
```

## TODO

- [ ] 更改输出格式。
- [ ] 使logfile和源文件在处理完参数之后统一按顺序打开。
