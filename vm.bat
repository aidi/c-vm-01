set NASMPATH=C:\Users\i7\AppData\Local\bin\NASM\nasm.exe
gcc -o vm-basic.exe vm-basic.c
vm-basic.exe
%NASMPATH% -f win64 out.asm -o out.obj
gcc out.obj -o out.exe -m64
out.exe

