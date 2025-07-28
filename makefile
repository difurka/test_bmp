F=# -Werror=catch-value -Wall -Wextra -Werror -fsanitize=address,undefined #-fno-elide-constructors  # -fno-elide-constructors, которая отменяет copy elision и требует вызова конструктора при возврате значения из функции.
ubuntu_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.out

windows_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.exe