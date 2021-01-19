#!/bin/bash

echo "open 127.0.0.1 8080" 
sleep 2 
echo "GET / HTTP/1.1" 
echo "host: webserv" 
echo "authorization: basic dXNlcjQyOnVzZXI0Mg" 
echo 
echo 
sleep 2
