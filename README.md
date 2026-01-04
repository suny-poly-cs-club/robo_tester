# Robot Tester

## Compile instructions

### Windows

#### MSVC
```cmd
cl gdi32.lib msvcrt.lib raylib.lib winmm.lib user32.lib shell32.lib main.cpp example.cpp -Ic:include /link /libpath:lib /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd4
```

#### MinGW
```cmd
g++ -o main.exe .\main.cpp .\example.cpp -I include -L lib -lraylib -lgdi32 -lwinmm
```

### Linux
```sh
g++ main.cpp example.cpp -I include -L lib_l -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o robo_test
```
Note: inorder to run you will need to add the library to you libray path, thanks linux
```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib_l
./robo_test
```

### Mac
You will need xcode build tools installed as usual
```sh
clang++ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib_x/libraylib.a main.cpp example.cpp -o robo_test
```