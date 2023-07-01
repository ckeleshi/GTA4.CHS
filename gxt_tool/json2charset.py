import struct
import json
import sys
import pathlib
import time

def collect_chars(folder):
    chars = set()
    for j in [j for j in pathlib.Path(folder).glob('*.json') if j.is_file()]:
        with open(pathlib.Path(folder,j),'r',encoding='utf-8') as f:
            chars = chars.union(set(f.read()))

    for ascii in range(1,256):
        chars.discard(chr(ascii))
    
    chars.discard(chr(0x2122)) #trademark
    chars.discard(chr(0x3000))
    chars.discard(chr(0xFEFF))

    return list(chars)

def write_charset(chars, filename):
    char_per_line = 64
    char_index = 0
    chars.sort()
    pathlib.Path(filename).parent.mkdir(parents=True,exist_ok=True)
    with open(filename, "w", encoding='utf-8') as f:
        for char in chars:
            f.write(char)
            char_index+=1
            if (char_index>=char_per_line):
                f.write('\n')
                char_index = 0

if __name__ == '__main__':
    start_time = time.perf_counter()
    chars = collect_chars(sys.argv[1])
    write_charset(chars, sys.argv[2])
    print(f'Process Time: {time.perf_counter() - start_time:.2f}s.')
