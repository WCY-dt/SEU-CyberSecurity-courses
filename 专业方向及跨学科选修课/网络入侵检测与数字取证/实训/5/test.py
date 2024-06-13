from sklearn.ensemble import RandomForestClassifier
import numpy as np


'''
@member domain       [str]  : domain name
@member label        [bool] : notdga/dga
@member domainLength [int]  : length of the domain
@member domainNumber [int]  : joined digits of the domain
@member domainEntropy[float]: entropy of the domain
@member domainRoot   [bool] : root is uncommon/common
@member domainVowel  [float]: ratio of vowel in the domain
'''
class Domain:
    def __init__(self, _domain, _label):
        self.domain = _domain
        self.label  = _label

        self.domainLength  = getLength(self.domain)
        self.domainNumber  = getNumber(self.domain)
        self.domainEntropy = getEntropy(self.domain)
        self.domainRoot    = getRoot(self.domain)
        self.domainVowel   = getVowel(self.domain)

    def returnFeature(self):
        return [self.domainLength,
                self.domainNumber,
                self.domainEntropy,
                self.domainRoot,
                self.domainVowel]

    def returnLabel(self):
        if self.label == "notdga":
            return 0
        else:
            return 1


'''
@param Domain[str]
@return [int]Length of the domain
'''
def getLength(domain): # DGA is longer
    return len(domain)


'''
@param Domain[str]
@return [int]Join all the digits in the domain to a integer
'''
def getNumber(domain): # DGA has more digits
    numberList = ''.join(c for c in domain if c.isdigit()) or None

    if (numberList):
        return int(numberList)
    else:
        return -1


'''
@param Domain[str]
@return [float]The entropy of the domain
'''
def getEntropy(domain): # DGA has a higher entropy
    _, counts = np.unique(list(domain), return_counts=True)
    total = sum(counts)
    percent = list(map(lambda x: x / total, counts))
    return sum(-n * np.log(n) for n in percent)


'''
@param Domain[str]
@return [bin]Wether the root of the domain is common
'''
def getRoot(domain): # DGA has strange root
	commonRootList = ['cn', 'com', 'net', 'org', 'gov', 'info', 'edu']

	if domain.split('.')[-1] in commonRootList:
		return 1
	else:
		return 0


'''
@param Domain[str]
@return [float]The ratio of domainVowels in the domain
'''
def getVowel(domain): # DGA has lower ratio of domainVowel
	domainVowelList = ['a', 'e', 'i', 'o', 'u']

	domain = domain.lower()

	countWord   = 0
	countVowel  = 0
	domainVowel = 0

	for x in domain:
		if x.isalpha():
			countWord += 1
			if x in domainVowelList:
				countVowel += 1

	if countWord == 0:
		return 0
	else:
		return float(countVowel) / countWord



'''
@param filename[str]
@return [list]feature list, [list]label list
'''
def initTrain(filename):
    domainList = []

    with open(filename) as f:
        for line in f:
            line = line.strip()
            if line == "": # Exception
                continue

            tokens = line.split(",")
            domain = tokens[0]
            label = tokens[1]

            domainList.append(Domain(domain, label))
    
    return list(map(lambda domain: domain.returnFeature(), domainList)),\
           list(map(lambda domain: domain.returnLabel(), domainList))


'''
@param filename[str]
@return [list]domain list, [list]feature list
'''
def initTest(filename):
    with open(filename) as f:
        testDomain = list(filter(lambda line: line != "", map(lambda line: line.strip(), f)))
    
    testFeatureMatrix = []
    for domain in testDomain:
        testFeatureMatrix.append([getLength(domain),
                                   getNumber(domain),
                                   getEntropy(domain),
                                   getRoot(domain),
                                   getVowel(domain)])
    
    return testDomain, testFeatureMatrix


if __name__ == '__main__':

    # Read train data
    trainFeatureMatrix, trainLabelList = initTrain("train.txt")
    # Read test data
    testDomain, testFeatureMatrix = initTest("test.txt")
    
    # Train
    clf = RandomForestClassifier(random_state=0)
    clf.fit(trainFeatureMatrix, trainLabelList)

    # Predict
    testLabel = clf.predict(testFeatureMatrix)

    # Generate result
    output = list(zip(testDomain, testLabel))

    # Write result to file
    with open("result.txt", "w+") as f:
        for domain, label in output:
            line = domain + ","
            if label == 0:
                line = line + "notdga\n"
            else:
                line = line + "dga\n"
            f.write(line)
