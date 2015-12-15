#!/bin/bash
CURRENTDIR=$PWD
path2profiler="/opt/AMD/CodeXL_1.6.7247/x86_64/sprofile"
path2exe="$CURRENTDIR/../build/linux/bin/sgemv"
workingdir="$CURRENTDIR"
profDir="$workingdir/sgemvProfileData"
mkdir -p $profDir
if [ ! -x $path2profiler ]; then
  echo "profiler does not exist..Exiting.."
  exit
fi
echo -e "\n M\t N\t Trans\t Imple\t Avg Time(ms)" >> $workingdir/profileSummary_sgemv.csv
while read line; do    
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    transA=$(echo $line | cut -f3 -d" " )
    Implem=$(echo $line | cut -f4 -d" " )
    datetime=$(date +%b-%d-%a_%H-%M-%S_)
    pc="perfCounter"
    path2outdir="$profDir/$datetime$Mvalue$Nvalue$transA$Implem"
    mkdir -p $path2outdir
    path2perf="$path2outdir/$pc"
    mkdir -p $path2perf
    cmd="(ls -a $path2outdir) | grep CLKernelSummary"
    if [ -x $path2exe ]; then
      echo $path2exe $Mvalue $Nvalue $transA $Implem
      runcmd="$path2profiler -o $path2outdir/output.atp -t -T -w $path2outdir $path2exe $Mvalue $Nvalue $transA $Implem --device gpu"
      echo $runcmd
      eval $runcmd
      echo $cmd
      filename="$(eval $cmd)"
      passarg=$path2outdir/$filename
      if [ -f "$workingdir/extracthtml_sgemv.py" ]; then
        python $workingdir/extracthtml_sgemv.py $passarg $Mvalue $Nvalue $transA $Implem
      fi
      runcmd2="$path2profiler -o $path2perf/output.csv -O -p -w $path2perf $path2exe $Mvalue $Nvalue $transA $Implem --device gpu"
      echo $runcmd2
      eval $runcmd2
    else
      echo $path2exe "doesnot exist" 
    fi
done < $workingdir/sgemv_input.txt
