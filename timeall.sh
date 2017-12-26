touch output.tsv

echo "Begin Timer..."


modes=( "BRUTEFORCE" "HUANG" "HUANGQS" "PERREAULT" "PERREAULTQS" )
length=30


for i in `seq 1 30`; do
  for m in "${modes[@]}"; do 
    echo $i $m $length
    echo "compile"
    g++ -w -D$m -DTIMELIMIT=$length -DWINDOW_SIZE=$i -g -O5 *.cpp -o bin/$m$i_$length
    echo "run"
    bin/$m$i_$length | tee -a output.tsv
    echo "sleep"
    # rm bin/$m$i
    sleep 5
  done

done
