#!/bin/bash
gcc server.c -o server.out -lfcgi
spawn-fcgi -p8080 server.out
nginx -g "daemon off;"
