../build/bootSfs -s 13 -i 5 ../data/test.sfs > tmp.out
DIFF=$(diff tmp.out ../data/test.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(bootSfs)\tpass\n"
else
    printf "Test(bootSfs)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
