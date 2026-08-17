import sys

def collect_special_chars(input_file):
    """
    收集文本文件中的特殊字符并输出到指定文件
    
    参数:
        input_file (str): 输入文本文件路径
    """
    special_chars = set()
    
    try:
        # 读取输入文件并检测特殊字符
        with open(input_file, 'r', encoding='utf-8') as f:
            for line in f:
                for char in line:
                    # 检查特殊字符（Unicode码点大于255）
                    if ord(char) > 255:
                        special_chars.add(char)
        
        special_chars.discard(chr(0x2122))  # trademark
        special_chars.discard(chr(0x3000))
        special_chars.discard(chr(0xFEFF))
        
        char_per_line = 64
        char_index = 0

        # 将结果写入输出文件
        with open('chars.txt', 'w', encoding='utf-8') as f:
            if special_chars:
                # 按Unicode码点排序
                for char in sorted(special_chars, key=lambda c: ord(c)):
                    f.write(char)
                    char_index += 1
                    if (char_index >= char_per_line):
                        f.write('\n')
                        char_index = 0
                
        print("处理完成！结果已保存到 'chars.txt'")
        
        with open('char_table.dat', 'wb') as f:
            if special_chars:
                # 写入元素个数
                f.write(len(special_chars).to_bytes(4, byteorder='little'))
                # 按Unicode码点排序
                for char in sorted(special_chars, key=lambda c: ord(c)):
                    f.write(ord(char).to_bytes(4, byteorder='little'))
                
        print("处理完成！结果已保存到 'char_table.dat'")
        
    except FileNotFoundError:
        print(f"错误: 文件 '{input_file}' 未找到。")
    except Exception as e:
        print(f"发生错误: {str(e)}")

if __name__ == "__main__":
    collect_special_chars(sys.argv[1])
