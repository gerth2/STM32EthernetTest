import os,gzip,json
from string import Template

from cachetools import RRCache

TMPLT_FILE = "./index_tmplt.c"
OUT_FILE = "../Core/Src/networking/index.c"
src_file_list = []

class FileContents():
    def __init__(self, source):

        self.sourceFile = source
        self.url = "/" + self.sourceFile.strip()
        self.codeName = self.sourceFile.replace(".", "_")

        # Hacky way to define mime type
        tmp = os.path.splitext(source)
        self.mimeType = tmp[1].replace(".", "")

        with open(self.sourceFile, "r") as infile:
            self.fileContentsStr = infile.read().encode("utf-8")
            self.compressedBytes = gzip.compress(self.fileContentsStr)

    def getCompressedDeclaration(self):
        return "static const U8 {}[] = {{ {} }};".format(self.codeName, ", ".join([hex(x) for x in self.compressedBytes]))

    def getDeclaration(self):
        #return "static const U8 {}[] = {{ {} }};".format(self.codeName, ", ".join(["\'{}\'".format(char) for char in json.dumps(self.fileContentsStr).strip("\"").replace("\"", "\\\"").replace("\'", "\\\'")]))
        return "static const U8 {}[] = {{ {} }};".format(self.codeName, ", ".join([hex(x) for x in self.fileContentsStr]))


if __name__ == "__main__":

    # Find all files to include
    for file in os.listdir("."):
        if file.endswith(".html") or file.endswith(".js") or file.endswith(".css"):
            print("Adding {}".format(file))
            src_file_list.append(FileContents(file))

    with open(OUT_FILE, "w") as outfile:
        pageData = ""
        for file in src_file_list:
            pageData += (file.getDeclaration() + "\n\n")

        first = True
        switchyard = ""
        for file in src_file_list:
            if(first):
                switchyard += "   if(strcmp(path, \"{}\") == 0) {{\n".format(file.url)
                first = False
            else:
                switchyard += "else if(strcmp(path, \"{}\") == 0) {{\n".format(file.url)

            switchyard += "      dptr={};\n".format(file.codeName)
            switchyard += "      size=sizeof({});\n".format(file.codeName)
            switchyard += "      egz=egz_{};\n".format(file.mimeType)
            switchyard += "      printf(\"[WEBSERVER] Serving {}\\n\");\n".format(file.url)
            switchyard += "   } "

        switchyard += "else {\n"
        switchyard += "      //URL Not found\n"
        switchyard += "      printf(\"[WEBSERVER] Could not find requested resource!\\n\");\n"
        switchyard += "      return 0;\n"
        switchyard += "   } \n\n"


        with open(TMPLT_FILE, "r") as tmpltFile:
            tmplt = Template(tmpltFile.read())
            outfile.write(tmplt.substitute(pageData = pageData, pageSwitchyard = switchyard))
        

            

