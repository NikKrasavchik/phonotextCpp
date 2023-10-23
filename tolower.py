f = open("in.txt")
lines = f.readlines()
f.close()
f = open("out.txt", 'w')

for i in range(len(lines)):
  str = ""
  for j in range(len(lines[i])):
    str += lines[i][j].lower()
  f.write(str)
f.close()