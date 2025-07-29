F = -Werror=catch-value -Wall -Wextra -Werror -fsanitize=address,undefined 

ubuntu_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.out

windows_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.exe
