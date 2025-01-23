CC = gcc
CFLAGS  = -g -Wall 
LDFLAGS = -lm
 
default: test1

test1: test_assign1_1.o dberror.o storage_mgr.o
	$(CC) $(CFLAGS) -o test1 test_assign1_1.o dberror.o storage_mgr.o $(LDFLAGS)

test2: test_assign1_2.o dberror.o storage_mgr.o
	$(CC) $(CFLAGS) -o test2 test_assign1_2.o dberror.o storage_mgr.o $(LDFLAGS)

test_assign1_1.o: test_assign1_1.c dberror.h storage_mgr.h test_helper.h
	$(CC) $(CFLAGS) -c test_assign1_1.c

test_assign1_2.o: test_assign1_2.c dberror.h storage_mgr.h test_helper.h
	$(CC) $(CFLAGS) -c test_assign1_2.c

dberror.o: dberror.c dberror.h 
	$(CC) $(CFLAGS) -c dberror.c

storage_mgr.o: storage_mgr.c storage_mgr.h
	$(CC) $(CFLAGS) -c storage_mgr.c

clean: 
	del /q *.o *.exe

run_test1:
	./test1

run_test2:
	./test2