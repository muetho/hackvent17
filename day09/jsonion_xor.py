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
mask = unicodedata.normalize('NFKD', data[0]['mask']).encode('ascii','ignore')
content = unicodedata.normalize('NFKD', data[0]['content']).encode('ascii','ignore')
# decode mask and content
mask = base64.b64decode(mask)
content = base64.b64decode(content)

# replace characters
string = ""
for i in range(0,len(content),len(mask)):
    for j in range(len(mask)):
        string += chr(ord(content[i+j]) ^ ord(mask[j]))

print(string)
