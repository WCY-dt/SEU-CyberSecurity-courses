import numpy as np
from matplotlib import pyplot


class KMeans(object):
    # initializations
    def __init__(self, k=1):  # k is the number of clusters
        self.K = k

    # fit the data
    def Fit(self, data):
        self.Centers = {}  # Centers is the dictionary of centers of clusters

        # choose the first K points to initialize centers
        for i in range(self.K):
            self.Centers[i] = data[i]

        while True:
            self.Clusters = {}  # Clusters is the dictionary of clusters of points
            # initialize the lists of centers
            for i in range(self.K):
                self.Clusters[i] = []

            # calculate the distance between each point and each center
            for X in data:
                Distances = []  # Distances is the list of distances of each point to each center
                for Center in self.Centers:
                    # calculate the 2-norm distance
                    Distances.append(np.linalg.norm(
                        X - self.Centers[Center], ord=2))
                # find out the index of the closest center
                Category = Distances.index(min(Distances))
                # add the point to the list of points belonging to the closest center
                self.Clusters[Category].append(X)

            # record the previous centers
            PreCenters = dict(self.Centers)

            # calculate the new centers
            for Cluster in self.Clusters:
                # calculate the mean of the points in the cluster
                self.Centers[Cluster] = np.average(
                    self.Clusters[Cluster], axis=0)

            # check if the new centers are the same as the previous centers
            Flag = True  # Flag is the flag of weather the centers are the same
            for Center in self.Centers:
                if self.Centers[Center].any() != PreCenters[Center].any():
                    # if the centers are not the same
                    Flag = False

            if Flag:  # satisfy the stopping condition
                break

    '''
    # predict the cluster for input data point
    def Predict(self, data):
        for X in data:
            Distances = []
            for Center in self.Centers:
                Distances.append(np.linalg.norm(
                    X - self.Centers[Center], ord=2))
            Category = Distances.index(min(Distances))
        return Category
    '''


if __name__ == '__main__':
    # the data set
    x = np.array([[0, 0], [3, 8], [2, 2], [1, 1], [5, 3],
                  [4, 8], [6, 3], [5, 4], [6, 4], [7, 5]])

    # plot the data
    pyplot.scatter(x[:, 0], x[:, 1])
    pyplot.show()

    Kmeans = KMeans(k=3)  # k=3 clusters
    Kmeans.Fit(x)  # fit the data

    # plot the data
    print(Kmeans.Centers)

    # plot the centers
    for Center in Kmeans.Centers:
        # red for the first center, blue for the second center, and green for the third center
        pyplot.scatter(Kmeans.Centers[Center][0], Kmeans.Centers[Center][1], marker='*', s=150, c=(
            'r' if Center == 0 else ('b' if Center == 1 else ('g' if Center == 2 else 'y'))))

    # plot the data
    for X in Kmeans.Clusters:  # iterate through the clusters
        # iterate through the points in the cluster
        for point in Kmeans.Clusters[X]:
            # red for the first center, blue for the second center, and green for the third center
            pyplot.scatter(point[0], point[1], c=(
                'r' if X == 0 else ('b' if X == 1 else ('g' if X == 2 else 'y'))))

    # prediction
    '''
    predict = np.array([[0, 9], [3, 9]])
    
    for X in predict:
        Category = Kmeans.Predict(X)
        pyplot.scatter(X[0], X[1], c=('r' if Category == 0 else (
            'b' if Category == 1 else ('g' if Category == 2 else 'y'))), marker='x')
    '''

    pyplot.show()
