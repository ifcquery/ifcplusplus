cd $(dirname $0)/../..

SolutionName=sowin1

if [ -z "${COINDIR}" ]
then
  echo COINDIR must be set
  exit 1
fi

build/general/generate-all.sh ${SolutionName} 7 9
