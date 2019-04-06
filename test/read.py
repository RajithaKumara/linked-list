import sys

filename = sys.argv[1]

# open the file for reading
filehandle = open(filename, 'r')  
sum=0.00000000
count=0
squr=0.000000
List = [] 
while True:  
    # read a single line
    line = filehandle.readline()
    # if(line!="\n"):
    #     print (line)
   
    
    if not line:
        break
    count=count+1
    sum=sum+(float(line))
    List.append(float(line))
    

# close the pointer to that file
average= sum/count
print("mean time: %f" % average)
print("number of times: %f" % count)
for x in List:
  squr=squr+((average-x)**2)
std=(squr/count)**0.5

print("std : %f" % std)

n=((100*1.96*std)/(5*average))**2

print("n : %f" % n)

filehandle.close()  
