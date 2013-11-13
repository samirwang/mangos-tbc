#!/bin/bash
HOST=127.0.0.1
USER=mangos
PASS=mangos
REALMD=realmd
CHARD=characters
MANGOSD=mangos

for f in *mangos*.sql
do
mysql -h $HOST -u $USER -p$PASS $MANGOSD < $f
done
 
for f in *realmd*.sql
do
mysql -h $HOST -u $USER -p$PASS $REALMD < $f
done
 
for f in *characters*.sql
do
mysql -h $HOST -u $USER -p$PASS $CHARD < $f
done