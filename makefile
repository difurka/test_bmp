F_U =  -Werror=catch-value -Wall -Wextra -Werror  -fsanitize=address,undefined 
F_W =  -Werror=catch-value -Wall -Wextra -Werror 

ubuntu_start: bmp.h drawer.h
	g++ ${F_U} main.cpp bmp.cpp
	./a.out

windows_start: bmp.h drawer.h
	g++ ${F_W} main.cpp bmp.cpp
	./a.exe
