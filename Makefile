walk: MonotoneBooleanFunction.cpp ShortList.cpp MonotoneBooleanFunction.h ShortList.h main.cpp
	g++ -O3 -march=native -mtune=native MonotoneBooleanFunction.cpp ShortList.cpp main.cpp -o walk