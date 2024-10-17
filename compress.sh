#!/bin/bash

# 定义要打包的文件和目录
FILES_TO_ZIP="bin libs CMakeLists.txt config.json"

# 定义输出的zip文件名
OUTPUT_ZIP="Compiler.zip"

# 删除已存在的同名zip文件
if [ -f "$OUTPUT_ZIP" ]; then
    rm "$OUTPUT_ZIP"
fi

# 创建zip文件并添加文件和目录
zip -r "$OUTPUT_ZIP" $FILES_TO_ZIP

echo "压缩文件已创建：$OUTPUT_ZIP"