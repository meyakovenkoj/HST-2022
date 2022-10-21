#!/bin/sh
set -x

usage() { echo "Usage: $0 -t <test.bin> [-r]" 1>&2; exit 1; }

while getopts ":rmgt:" o; do
    case "${o}" in
        r)
            MPIRUNFL=--allow-run-as-root
            ;;
        t)
            TESTBIN=${OPTARG}
            ;;
        m)
            MPIRUN=mpirun
            ;;
        g)
            NOGEN=1
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${TESTBIN}" ]; then
    usage
fi

if [ -z "${NOGEN}" ]; then
./gen.bin -s 100 -l 100
./gen.bin -s 200 -l 200
./gen.bin -s 400 -l 400
./gen.bin -s 800 -l 800
./gen.bin -s 1600 -l 1600
fi

$MPIRUN $MPIRUNFL ./${TESTBIN} -s 26214400 -l 100 gen_0100mb_1.txt gen_0100mb_2.txt
$MPIRUN $MPIRUNFL ./${TESTBIN} -s 52428800 -l 200 gen_0200mb_1.txt gen_0200mb_2.txt
$MPIRUN $MPIRUNFL ./${TESTBIN} -s 104857600 -l 400 gen_0400mb_1.txt gen_0400mb_2.txt
$MPIRUN $MPIRUNFL ./${TESTBIN} -s 209715200 -l 800 gen_0800mb_1.txt gen_0800mb_2.txt
$MPIRUN $MPIRUNFL ./${TESTBIN} -s 419430400 -l 1600 gen_1600mb_1.txt gen_1600mb_2.txt