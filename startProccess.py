import os

dirAddr = '../../texts/'

textNames = os.listdir(dirAddr)

count = 0

for textName in textNames:
    count += 1
    print('Start proccess',count, "from", len(textNames), "===", textName)

    if textName[0] == '.': continue
    
    if textName[-4::] == '.txt':
        os.rename(dirAddr + textName, dirAddr + textName[:-4:])
        textName = textName[:-4:]
        
    try:
        f = open(dirAddr + textName, encoding="utf-8")
        f.read()
    except:
        os.rename(dirAddr + textName, dirAddr + textName+'.txt')
        continue
        os.system(f'iconv -f 1251 -t utf8 ../texts/{textName} | tee {textName}')
    f = open(dirAddr + textName, encoding="utf-8")
    lines = f.readlines()
    f.close()
    f = open(dirAddr + textName, 'w', encoding="utf-8")
    for i in range(len(lines)):
        str = ""
        for j in range(len(lines[i])):
            str += lines[i][j].lower()
        f.write(str)
    f.close()

    os.system('C:/Users/nikita.stachinskiy/Desktop/phonotextCpp/build/Debug/app.exe \"' + textName + '\"')