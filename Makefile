$(out): main.cpp
	clang++ -g -Og -o $@ $^

clean:
	rm $(out)
