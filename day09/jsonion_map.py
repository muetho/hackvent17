import json
import sys

# filename passed as commandline argument
filename = sys.argv[1]
if(filename == ''):
    print('No JSON input file provided')
    sys.exit()

# load JSON data
data = json.load(open(filename))

# store values
map_from = data[0]['mapFrom']
map_to = data[0]['mapTo']
content = data[0]['content']

# replace characters
string = ""
for i in range(len(content)):
    for j in range(len(map_from)):
        if (content[i] == map_from[j]):
            string += (map_to[j])
            break

print(string)
