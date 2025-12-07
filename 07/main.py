import sys

t_in = [l.rstrip() for l in sys.stdin.readlines()]

parts = [0] * len(t_in[0])
parts[t_in[0].find("S")] = 1

mirror = '^'

def print_parts(ps):
    for p in ps:
        if p != 0:
            print(f"{p}'", end = '')
    print('')

for row in t_in:
    print_parts(parts)
    for c,s in enumerate(row):
        if s == mirror:
            parts[c - 1] += parts[c]
            parts[c + 1] += parts[c]
            parts[c] = 0

print(f"Sum = {sum(parts)}")
