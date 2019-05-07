#!/bin/bash

#a

ps aux|awk \
' { \
if(NR!=1) \
print "User: " $1 " - proces " $11 " (CPU: " $3 ", MEM: " $4 ")" \
} '
