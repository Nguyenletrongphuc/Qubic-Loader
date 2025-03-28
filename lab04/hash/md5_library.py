import hashlib

def calculate_md5(input_string):
    md5_hash = hashlib.md5()
    md5_hash.update(input_string.encode('utf-8'))
    return md5_hash.hexdigest()

input_string = input("Enter string:")
md5_hash = calculate_md5(input_string)

print("MD5 của chuỗi '{}' là: {}".format(input_string, md5_hash))

