.PHONY: clean all run 
CC=g++

#Ofast          - fregacazzi delle specifiche dei Floating Point violate, i risultati sono uguali, e va faster
#march = native - dovrebbe essere faster
#std   = c++11  - tantissima roba che uso e' c++11 only
#DNDEBUG        - disabilita le assert. Se funzionava con le assert allora va anche senza
#fno-stack....  - mi fido di aver scritto codice senza buffer overflow. Piu' veloce!
#Wall		    - no warning = noob programmer, tanti warning = pro programmer!
#CFLAGS= -Ofast -march=native -std=c++11 -fno-stack-protector -DNDEBUG -Wall -pthread
CFLAGS= -O2 -std=c++11 -Wall -pthread
MAIN= kad

# i vari colori da utilizzare con gli errori. 32: verde 31: rosso 33:giallo
# 0m, nessun colore, 03m testo normale (non grassetto, ne' sottolineato)
NO_COLOR=\033[0m
OK_COLOR=\033[0;32m
ERROR_COLOR=\033[0;31m
WARN_COLOR=\033[0;33m
 
# le stringhe [OK] [ERROR] e [WARNING] con i vari colori
OK_STRING=$(NO_COLOR)[$(OK_COLOR)OK$(NO_COLOR)]
PASS_STRING = $(NO_COLOR)[$(OK_COLOR)PASS$(NO_COLOR)]
WARN_STRING=$(NO_COLOR)[$(WARN_COLOR)WARNING$(NO_COLOR)]
ERROR_STRING=$(NO_COLOR)[$(ERROR_COLOR)ERROR$(NO_COLOR)]
FAIL_STRING = $(NO_COLOR)[$(ERROR_COLOR)FAIL$(NO_COLOR)]
 
# contiene le informazioni su dove spedire eventuali errori di compilazione
ERROR_HANDLER = 2> temp.log || touch temp.errors;

all: $(MAIN)

run: $(MAIN)
	@echo
	@./$<

Messenger.o: Messenger.cpp
	@printf "Compiling $<..."
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Key.o: Key.cpp
	@printf "Compiling $<..."
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Ip.o: Ip.cpp
	@printf "Compiling $<... "
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Performer.o: Performer.cpp
	@printf "Compiling $<... "
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Node.o: Node.cpp
	@printf "Compiling $<... "
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Kbucket.o: Kbucket.cpp
	@printf "Compiling $<... "
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

Distance.o: Distance.cpp
	@printf "Compiling $<... "
	@$(CC) $(CFLAGS) -c $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

$(MAIN): main.cpp Messenger.o Key.o Ip.o Performer.o Node.o Kbucket.o Distance.o
	@printf "Compiling Executable..."
	@$(CC) $(CFLAGS) $^ -o $@ $(ERROR_HANDLER)
	@if test -e temp.errors; then echo "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then echo "$(WARN_STRING)" && cat temp.log; else echo "$(OK_STRING)"; fi;
	@rm -f temp.errors temp.log

clean:
	@rm -f *.o ~* $(MAIN)
