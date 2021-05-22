

class HtmlReader:
    def __init__(self,path):
        self.path = path

    def read(self):
        with open(self.path, "r+") as stream:
            buf = stream.read()
        
        return buf
    

def main():
    reader = HtmlReader('G:/cppserver/CppServer/CppServer/templates/home.txt')
    print(reader.read())
 
main()