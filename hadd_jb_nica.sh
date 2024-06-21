#!/bin/bash

#SBATCH -p nica
#SBATCH -t 12:00:00
#SBATCH -J hadd
#SBATCH --mem-per-cpu=10G
#SBATCH -a 1-79
#SBATCH -o /nica/mpd1/demanov/data_bmn/qa_run8_out/log/hadd_%A_%a.out
#SBATCH -e /nica/mpd1/demanov/data_bmn/qa_run8_out/log/hadd_%A_%a.err
#SBATCH -w ncx11[3-4],ncx182,ncx18[4-5],ncx203,ncx20[7-8],ncx21[1-4],ncx217,ncx22[4-6],ncx228
#SBATCH -N 1

source /cvmfs/nica.jinr.ru/sw/os/login.sh legacy
source /cvmfs/nica.jinr.ru/centos7/bmn/env.sh
#source /cvmfs/nica.jinr.ru/centos7/bmnroot/23.08.0/bmnroot_config.sh
source /cvmfs/nica.jinr.ru/centos7/bmnroot/dev/bmnroot_config.sh

export JOB_ID=${SLURM_ARRAY_JOB_ID}
export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999))

export MAIN_DIR=/lhep/users/demanov/bmn/qa
export INPUT_DIR=${MAIN_DIR}/hadd_lists
export OUT_DIR=/nica/mpd1/demanov/data_bmn/qa_run8_out/hadd_cent_${JOB_ID}
export LOG_DIR=/nica/mpd1/demanov/data_bmn/qa_run8_out/log

export OUT_FILE=${OUT_DIR}/hadd_qa_run8_${JOB_ID}_${TASK_ID}.root
export INPUT_FILE=${INPUT_DIR}/list_${TASK_ID}.list
export LOG=${LOG_DIR}/log_hadd_qa_run8_${JOB_ID}_${TASK_ID}.log

mkdir -p $OUT_DIR
mkdir -p $LOG_DIR

# Main process
echo "Node name: ${SLURMD_NODENAME}" &>> $LOG
echo "INFILE:  ${INPUT_FILE}" &>> $LOG
echo "Job Id:  ${JOB_ID}" &>> $LOG
echo "Task Id: ${TASK_ID}" &>> $LOG
echo "OUTFILE: ${OUT_FILE}" &>> $LOG
echo $(date '+%Y-%m-%d %H:%M:%S,%3N') &>>$LOG

cd $MAIN_DIR
hadd ${OUT_FILE} @${INPUT_FILE} &>> $LOG
#LD_PRELOAD=/mnt/pool/rhic/1/demanov/qntools_macros_femtodst/hadd/startup_C.so
#root -l -b -q $MACRO_EXE'("'${INPUT_FILE}'","'${OUT_FILE}'")' &>> $LOG
echo "Job is finished" &>> $LOG
echo $(date '+%Y-%m-%d %H:%M:%S,%3N') &>>$LOG
