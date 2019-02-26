.PHONY : all
all : expreval parsetree preinpost diff simplify
expreval: expreval.c
	gcc expreval.c
	./a.out part1/in1.txt
	./a.out part1/in2.txt
	./a.out part1/in3.txt
	./a.out part1/in4.txt
parsetree: parsetree.c
	gcc parsetree.c
	./a.out part1/in1.txt
	./a.out part1/in2.txt
	./a.out part1/in3.txt
	./a.out part1/in4.txt
preinpost: preinpost.c
	gcc preinpost.c
	./a.out part1/in1.txt
	./a.out part1/in2.txt
	./a.out part1/in3.txt
	./a.out part1/in4.txt
diff: diff.c
	gcc diff.c
	./a.out part2/in1.txt
	./a.out part2/in2.txt
	./a.out part2/in3.txt
	./a.out part2/in4.txt
	./a.out part2/in5.txt
	./a.out part2/in6.txt
	./a.out part2/in7.txt
	./a.out part2/in8.txt
simplify: simplify.c
	gcc simplify.c
	./a.out part2/in1.txt
	./a.out part2/in2.txt
	./a.out part2/in3.txt
	./a.out part2/in4.txt
	./a.out part2/in5.txt
	./a.out part2/in6.txt
	./a.out part2/in7.txt
	./a.out part2/in8.txt