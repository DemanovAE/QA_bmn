#!/bin/bash

#SBATCH -p nica
#SBATCH -t 12:00:00
#SBATCH -J QA
#SBATCH --mem-per-cpu=8G
#SBATCH -a 1-999
#SBATCH -o /nica/mpd1/demanov/data_bmn/qa_run8_out/log/%A_%a.out
#SBATCH -e /nica/mpd1/demanov/data_bmn/qa_run8_out/log/%A_%a.err
#SBATCH -w ncx114,ncx16[2-6],ncx168,ncx169,ncx17[5-7],ncx147,ncx182,ncx18[4-5],ncx207,ncx212,ncx217,ncx228
#SBATCH -N 1

source /cvmfs/nica.jinr.ru/sw/os/login.sh legacy
source /cvmfs/nica.jinr.ru/centos7/bmn/env.sh
#source /cvmfs/nica.jinr.ru/centos7/bmnroot/23.08.0/bmnroot_config.sh
source /cvmfs/nica.jinr.ru/centos7/bmnroot/dev/bmnroot_config.sh

export JOB_ID=${SLURM_ARRAY_JOB_ID}
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999))
export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+999))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+1998))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+2997))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+3996))


export MAIN_DIR=/lhep/users/demanov/bmn/qa
export IN_FIT_FILE=${MAIN_DIR}/OUT_24.04_new.root
export INPUT_DIR=${MAIN_DIR}/lists
export MACRO_EXE=${MAIN_DIR}/run8_qa_new.C
export OUT_DIR=/nica/mpd1/demanov/data_bmn/qa_run8_out/qa_24.04.0_cent
export LOG_DIR=/nica/mpd1/demanov/data_bmn/qa_run8_out/log

export OUT_FILE=${OUT_DIR}/qa_run8_${JOB_ID}_${TASK_ID}.root
export INPUT_FILE=${INPUT_DIR}/list_${TASK_ID}.list
export LOG=${LOG_DIR}/log_qa_run8_${JOB_ID}_${TASK_ID}.log

mkdir -p $OUT_DIR
mkdir -p $LOG_DIR

# Main process
echo "Node name: ${SLURMD_NODENAME}" &>> $LOG
echo "Fit file: ${IN_FIT_FILE}" &>> $LOG
echo "INFILE:  ${INPUT_FILE}" &>> $LOG
echo "Job Id:  ${JOB_ID}" &>> $LOG
echo "Task Id: ${TASK_ID}" &>> $LOG
echo "OUTFILE: ${OUT_FILE}" &>> $LOG

cd $MAIN_DIR
root -l -b -q $MACRO_EXE'("'${INPUT_FILE}'","'${OUT_FILE}'","'${IN_FIT_FILE}'")' &>> $LOG
echo "Job is finished" &>> $LOG
