import hashlib
import math
import sys

HEX_DIGEST_LENGTH = 32
PASSWD_LENGTH = 5
ASCII_COUNT = 95

def h(s):
    m = hashlib.md5()
    m.update(s.encode('utf-8'))
    return m.hexdigest()

def b(hsh):
    passwd = ['\0'] * PASSWD_LENGTH

    for length in range(1, PASSWD_LENGTH + 1):
        print(f"Str length: {length}")
        iters = int(math.pow(ASCII_COUNT, length))
        powers_of_i = [int(math.pow(ASCII_COUNT, p)) for p in range(length)]

        for j in range(iters):
            for p in range(length - 1, -1, -1):
                if j % powers_of_i[p] == 0:
                    passwd[p] = chr((j // powers_of_i[p]) % ASCII_COUNT + 33)
                    break

            passwd_str = ''.join(passwd[:length])
            md5string = h(passwd_str)
            
            if md5string == hsh:
                print(f"Password bruted: {passwd_str}")
                return True
    return False

def m():
    if len(sys.argv) < 2:
        print("usage: python md5_bruteforce.py input-file")
        sys.exit(-1)

    filename = sys.argv[1]

    try:
        with open(filename, "r") as f:
            target_hash = f.read().strip()
    except IOError:
        print(f"Cannot open file: {filename}")
        sys.exit(-1)

    if len(target_hash) != HEX_DIGEST_LENGTH:
        print(f"Passed file has a string which is not {HEX_DIGEST_LENGTH} long.")
        sys.exit(-1)

    print(target_hash)

    b(target_hash)

if __name__ == "__main__":
    m()
