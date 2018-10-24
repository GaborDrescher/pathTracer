.PHONY: all clean

all:
	g++ -Wall -Wextra -O3 -ffast-math -flto -I. -o pathtracer */*.cc *.cc

clean:
	rm -f pathtracer

