#!/bin/bash

path2profiler=$PATH_TO_SPROFILE
path2exe="$HCBLAS_PATH/source/testfiles/build/linux/Test_Executables/sger"
workingdir="$HCBLAS_PATH/profile"
profDir="$workingdir/sgerProfileData"
mkdir -p $profDir
if [ ! -x $path2profiler ]; then
  echo "profiler does not exist..Exiting.."
  exit
fi
echo -e "\n M\t N\t Imple\t Avg Time(ms)" >> $workingdir/profileSummary_sger.csv
while read line; do    
    Mvalue=$(echo $line | cut -f1 -d" " )
    Nvalue=$(echo $line | cut -f2 -d" " )
    Implem=$(echo $line | cut -f3 -d" " )
    datetime=$(date +%b-%d-%a_%H-%M-%S_)
    pc="perfCounter"
    path2outdir="$profDir/$datetime$Mvalue$Nvalue$Implem"
    mkdir -p $path2outdir
    path2perf="$path2outdir/$pc"
    mkdir -p $path2perf
    cmd="(ls -a $path2outdir) | grep CLKernelSummary"
    if [ -x $path2exe ]; then
      echo $path2exe $Mvalue $Nvalue $Implem
      runcmd="$path2profiler -o $path2outdir/output.atp -t -T -w $path2outdir $path2exe $Mvalue $Nvalue $Implem --device gpu"
      echo $runcmd
      eval $runcmd
      echo $cmd
      filename="$(eval $cmd)"
      passarg=$path2outdir/$filename
      if [ -f "$workingdir/extracthtml_sger.py" ]; then
        python $workingdir/extracthtml_sger.py $passarg $Mvalue $Nvalue $Implem
      fi
      runcmd2="$path2profiler -o $path2perf/output.csv -O -p -w $path2perf $path2exe $Mvalue $Nvalue $Implem --device gpu"
      echo $runcmd2
      eval $runcmd2
    else
      echo $path2exe "doesnot exist" 
    fi
done < $workingdir/sger_input.txt
