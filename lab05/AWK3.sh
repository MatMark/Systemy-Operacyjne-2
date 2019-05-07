#!/bin/bash

#c

ps aux|awk \
' { \
if(NR!=1) \
CPU += $3; \
MEM += $4; \
} \
END \
{ \
print "CPU: " CPU "\tMEM: " MEM \
}'


