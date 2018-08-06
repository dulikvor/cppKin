PROJECT_DIR=$1
THIRD_PARTY_DIR=$2
COMPILER=${THIRD_PARTY_DIR}/bin/thrift
COMPILE_DIR=IDL
OUTPUT_DIR=GeneratedFiles
#Create output dir if not exists
mkdir -p ${PROJECT_DIR}/${OUTPUT_DIR}
echo "Thrift build begins!!!"
for file in $(find $PROJECT_DIR"/"$COMPILE_DIR -name '*.thrift');
do
sh -c $COMPILER" --gen cpp --out ${PROJECT_DIR}/${OUTPUT_DIR} "$file
done



