PROJECT_DIR=$1
COMPILER=${PROJECT_DIR}/Third_Party/bin/thrift
COMPILE_DIR=IDL
OUTPUT_DIR=GeneratedFiles
#Create output dir if not exists
mkdir -p ${PROJECT_DIR}/${OUTPUT_DIR}
echo "Thrift build begins!!!"
for file in $(find $PROJECT_DIR"/"$COMPILE_DIR -name '*.thrift');
do
sh -c $COMPILER" --gen cpp --out ${PROJECT_DIR}/${OUTPUT_DIR} "$file
done



