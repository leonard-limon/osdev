
# LCOS 0.2

## FIXes:
* fix interrupt bug (that does not print the exception_messages ==>
	address not correct : 0x00)
* fix keyboard / character printing bug ('l' works...)
* fix cursor problem

## TODOs:
* handle backspace printing
* fix the sleep routine (timer_.asm)
* change paging setup so that kernel stack cannot corrupt kernel code
