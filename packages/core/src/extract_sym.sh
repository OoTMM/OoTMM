#!/bin/sh

nm -g -f bsd $1 | awk '{printf "%s = 0x%s;\n",$3,$1}' > $2
