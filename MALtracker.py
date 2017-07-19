manga = {}
anime = {}
lightnovel = {}
succes = False
dicts = [anime, manga, lightnovel]
values = []
userInput = ["", "", ""]

def extract(values, dict):
    for i in values:
        i = i.strip()
        z = i.split(":")
        dict[z[0]] = int(z[1].strip())

def add():
    while True:
        try:
            userInput = input("Enter information in order: name, number, type\n").split(",")
            break
        except:
            print("enter valid information")

    return userInput

def view(dict, type):
    print(type + ":")
    for i in dict:
        print(i + ": " + str(dict[i]))
    print()

def viewAll(dictList, typeList):
    for i in range(len(dictList)):
        view(dictList[i], typeList[i])


try:
    datafile = open("MALdata.txt", "r+")
    information = datafile.read().split("*")[1:]
    for i in information:
        values.append(i.split("`")[1].split(",")[:-1])
    for i in range(len(values)):
        extract(values[i], dicts[i])

except:
    print("file not found")
    datafile = open("MALdata.txt", "w+")

while True:
    try:
        command = input("Please enter a command\n").lower()
        if command == "add":
            userInput = add()
        elif command == "view manga":
            view(manga, "manga")
        elif command == "view anime":
            view(anime, "anime")
        elif command == "view lightnovel":
            view(lightnovel, "lightnovel")
        elif command == "view all":
            viewAll(dict, ["anime", "manga", "lightnovel"])
        elif command == "quit" or command == "q":
            break

    except:
        print("please enter a valid command")

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
