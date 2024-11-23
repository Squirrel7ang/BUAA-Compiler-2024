#!/bin/python3

import shlex
import subprocess
import json
import os

tests = []
test_names = []
test_files = []
input_files = []
output_files = []


def link():
    print("begin link_llvm... ")

    print("cp llvm_ir.txt main.ll")
    os.system("cp llvm_ir.txt main.ll")
    print("cp linkTest/lib.ll ./lib.ll")
    os.system("cp linkTest/lib.ll ./lib.ll")
    command1 = ['llvm-link', "main.ll", "lib.ll", "-S", "-o", "out.ll"]

    # link
    result1 = subprocess.Popen(command1, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = result1.communicate()
    if result1.returncode != 0:
        print("-"*30)
        print("llvm-link faild")
        print("-"*30)
        print("标准输出:")
        print(stdout)
        print("-"*30)
        print("标准错误输出:")
        print(stderr)
        print("-"*30)
        print("返回值:", result1.returncode)
        print("-"*30)

        print("cp testfile.txt ../build/testfile.txt")
        os.system("cp testfile.txt ../build/testfile.txt")
        print("cp main.ll ../build/llvm_ir.txt")
        os.system("cp main.ll ../build/llvm_ir.txt")
        exit(-1)
    print("")

def run(input_file_name):
    print("begin lli... ")
    command2 = ['lli', 'out.ll']

    if input_file_name != "":
        with open(input_file_name, 'r') as input_file:
            result2 = subprocess.Popen(command2, stdin=input_file, stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE, text=True)
    else:
        result2 = subprocess.Popen(command2, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = result2.communicate()
    if result2.returncode != 0:
        print("-"*30)
        print("lli faild")
        print("-"*30)
        print("标准输出:")
        print(stdout)
        print("-"*30)
        print("标准错误输出:")
        print(stderr)
        print("-"*30)
        print("返回值:", result2.returncode)
        print("-"*30)

        print("cp testfile.txt ../build/testfile.txt")
        os.system("cp testfile.txt ../build/testfile.txt")
        print("cp main.ll ../build/llvm_ir.txt")
        os.system("cp main.ll ../build/llvm_ir.txt")
        exit(-1)
    print("")

def _compile():
    print("begin compiling... ")
    process = subprocess.Popen(["./Compiler"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    process.wait()
    if not (process.returncode == 0):
        print("命令执行失败:")
        print("标准输出:", process.stdout)
        print("标准错误输出:", process.stderr)
        print("返回值:", process.returncode)

        print("cp testfile.txt ../build/testfile.txt")
        os.system("cp testfile.txt ../build/testfile.txt")
        print("cp main.ll ../build/llvm_ir.txt")
        os.system("cp main.ll ../build/llvm_ir.txt")
        exit(-1)
    print("")

def test(t):
    test_file = t["test_file"]
    input_file = t["input_file"]

    print("cp \"" + test_file + "\" \"./testfile.txt\"")
    os.system("cp \"" + test_file + "\" \"./testfile.txt\"")

    print("begin testing " + test_file)

    _compile()
    link()
    run(input_file)

    print("")

    print("Done")
    print("")

    pass

def clean():
    os.system("rm error.txt input.txt lexer.txt llvm_ir.txt parser.txt symbol.txt testfile.txt lib.ll main.ll out.ll")
    pass

def read_tests():
    filename = 'testfiles.json'

    with open(filename, 'r') as file:
        data = json.load(file)

    for test_name, test_info in data.items():
        test_file = test_info['testfile']
        input_file = test_info['input']
        output_file = test_info['output']
        new_test = {"test_name": test_name, "test_file": test_file, "input_file": input_file, "output_file": output_file}
        tests.append(new_test)

    pass

def get_compiler():
    os.system("cp ../build/Compiler ./Compiler")

def test_all_files():
    read_tests()
    get_compiler()
    for t in tests:
        test(t)
    clean()

    pass


if __name__ == '__main__':
    test_all_files()
