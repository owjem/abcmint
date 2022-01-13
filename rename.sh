#!/bin/bash

for file in $(find . -type f -name "*abcmint*"); do
  if [ -e "$file" ]; then
    echo "$file"
    newname=`echo "$file" | sed 's/abcmint/bitcoin/'`
    mv "$file" "$newname"
    echo "$newname"
  fi
done

for file in $(find ./src/test -type f -name "*Test"); do
  if [ -e "$file" ]; then
    echo "$file"
    newname=`echo "$file" | sed 's/Test/_tests/'`
    mv "$file" "$newname"
    echo "$newname"
  fi
done

for file in $(find ./src/test -type f -name "*_tests.cpp"); do
  if [ -e "$file" ]; then
    echo "$file"
    newname=`echo "$file" | sed 's/_tests.cpp/_tests.cpp.bak/'`
    mv "$file" "$newname"
    echo "$newname"
  fi
done
