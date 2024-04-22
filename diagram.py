import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime as dt

textLength = 0
combNumbers = []
jsonKeyNames = []
dicCombSortedNumbers = {}

def parseJson(jsonName):
    global textLength, combNumbers, jsonKeyNames
    json = pd.read_json(jsonName)

    textLength = json.iloc[0]['Text length']

    jsonKeyNames = list(json)[1::]

    tmpCombNumbers = []
    for i in range(len(jsonKeyNames)):
        tmpValues = list(json[jsonKeyNames[i]])
        tmpCombNumbers.append(tmpValues[3])

    for i in range(len(tmpCombNumbers)):
        tmpComb = []
        strNum = ''
        for j in range(len(tmpCombNumbers[i])):
            if tmpCombNumbers[i][j] == ' ':
                tmpComb.append(int(strNum))
                strNum = ''
            else:
                strNum += tmpCombNumbers[i][j]
        combNumbers.append(tmpComb)

def preproccessValues():
    global textLength, combNumbers, dicCombSortedNumbers, jsonKeyNames
    combSortedNumbers = []
    for i in range(len(combNumbers)):
        sortedCombinations = []
        for j in range(10):
            firstNum = textLength/10*j
            lastNum = textLength/10*(j+1)

            sortedNumbers = []
            for k in range(len(combNumbers[i])):
                if (firstNum < combNumbers[i][k] and combNumbers[i][k] < lastNum):
                    sortedNumbers.append(combNumbers[i][k])
            
            sortedCombinations.append(sortedNumbers)
        combSortedNumbers.append(sortedCombinations)

    for i in range(len(combSortedNumbers)):
        dicCombSortedNumbers[jsonKeyNames[i]] = combSortedNumbers[i]

def createDiagram():
    global dicCombSortedNumbers

    fig = plt.figure()
    ax = plt.axes()
    ax.yaxis.grid(True)

    xAxe = []
    yAxe = []

    for i in range(len(dicCombSortedNumbers['вк'])):
        xAxe.append(int(textLength/10*i))
        yAxe.append(len(dicCombSortedNumbers['вк'][i]))

    plt.plot(xAxe, yAxe)
    plt.show()

    fig.savefig('diagrams/diagram.png')

def main():
    parseJson('./data/outJson.json')
    preproccessValues()
    createDiagram()



if __name__ == '__main__':
    main()