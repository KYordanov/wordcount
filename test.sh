# sanity test generate example dict file and text file, run the program and check the result for correctness

touch dictfile
echo "foo" >> dictfile
echo "bar" >> dictfile
echo "#three" >> dictfile
echo "aaa" >> dictfile
echo "" >> dictfile

touch textfile
echo "One foo two bar three aaa this foo is test" > textfile
echo "" >> textfile

result=$(./wordcount dictfile textfile | tr -d '\t' | tr -d '\r'| tr -d '\n')
correct_result="1aaa1bar2foo10total words"

if [ "$result" = "$correct_result" ]; then
	echo "Test of the program successful"
else
	echo "Test of the program failed"
fi

rm dictfile
rm textfile

