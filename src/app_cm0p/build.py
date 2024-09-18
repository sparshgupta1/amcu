import subprocess
f = open('build.txt','r')
g = f.readlines()
for var in g:
    if (var[0] == '#'):
        print('Skip')
        continue
    return_code = subprocess.call(var, shell=True)
    if (return_code):
        print("Error")