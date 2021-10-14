import functions as fu
method = int(input())
array_size = int(input())
array = []
for i in range(array_size):
    x = float(input())
    array.append(x)
    y = float(input())
    array.append(y)

print(array)
val = float(input())
print(method)

if method == 0:
    cl = fu.clasterise(array, array_size, val)
elif method == 1:
    cl = fu.maxmin(array, array_size, val)
elif method == 2:
    cl = fu.kmean(array, array_size, val)

file = open("out.txt", "w")
file.write(str(len(cl)))
file.write(" ")
for i in range(len(cl)):
    file.write(str(len(cl[i])))
    if i != len(cl)-1:
        file.write(" ")
file.write("\n")
file.close()

file = open("out.txt", "a")
for i in range(len(cl)):
    for j in range(len(cl[i])):
        for k in range(2):
            file.write(str(cl[i][j][k]))
            file.write(" ")
        file.write("\n")

file.close()
