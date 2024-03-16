import pandas as pd
import sys

textNames = []
textAuthors = []

def dissambleXlsx(xlsxName):
    global textNames, textAuthors

    xlsx = pd.read_excel(xlsxName)
    
    textAuthors = list(xlsx.iloc[:,1])
    textNames = list(xlsx.iloc[:,2])

    keys = xlsx.columns.tolist()
    map = dict.fromkeys(xlsx.columns.tolist())

    xlsxKeyNames = list(xlsx)[1::]

    xlsxValues = []

    for i in range(len(xlsxKeyNames)):
        tmpValues = list(xlsx[xlsxKeyNames[i]])
        xlsxValues.append(tmpValues)

    return xlsxKeyNames, xlsxValues

def dissambleJson(jsonName, xlsxKeyNames, xlsxValues):
    resultKeyNames = xlsxKeyNames
    resultValues = xlsxValues

    json = pd.read_json(jsonName)
    jsonKeyNames = json.columns.tolist()

    for i in range(len(xlsxKeyNames)):
        inJson = False
        for j in range(len(jsonKeyNames)):
            if (jsonKeyNames[j] == xlsxKeyNames[i]):
                inJson = True
                resultValues[i].append(list(json[jsonKeyNames[j]])[2])
                break
        if (not inJson):
            resultValues[i].append(0)

    for i in range(len(jsonKeyNames)):
        inXlsx = False
        for j in range(len(xlsxKeyNames)):
            if (xlsxKeyNames[j] == jsonKeyNames[i]):
                inXlsx = True
                break
        if (not inXlsx):
            resultKeyNames.append(jsonKeyNames[i])
            resultValues.append([0 for k in range(len(resultValues[0]))])
            resultValues[-1][-1] = list(json[jsonKeyNames[i]])[2]

    return resultKeyNames, resultValues

def rewriteXlsx(xlsxName, resultKeyNames, resultValues):
    global textNames, textAuthors

    arg = sys.orig_argv[2].split('_')

    name = arg[0]
    author = arg[1]
    
    textNames.append(name)
    textAuthors.append(author)

    resultDictionary = {'Author': None, 'Name': None}
    resultDictionary.update(dict.fromkeys(resultKeyNames))

    for i in range(len(resultKeyNames)):
        if resultKeyNames[i] == 'Author':
            resultDictionary[resultKeyNames[i]] = textAuthors
        elif resultKeyNames[i] == 'Name':
            resultDictionary[resultKeyNames[i]] = textNames
        else:
            resultDictionary[resultKeyNames[i]] = resultValues[i]

    df = pd.DataFrame(resultDictionary)

    df.to_excel(xlsxName, sheet_name='test')

def main():
    xlsxName = '../stats.xlsx'
    jsonName = '../data/outJson.json'

    xlsxKeyNames, xlsxValues = dissambleXlsx(xlsxName)
    resultKeyNames, resultValues = dissambleJson(jsonName, xlsxKeyNames, xlsxValues)
    rewriteXlsx(xlsxName, resultKeyNames, resultValues)

if __name__ == '__main__':
    main()