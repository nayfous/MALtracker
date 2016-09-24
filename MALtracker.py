manga = {}
anime = {}
lightnovel = {}
succes = False
try:
    datafile = open("MALdata.txt", "r+")
    information = datafile.read()
    information = information.split("*")[1:]
    animeValues = information[0].split("`")[1].split(",")[:-1]
    mangaValues = information[1].split("`")[1].split(",")[:-1]
    lightnovelValues = information[2].split("`")[1].split(",")[:-1]
    for i in animeValues:
        i = i.strip()
        z = i.split(":")
        anime[z[0]] = int(z[1].strip())
    print("hier1")
    for i in mangaValues:
        i = i.strip()
        z = i.split(":")
        manga[z[0]] = int(z[1].strip())
        print("hier2")
    for i in lightnovelValues:
        i = i.strip()
        z = i.split(":")
        lightnovel[z[0]] = int(z[1].strip())
        print("hier2")

except:
    datafile = open("MALdata.txt", "w+")
    information = ""


while True:
    try:
        userInput = input("Enter information in order: name, number, type\n")
        userInput = userInput.split(",")
        userInput[1] = int(userInput[1])
        break
    except:
        print("enter valid information")

datafile.close()
datafile = open("MALdata.txt", "w+")

if userInput[2].lower().strip() == "anime":
    anime[userInput[0].lower().strip()] = userInput[1]

elif userInput[2].lower().strip() == "manga":
    manga[userInput[0].lower().strip()] = userInput[1]

elif userInput[2].lower().strip() == "lightnovel":
    lightnovel[userInput[0].lower().strip()] = userInput[1]

datafile.write("*ANIME" + "-" * 100 + "`" + "\n")
for i in anime:
    datafile.write(i + ": " + str(anime[i]) + "," + "\n")
datafile.write("*MANGA" + "-" * 100 + "`" + "\n")
for i in manga:
    datafile.write(i + ": " + str(manga[i]) + "," + "\n")
datafile.write("*LIGHTNOVEL" + "-" * 100 + "`" + "\n")
for i in lightnovel:
    datafile.write(i + ": " + str(lightnovel[i]) + "," + "\n")

datafile.close()
