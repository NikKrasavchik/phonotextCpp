import pandas as pd
import sys

textNames = []

def dissambleXlsx(xlsxName):
    global textNames

    xlsx = pd.read_excel(xlsxName)

    textNames = list(xlsx.iloc[:,0])

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
    global textNames
    textNames.append(sys.orig_argv[2])

    resultDictionary = dict.fromkeys(resultKeyNames)

    for i in range(len(resultKeyNames)):
        resultDictionary[resultKeyNames[i]] = resultValues[i]

    df = pd.DataFrame(resultDictionary)
    df.index = textNames

    df.to_excel(xlsxName, sheet_name='test')

def main():
    xlsxName = '../template.xlsx'
    jsonName = '../data/outJson.json'

    xlsxKeyNames, xlsxValues = dissambleXlsx(xlsxName)
    resultKeyNames, resultValues = dissambleJson(jsonName, xlsxKeyNames, xlsxValues)
    rewriteXlsx(xlsxName, resultKeyNames, resultValues)

if __name__ == '__main__':
    main()