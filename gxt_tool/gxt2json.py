import struct
import json
import sys
import time
from pathlib import Path
from array import array
from collections import namedtuple

def load_gxt(filename):
    data = dict()

    with open(filename, "rb") as f:
        gxt_bin = f.read()
    
    version, bits_per_char = struct.unpack('HH', gxt_bin[0:4])  # 读取header
    tabl_block_offset = 4
    tabl, tabl_data_size = struct.unpack('4sI', gxt_bin[tabl_block_offset:tabl_block_offset+8])  # 读取table header
    tabl_data_offset = 12
    tabl_count = tabl_data_size//12 #计算table个数
    for table_index in range(tabl_count): #逐个读取table
        table_array = list()
        tabl_name_bytes, tkey_block_offset = struct.unpack('8sI',gxt_bin[tabl_data_offset+12*table_index:tabl_data_offset+12*table_index+12])
        tabl_name_str=tabl_name_bytes.decode('utf-8').strip('\0')
        if (tabl_name_str != 'MAIN'):
            tkey_block_offset+=8
        
        tkey,tkey_data_size = struct.unpack('4sI', gxt_bin[tkey_block_offset:tkey_block_offset+8])
        tkey_data_offset = tkey_block_offset+8
        tkey_count = tkey_data_size//8
        tdat_block_offset = tkey_data_offset+tkey_data_size
        tdat, tdat_data_size = struct.unpack('4sI', gxt_bin[tdat_block_offset:tdat_block_offset+8])
        tdat_data_offset = tdat_block_offset+8
        for key_index in range(tkey_count):
            dat_offset,crc = struct.unpack('II', gxt_bin[tkey_data_offset+8*key_index:tkey_data_offset+8*key_index+8])
            entry_bytes_start_offset = tdat_data_offset+dat_offset
            entry_bytes_cur_offset = 0
            dat_str = str()
            while True:
                unicode, = struct.unpack('H', gxt_bin[entry_bytes_start_offset+entry_bytes_cur_offset:entry_bytes_start_offset+entry_bytes_cur_offset+2])
                entry_bytes_cur_offset+=2
                if unicode != 0:
                    dat_str+=chr(unicode)
                else:
                    break

                #TODO: 过滤/替换不支持的原版字符

            table_array.append({'hash':crc,'desc':'','original':dat_str,'translated':''})

        data[tabl_name_str] = table_array #将当前表插入总表

    return data

def write_jsons(data:dict, folder):
    Path(folder).mkdir(parents=True,exist_ok=True)
    for table in data.items():
        with open(Path(folder,f"{table[0]}.json"), "w") as f:
            f.write(json.dumps({table[0]:table[1]},indent=4,sort_keys=False))

if __name__ == '__main__':
    start_time = time.perf_counter()
    data = load_gxt(sys.argv[1])
    write_jsons(data, sys.argv[2])
    print(f'Process Time: {time.perf_counter() - start_time:.2f}s.')
