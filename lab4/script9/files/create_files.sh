#!/bin/bash

for i in {0..100}
do 
  dd if=/dev/zero of=file_$i bs=64 count=1
done
