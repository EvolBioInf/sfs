../build/ms2sfs ../data/msOdd.dat > tmp.out
DIFF=$(diff tmp.out ../data/msOdd.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(ms2sfs, odd, unfolded)\tpass\n"
else
    printf "Test(ms2sfs, odd, unfoldedx)\tfail\n"
    echo ${DIFF}
fi

../build/ms2sfs -f ../data/msOdd.dat > tmp.out
DIFF=$(diff tmp.out ../data/msOddF.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(ms2sfs, odd, folded)\tpass\n"
else
    printf "Test(ms2sfs, odd, folded)\tfail\n"
    echo ${DIFF}
fi

../build/ms2sfs ../data/msEven.dat > tmp.out
DIFF=$(diff tmp.out ../data/msEven.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(ms2sfs, even, unfolded)\tpass\n"
else
    printf "Test(ms2sfs, even, unfolded)\tfail\n"
    echo ${DIFF}
fi

../build/ms2sfs -f ../data/msEven.dat > tmp.out
DIFF=$(diff tmp.out ../data/msEvenF.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(ms2sfs, even, folded)\tpass\n"
else
    printf "Test(ms2sfs, even, folded)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
