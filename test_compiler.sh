build/scdd -i ./test.scdd
nasm -felf64 out.s
ld -o out out.o
./out
echo $?
rm out out.o
