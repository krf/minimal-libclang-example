# Build

```
$ make
```

# Test

```
make test
```

Should print out something along this (note: I stripped some of the completion results):

```
$ make test
clang++ -std=c++11 -I /usr/lib/llvm-4.0/include -L/usr/lib/llvm-4.0/lib  -lclang -o libclang_parse main.cpp
./libclang_parse test.cpp 7 9 -Wall
795
...
COMPLETION: SomeEnum aaa
COMPLETION: SomeEnum bbb
...
DIAGNOSTIC: test.cpp:9:5: expected expression
DIAGNOSTIC: test.cpp:6:13: enumeration values 'aaa' and 'bbb' not handled in switch
Total diagnostics available: 2
```
