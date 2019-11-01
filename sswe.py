import string
import random
import itertools
import numpy as np
import os

GAMMA = list(string.ascii_uppercase)
SIGMA = list(string.ascii_lowercase)


def expand(t, exp):
    exp_t = ''
    for c in t:
        if c in GAMMA:
            i = GAMMA.index(c)
            exp_t += exp[i]
        else:
            exp_t += c

    return exp_t


def check(s, t, idx, R_arr):
    exp = get_exp(idx, R_arr)
    return (expand(t, exp) in s)


def parse(filename):
    with open(filename, 'r') as f:

        lines = f.readlines()

        k = int(lines[0])
        s = lines[1].strip()
        t_arr = [line.strip() for line in lines[2:2+k]]
        R_arr = [line[2:].strip().split(',') for line in lines[2+k:]]

        return k,s,t_arr,R_arr


def get_exp(idx, R_arr):
    exp = []
    for i,x in enumerate(idx):
        exp.append(R_arr[i][x])

    return exp


def trim_R(R_arr, s):
    """Remove r_i that aren't already substrings of s"""
    trimmed_R_arr = []
    
    for R in R_arr:
        trimmed_R = [w for w in R if w in s]
        trimmed_R_arr.append(trimmed_R)

    return trimmed_R_arr


def inc(idx, max_arr):
    if idx == max_arr: 
        return None

    for i in reversed(range(len(idx))):
        if idx[i] != -1:
            idx[i] += 1
            if i < len(max_arr) and idx[i] > max_arr[i]:
                idx[i] = 0
            else:
                break

    return idx


def get_all_sols(s, t, R_arr, prev_sols):
    idx = [0 if c in t else -1 for c in GAMMA[:len(R_arr)]]
    max_arr = [len(R_arr[i]) - 1 if idx[i] != -1 else -1 for i in range(len(idx))]
    sols = []

    while idx is not None:
        if check(s, t, idx, R_arr): 
            sols.append(idx.copy())
        idx = inc(idx, max_arr)

    return sols


def verify(k, s, t_arr, R_arr):
    L = len(R_arr) - 1
    for t in t_arr:
        for c in t:
            if c in GAMMA and GAMMA.index(c) > L:
                # Input is invalid if c < 97 (upper case) and the index
                # of c-65 (A=0, B=1, etc.) is out of bounds in R_arr. 
                return False
    
    return True


def combine(sols, s, t_arr, R_arr):
    """Sols of dimensions (k, n_sols, len(R))"""
    all_options = [[] for _ in range(len(sols[0][0]))]
    for test in sols:
        for sol in test:
            for j,idx in enumerate(sol):
                if idx not in all_options[j] and idx != -1: 
                    all_options[j].append(idx)
    
    all_options = [x if x else [-1] for x in all_options]
    idx_arr = itertools.product(*all_options)
    
    for idx in idx_arr:
        passed = test_sol(idx, s, t_arr, R_arr)
        if passed:
            return idx
    
    return None


def test_sol(sol, s, t_arr, R_arr):
    exp = get_exp(sol, R_arr)
    passed = True
    for t in t_arr:
        if expand(t, exp) not in s:
            passed = False
            break
    return passed


if __name__ == '__main__':
    for filename in os.listdir('tests'):
        print(filename)
        k, s, t_arr, R_arr = parse(f'tests/{filename}')

        if verify(k, s, t_arr, R_arr):
            R_arr = trim_R(R_arr, s)

            sols = []
            for t in t_arr:
                sols.append(get_all_sols(s, t, R_arr, sols))

            sol = combine(sols, s, t_arr, R_arr)

            if sol:
                print('YES')
                print(sol)
            else:
                print('NO')
        else:
            print('NO')
