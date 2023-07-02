import struct
import json
import sys
import time
import pathlib

def load_jsons(folder):
    data = dict()
    for j in [j for j in pathlib.Path(folder).glob('*.json') if j.is_file()]:
        with open(j,'r',encoding='utf-8') as f:
            table = json.load(f)
            for table_name in table:
                data[table_name] = table[table_name]

    return data

def write_gxt(tables,filename):
    pathlib.Path(filename).parent.mkdir(parents=True,exist_ok=True)
    with open(filename,"wb") as f:
        f.write(b'\x04\x00\x10\x00')
        f.write(b'TABL')
        f.write(struct.pack('I', len(tables)*12))
        #预留table data的空隙，先写入TKEY & TDAT并计算出Table的Offset值，再一次性写入TablBlock
        tkey_block_offset = 12+len(tables)*12
        f.seek(tkey_block_offset)
        f.write(b'TKEY')
        #排序表名，首先写入'MAIN'table
        sorted_keys = ['MAIN']+[table_name for table_name in tables.keys() if table_name != 'MAIN']
        for table_name in sorted_keys:
            pass

if __name__ == '__main__':
    start_time = time.perf_counter()
    tables = load_jsons(sys.argv[1])
    write_gxt(tables, sys.argv[2])
    print(f'Process Time: {time.perf_counter() - start_time:.2f}s.')
