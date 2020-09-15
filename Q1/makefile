#MAKEFILE

all: prefix_table prefix_table_opt

prefix_table: prefix_table.cpp
	clang++-9 prefix_table.cpp -o prefix_table

prefix_table_opt: prefix_table_opt.cpp
	clang++-9 prefix_table_opt.cpp -o prefix_table_opt

.PHONY: clean

clean:
	-rm prefix_table prefix_table_opt



