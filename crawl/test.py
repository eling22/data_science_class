import operator
list_str = ['123','11','13','ba','b1','b12']

test = [[12, 'tall', 'blue', 1],
[2, 'short', 'red', 9],
[4, 'tall', 'blue', 13]]

#list_str.sort()

#print(list_str)

print(sorted(test,key=lambda x: (x[1],x[3])))

#print(sorted(test,key=operator.itemgetter(1,0)))