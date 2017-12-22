import json
import sys
import unicodedata
from base64 import b64decode
from StringIO import StringIO
from gzip import GzipFile

# filename passed as commandline argument
filename = sys.argv[1]
if(filename == ''):
    print('No JSON input file provided')
    sys.exit()

# load JSON data
data = json.load(open(filename))

# store values normalized
content = unicodedata.normalize('NFKD', data[0]['content']).encode('ascii','ignore')

# unzip
string = GzipFile(fileobj=StringIO(b64decode(content))).read()

print(string)
