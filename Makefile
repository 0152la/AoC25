$(out): main.cpp
	clang++ -Wall -Wpedantic -Werror -g -O0 -std=c++20 -o $@ $^

clean:
	rm $(out)
