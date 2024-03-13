import os

textNames = os.listdir('../texts/')

for textName in textNames:
    print(textName)
    os.system('/Users/nikkk/Documents/phonotextCpp/build/app \"' + textName + '\"')