import os

textNames = os.listdir('../texts/')

for textName in textNames:
    os.system('/Users/nikkk/Documents/phonotextCpp/build/app \"' + textName + '\"')