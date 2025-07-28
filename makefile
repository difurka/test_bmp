F=# -Werror=catch-value -Wall -Wextra -Werror -fsanitize=address,undefined #-fno-elide-constructors  # -fno-elide-constructors, которая отменяет copy elision и требует вызова конструктора при возврате значения из функции.
all: bmp.h drawer.h
	g++ -std=c++20  ${F} main.cpp bmp.cpp
	./a.out