F=-Werror=catch-value -Wall -Wextra -Werror -fsanitize=address,undefined #-fno-elide-constructors  # -fno-elide-constructors, которая отменяет copy elision и требует вызова конструктора при возврате значения из функции.
ubuntu_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.out

windows_start: bmp.h drawer.h
	g++ ${F} main.cpp bmp.cpp
	./a.exe

# https://practicum.yandex.ru/trainer/cpp/lesson/f1dab89c-21e8-41bb-b983-d76b86874018/
clang_tidy:
	clang-tidy planets.cpp -- -std=c++17


# https://practicum.yandex.ru/trainer/cpp/lesson/f1dab89c-21e8-41bb-b983-d76b86874018/

F= -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG=1 -D_ITERATOR_DEBUG_LEVEL=2#-Wall -Wextra -Werror -fsanitize=address,undefined #-fno-elide-constructors  # -fno-elide-constructors, которая отменяет copy elision и требует вызова конструктора при возврате значения из функции.
debug:
	g++ -std=c++20 -O2 -lm -x c++ ${F} dots.cpp 
	./a.out

# https://practicum.yandex.ru/trainer/cpp/lesson/f1dab89c-21e8-41bb-b983-d76b86874018/
all:
	clang++ -fsanitize=undefined -std=c++17 int_numbers.cpp
	./a.out