import subprocess
f = open('copy_ns.txt','r')
g = f.readlines()
for var in g:
    return_code = subprocess.call(var, shell=True)
    if (return_code):
        print("Error")