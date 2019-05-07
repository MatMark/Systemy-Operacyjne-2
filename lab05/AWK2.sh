#!/bin/bash

#b

ps aux|awk \
' { \
if(NR!=1) \
print "User: " $1 " - proces " a[split($11, a, "/")] " (CPU: " $3 ", MEM: " $4 ")" \
} '

