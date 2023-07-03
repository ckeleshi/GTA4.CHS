import struct
import json
import sys
import time
from pathlib import Path
from array import array
from collections import namedtuple
from sys import exception

#将一个字符串转换成8字节串
def str_to_8bytes(string:str):
    arr = array('B')
    bstring = string.encode('utf-8')
    if len(bstring) == 0 or len(bstring) > 7:
        raise ValueError
    
    arr.extend(bstring)

    while len(arr) < 8:
        arr.append(0)

    return bytes(arr)
    

def load_jsons(folder):
    data = dict()
    for j in [j for j in Path(folder).glob('*.json') if j.is_file()]:
        with open(j,'r',encoding='utf-8') as f:
            table = json.load(f)
            for table_name in table:
                data[table_name] = table[table_name]

    return data

def write_gxt(tables,filename):
    TableEntry = namedtuple("TableEntry", "name offset")
    KeyEntry = namedtuple("KeyEntry", "offset hash")
    Path(filename).parent.mkdir(parents=True,exist_ok=True)
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
        table_entries = list()
        for table_name in sorted_keys:
            #生成Table Entry
            table_entries.append(TableEntry(str_to_8bytes(table_name),f.tell()))
            key_entries = list()
            #字符串序列化后的字节
            data_bytes = array('H') 
            for entry in tables[table_name]:
                key_entries.append(KeyEntry(len(data_bytes)*2,entry['hash']))
                if len(entry['translated']) == 0:
                    str_to_serialize = entry['original']
                else:
                    str_to_serialize = entry['translated']
                
                data_bytes.extend([ord(c) for c in str_to_serialize])
                data_bytes.append(0)
                pass

            #写入key block
            if table_name != 'MAIN':
                f.write(str_to_8bytes(table_name))

            f.write(b'TKEY')
            f.write(struct.pack('I', len(key_entries)*12))
            for key_entry in key_entries:
                f.write(struct.pack('II', key_entry.offset, key_entry.hash))
            
            #写入data block
            f.write(b'TDAT')
            f.write(struct.pack('I',len(data_bytes)*2))
            f.write(data_bytes)

        #最后写入TableBlock
        f.seek(12)
        table_block_bytes = array('B')
        for table_entry in table_entries:
            table_block_bytes.extend(struct.pack("8sI",table_entry.name,table_entry.offset))
        
        f.write(table_block_bytes)

if __name__ == '__main__':
    start_time = time.perf_counter()
    tables = load_jsons(sys.argv[1])
    write_gxt(tables, sys.argv[2])
    print(f'Process Time: {time.perf_counter() - start_time:.2f}s.')
