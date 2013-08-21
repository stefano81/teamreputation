#!/usr/bin/env python

import sys

def get_user(token):
    return token.strip().strip('"').split('[')[0]

def get_competences(token):
    competences = {}
    for c in  token.split('[')[1].strip().strip('"],').split(','):
        s = c.split('=')
        competences[s[0]] = float(s[1])

    return competences

def main():
    with open(sys.argv[1], 'r') as dotfile:
        edges = {}
        comp = {}
        for line in dotfile:
            if '{' in line or '}' in line:
                continue
            parts = line.strip().split('->')
            user = get_user(parts[0])
            if user not in comp:
                comp[user] = get_competences(parts[0])

            if 2 == len(parts):
                edge = parts[1].strip().strip('"').split('" ')
                label = edge[1].split('=')[1].strip('"]').replace('[', '=')
                user2 = get_user(edge[0])
                edges.setdefault(user, []).append((user2, label))
                if user2 not in comp:
                    comp[user2] = get_competences(edge[0])

        with open(sys.argv[2], 'wc') as userfile:
            for u in comp:
                l = '%s' % u
                for i in range(0,10):
                    k = 'c%02d' % i
                    if k in comp[u]:
                        l += ',%f' % comp[u][k]
                    else:
                        l += ',%f' % 0.0
                l += '\n'
                userfile.write(l)
                
        with open(sys.argv[3], 'wc') as edgefile:
            for u in edges:
                for v, l in edges[u]:
                    edgefile.write('{},{},{}\n'.format(u,v,l))
    
if __name__ == '__main__':
    main()
