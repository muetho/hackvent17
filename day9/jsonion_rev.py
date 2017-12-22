import json
import sys
import unicodedata
import base64

# filename passed as commandline argument
filename = sys.argv[1]
if(filename == ''):
    print('No JSON input file provided')
    sys.exit()

# load JSON data
data = json.load(open(filename))

# store values normalized
content = unicodedata.normalize('NFKD', data[0]['content']).encode('ascii','ignore')

# replace characters
string = ""
for i in range(len(content)):
    string += content[len(content)-1-i]

print(string)
