CC = gcc

.PHONY: clean

%.o: %.c
	$(CC) -c -o $@ $<

TS_Parser_TDT_tables: TS_Parser_TDT_tables.o
	$(CC) -o TS_Parser_TDT_tables TS_Parser_TDT_tables.o
clean:
	rm - TS_Parser_TDT_tables ./*.o
