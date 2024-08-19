exam:
	clang++ -g -O3 example.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o exam --ggdb

run: exam
	./exam

.PHONY: clean

clean:
	rm -f ./exam
