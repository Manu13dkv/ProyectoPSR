#!/bin/sh
set -e
if [ $# != 2 ]
then
 echo "Usage: $0 cap-file sample time(s)"
 exit 1
fi
ifile="$1"
gran="$2"
d=$(mktemp -d)
tshark -o column.format:'"T","%t","L","%L"' -r "$ifile" | awk -v gran=$gran '
BEGIN {
 offset = 0
 printf("Time,Delta-Bytes,Bps\n");
}
{
 if ( ($1 - offset) > gran) {
 b=bytes[offset]
 offset += gran
 bytes[offset] = 0
 printf("%f,%u,%6.2f\n", offset, b, (8.0 * b) / gran)
 }
 bytes[offset] += $2
}
' > $d/x.dat
cat << EOF > $d/gnuplot.txt
set datafile separator ","
set title "$ifile bitrate (${gran}s sample time)"
set xlabel "Time(seconds)"
set ylabel "BandWidth(bps)"
plot "$d/x.dat" using 1:3 title "$ifile" with lines
pause -1
EOF
echo "Press return to finish"
gnuplot $d/gnuplot.txt
rm -rf $d

