#!/bin/bash -e
CURRENTDIR=$PWD
path2profiler="/opt/AMD/CodeXL_1.6.7247/x86_64/sprofile"
path2exe="$CURRENTDIR/../build/linux/bin/sgemm"
workingdir="$CURRENTDIR"
profDir="$workingdir/sgemmProfileData"
mkdir -p $profDir
if [ ! -x $path2profiler ]; then
  echo "profiler does not exist..Exiting.."
  exit
fi
echo -e "\n M\t N\t K\t TransA\t TransB\t Imple\t Avg Time(ms)" >> $workingdir/profileSummary_sgemm.csv
while read line; do
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Kvalue=$(echo $line | cut -f3 -d" " )
    transA=$(echo $line | cut -f4 -d" " )
    transB=$(echo $line | cut -f5 -d" " )
    Implem=$(echo $line | cut -f6 -d" " )
    datetime=$(date +%b-%d-%a_%H-%M-%S_)
    pc="perfCounter"
    path2outdir="$profDir/$datetime$Mvalue$Nvalue$Kvalue$transA$transB$Implem"
    mkdir -p $path2outdir
    path2perf="$path2outdir/$pc"
    mkdir -p $path2perf
    cmd="(ls -a $path2outdir) | grep CLKernelSummary"
    if [ -x $path2exe ]; then
      echo $path2exe $Mvalue $Nvalue $Kvalue $transA $transB  
      runcmd="$path2profiler -o $path2outdir/output.atp -t -T -w $path2outdir $path2exe $Mvalue $Nvalue $Kvalue $transA $transB $Implem --device gpu"
      echo $runcmd
      eval $runcmd
      echo $cmd
      filename="$(eval $cmd)"
      passarg=$path2outdir/$filename
      if [ -f "$workingdir/extracthtml_sgemm.py" ]; then
        python $workingdir/extracthtml_sgemm.py $passarg $Mvalue $Nvalue $Kvalue $transA $transB $Implem
      fi
      runcmd2="$path2profiler -o $path2perf/output.csv -O -p -w $path2perf $path2exe $Mvalue $Nvalue $Kvalue $transA $transB $Implem --device gpu"
      echo $runcmd2
      eval $runcmd2
    else
      echo $path2exe "doesnot exist" 
    fi
done < $workingdir/sgemm_input.txt
