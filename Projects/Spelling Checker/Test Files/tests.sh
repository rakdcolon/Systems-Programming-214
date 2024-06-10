#!/bin/sh

SPCHK=./spchk

echo 1. correct words. no errors
echo ----
$SPCHK dict1 d1/c0.txt
echo ----
echo 2. correct words. no errors
echo ----
$SPCHK dict1 d1/c1.txt
$SPCHK dict1 d1/c2.txt
echo ----
echo 3. incorrect words. 1 error
echo ----
$SPCHK dict1 d1/nonword.txt
echo ----
echo 4a. incorrect capitalization. 1 error
echo ----
$SPCHK dict1 d1/incorrect-capitals1.txt
echo ----
echo 4b. incorrect capitalization. 3 errors
echo ----
$SPCHK dict1 d1/incorrect-capitals2.txt
echo ----
echo 5. hyphenated words. 2 errors
echo ----
$SPCHK dict1 d1/hyphen.txt
echo ----
echo 6. tricky punctuation. 4 errors
echo ----
$SPCHK dict1 d1/punct.txt
echo ----
echo 7. single directory. 2 errors
echo ----
$SPCHK dict1 d0/s3
echo ----
echo 9. recursive directory. 5 errors
echo ----
$SPCHK dict1 d0
echo ----
echo 10. traversal skips. no errors
echo ----
$SPCHK dict2 d0
echo ----
