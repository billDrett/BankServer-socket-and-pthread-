# In  order  to  execute  this  "Makefile " just  type  "make "
OBJS1     = main.o List.o Node.o bankAccount.o hashtable.o readWriteFunct.o transaction.o bankServer.o queue.o
OBJS2     = bankClient.o readWriteFunct.o
SOURCE   = main.cpp List.cpp Node.cpp bankAccount.cpp hashtable.cpp readWriteFunct.cpp transaction.cpp bankServer.cpp queue.cpp
HEADER1  = List.h Node.h bankAccount.h hashtable.h readWriteFunct.h transaction.h bankServer.h queue.h
HEADER2  = readWriteFunct.h
OUT      = server client
CC       = g++
FLAGS    = -g -c
# -g  option  enables  debugging  mode
# -c flag  generates  object  code  for  separate  files
all: server client
	
#  create/ compile  the  individual  files  >> separately <<
server: $(OBJS1)
	$(CC) -g $(OBJS1) -o server -pthread
client: $(OBJS2)
	$(CC) -g $(OBJS2) -o client
	
main.o: main.cpp $(HEADER1)
	$(CC) $(FLAGS) main.cpp
	
bankClient.o: bankClient.cpp $(HEADER2)
	$(CC) $(FLAGS) bankClient.cpp

bankAccount.o: bankAccount.cpp bankAccount.h
	$(CC) $(FLAGS) bankAccount.cpp

transaction.o: transaction.cpp transaction.h
	$(CC) $(FLAGS) transaction.cpp

boardServer.o: boardServer.cpp boardServer.h
	$(CC) $(FLAGS) boardServer.cpp

readWriteFunct.o: readWriteFunct.cpp readWriteFunct.h
	$(CC) $(FLAGS) readWriteFunct.cpp

queue.o: queue.cpp queue.h
	$(CC) $(FLAGS) queue.cpp

List.o: List.cpp List.h
	$(CC) $(FLAGS) List.cpp

Node.o: Node.cpp Node.h
	$(CC) $(FLAGS) Node.cpp

hashtable.o: hashtable.cpp hashtable.h
	$(CC) $(FLAGS) hashtable.cpp

#  clean  house
clean :
	rm -f $(OBJS1) $(OBJS2) $(OUT)
