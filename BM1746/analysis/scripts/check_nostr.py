# -*- coding: utf-8 -*-
# 转写文件静态自查: 括号配平 / 重复定义 / 行数
import re
import collections
import sys

path = sys.argv[1] if len(sys.argv) > 1 else 'nostr.c'
src = open(path, encoding='utf-8').read()

s = re.sub(r'/\*.*?\*/', '', src, flags=re.S)
s = re.sub(r'//[^\n]*', '', s)
s = re.sub(r'"(?:\\.|[^"\\])*"', '""', s)
s = re.sub(r"'(?:\\.|[^'\\])*'", "''", s)

print('braces:', s.count('{'), s.count('}'))
print('parens:', s.count('('), s.count(')'))
print('brackets:', s.count('['), s.count(']'))

depth = 0
closes = []
neg = None
for i, line in enumerate(s.split('\n'), 1):
    depth += line.count('{') - line.count('}')
    if depth < 0 and neg is None:
        neg = i
    if line.rstrip() == '}' and depth == 0:
        closes.append(i)
print('first negative line:', neg)
print('top-level closes at lines:', closes)
print('final depth:', depth)

defs = re.findall(
    r'^(?:static\s+)?(?:const\s+)?(?:void|int|char|short|long|float|double|unsigned\s+\w+|u?int(?:8|16|32|64)_t)\s+\**(\w+)\s*\(',
    src, flags=re.M)
dup = [k for k, v in collections.Counter(defs).items() if v > 1]
print('dup defs (decl+def pairs are expected):', dup if dup else 'NONE')
print('lines:', src.count('\n') + 1)
