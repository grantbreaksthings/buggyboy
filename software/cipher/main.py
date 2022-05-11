#!/bin/python3

import sys

def encode_letter(c: str) -> str:
    if c == " ":
        return "*"
    if c == ".":
        return c
    c = c.lower()
    lut = ["_,@", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"]
    for entry in lut:
        current_index = lut.index(entry) + 1
        encoded_letter = str()
        for char in entry:
            encoded_letter = encoded_letter + str(current_index)
            if char == c:
                return encoded_letter
    return ""


def encode_string(s: str) -> str:
    ret = ""
    for char in s:
        encoded = encode_letter(char)
        if encoded:
            ret = ret + encoded + "|"
    return ret


if __name__ == '__main__':
    ins = " ".join(sys.argv[1:])
    print(f"encoding \"{ins}\"")
    print(encode_string(ins))

