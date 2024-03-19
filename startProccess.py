import os

textNames = os.listdir('../texts/')

for textName in textNames:
    if textName[0] == '.': continue
    
    if textName[-4::] == '.txt':
        os.rename('../texts/' + textName, '../texts/' + textName[:-4:])
        textName = textName[:-4:]

    print(textName)

    try:
        f = open("../texts/" + textName, encoding="utf-8")
        f.read()
    except:
        os.rename('../texts/' + textName, '../texts/' + textName+'.txt')
        continue
        os.system(f'iconv -f 1251 -t utf8 ../texts/{textName} | tee {textName}')
    f = open("../texts/" + textName, encoding="utf-8")
    lines = f.readlines()
    f.close()
    f = open("../texts/" + textName, 'w', encoding="utf-8")
    for i in range(len(lines)):
        str = ""
        for j in range(len(lines[i])):
            str += lines[i][j].lower()
        f.write(str)
    f.close()

    os.system('/Users/nikkk/Documents/phonotextCpp/build/app \"' + textName + '\"')