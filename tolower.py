import os

textNames = os.listdir('../texts/')

for textName in textNames:
    f = open("../texts/" + textName, encoding="utf8")

    lines = f.readlines()
    f.close()
    f = open("../texts/" + textName, 'w')
    for i in range(len(lines)):
      str = ""
      for j in range(len(lines[i])):
        str += lines[i][j].lower()
      f.write(str)
    f.close()