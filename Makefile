stage1: stage1.c stage1.h
	clang stage1.c -o stage1 -g -Wextra -Wall

stage2: stage1.c stage1.h stage2.c stage2.h
	clang stage2.c -o stage2 -g -Wextra -Wall
