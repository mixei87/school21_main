#!/bin/bash

sed -i '' 's/[xyz]\[[0-9]\]\[[0-9]\] = //g' change.txt
sed -i '' 's/[,;]//g' change.txt
sed -i '' ':a
N
$!ba
s/\n/ /g' change.txt
sed -i '' 's/ \{1,\}/ /g' change.txt