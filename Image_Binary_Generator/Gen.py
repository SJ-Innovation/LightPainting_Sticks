from PIL import Image



## User Configurable Settings ===============

FileName = input("Enter Name of Image To Process: ")
LoadedImage = Image.open(FileName)
Resolution_OverSampling_Factor = 5


WalkingSpeed = 1.4#Meters per second
#Time_For_Image = 5000##Milliseconds       ##Only use one of these at a time!
#Time_Per_Row = 5 ##Milliseconds


##============================================
## Nothing below here is user-servicable. Please dont modify :)

Stick_Size = 144
LED_Width = 3#mm

try:
    WalkingSpeed
    RowsPerMeter = 1000/LED_Width
    RowsPerSecond = WalkingSpeed*RowsPerMeter
    Time_Per_Row = 1000/RowsPerSecond
except NameError:
    pass

try:
    Time_For_Image
except NameError:
    Time_For_Image = None

try:
    Time_Per_Row
except NameError:
    Time_Per_Row = None

if (Time_Per_Row == None and Time_For_Image == None):
    print("Error: Need to define either Time_Per_Pixel or Time_For_Image. You've defined neither/")
    exit(1)
if (Time_Per_Row != None and Time_For_Image != None):
    print("Error: Need to define either Time_Per_Pixel or Time_For_Image. You've defined both.")
    exit(1)




FileName  =FileName[:FileName.index(".")]
WriteFile = open(FileName+"-printme.lsk","wb")
OrigSize = LoadedImage.size
print("Input Image Size: X=",OrigSize[0] ,"Y=",OrigSize[1])
Normalisation_Factor = Stick_Size / OrigSize[1]
print("Image Scaling Factor:", Normalisation_Factor)
print("LED Oversampling Factor:",Resolution_OverSampling_Factor)



def RGBToBin(RGB):
    R,G,B = RGB[:3]
    R = R.to_bytes(1,byteorder = 'big')
    G = G.to_bytes(1,byteorder = 'big')
    B = B.to_bytes(1,byteorder = 'big')
    return R+G+B




NewSize = (round(OrigSize[0] * Normalisation_Factor * Resolution_OverSampling_Factor), Stick_Size)
if Time_Per_Row == None:
    Time_Per_Row = round(float(Time_For_Image) / float(NewSize[0]))
if Time_For_Image == None:
    Time_For_Image = round(Time_Per_Row * NewSize[0])

print("Resulting Image Size: X=",NewSize[0],"Y=",NewSize[1])
print("Each column will take:", str(Time_Per_Row) + "ms")
print("The whole picture will take:",str(Time_For_Image)+"ms")
LoadedImage = LoadedImage.resize(NewSize,resample=Image.LANCZOS)

WriteFile.write(round(Time_Per_Row / Resolution_OverSampling_Factor).to_bytes(2, byteorder = 'big')) #HoldTime

print("Writing Image Data")
#(0,0) is top left
for ReadX in range(LoadedImage.size[0]):
    for ReadY in range(Stick_Size):
        ThisPix = RGBToBin(LoadedImage.getpixel((ReadX,ReadY)))
        WriteFile.write(ThisPix)

WriteFile.close()
print("Done - Make sure to rename "+FileName+"-printme.lsk to printme.lsk !!")